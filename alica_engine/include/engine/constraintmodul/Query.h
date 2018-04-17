#pragma once

//#define Q_DEBUG

#include "engine/AlicaEngine.h"
#include "engine/BasicBehaviour.h"
#include "engine/IAlicaClock.h"
#include "engine/RunningPlan.h"
#include "engine/TeamObserver.h"
#include "engine/constraintmodul/ConditionStore.h"
#include "engine/constraintmodul/ISolver.h"
#include "engine/constraintmodul/IVariableSyncModule.h"
#include "engine/constraintmodul/ProblemDescriptor.h"
#include "engine/constraintmodul/ProblemPart.h"
#include "engine/constraintmodul/SolverTerm.h"
#include "engine/constraintmodul/SolverVariable.h"
#include "engine/constraintmodul/VariableSyncModule.h"
#include "engine/model/Condition.h"
#include "engine/model/Parametrisation.h"
#include "engine/model/PlanType.h"
#include "engine/model/State.h"
#include "engine/model/Variable.h"

#include <map>
#include <memory>
#include <vector>


namespace alica {
class ProblemPart;
class RunningPlan;
class IAlicaClock;
class BasicBehaviour;

/**
 * Internal class to deal with bindings in states and plantypes
 */
class UniqueVarStore {
public:
    UniqueVarStore();

    void clear();
    void add(const Variable* v);
    const Variable* getRep(const Variable* v);
    void addVarTo(const Variable* representing, const Variable* toAdd);
    VariableSet getAllRep() const;
    int getIndexOf(const Variable* v) const;
    friend std::ostream& operator<<(std::ostream& os, const UniqueVarStore& store) {
        os << "UniqueVarStore: " << std::endl;
        // write obj to stream
        for (const auto& variableList : store.store) {
            os << "VariableList: ";
            for (const auto& variable : variableList) {
                os << *variable << ", ";
            }
            os << std::endl;
        }
        return os;
    }

private:
    /**
     *  Each inner list of variables is sorted from variables of the top most plan to variables of the deepest plan.
     *  Therefore, the first element is always the variable in the top most plan, where this variable occurs.
     */
    std::vector<VariableSet> store;
};

/**
 * Encapsulates queries to variables (which are associated with specific solvers).
 */
class Query {
public:
    Query();

    void addStaticVariable(const Variable* v);
    void addDomainVariable(const supplementary::AgentID* robot, const std::string& ident, AlicaEngine* ae);
    void clearDomainVariables();
    void clearStaticVariables();

    template <class SolverType>
    bool existsSolution(std::shared_ptr<RunningPlan> rp);

    template <class SolverType, typename ResultType>
    bool getSolution(std::shared_ptr<RunningPlan> rp, std::vector<ResultType>& result);

    const VariableSet& getRelevantStaticVariables() const { return relevantStaticVariables; }
    const VariableSet& getRelevantDomainVariables() const { return relevantDomainVariables; }
    void setRelevantStaticVariables(const VariableSet& value);
    void setRelevantDomainVariables(const VariableSet& value);

    void addProblemParts(std::vector<std::shared_ptr<ProblemPart>>& l);

    std::shared_ptr<UniqueVarStore> getUniqueVariableStore(); /*< for testing only!!! */

private:
    bool collectProblemStatement(std::shared_ptr<RunningPlan> rp, ISolverBase* solver,
            std::vector<std::shared_ptr<ProblemDescriptor>>& cds, VariableSet& relevantVariables, int& domOffset);

    std::shared_ptr<UniqueVarStore> uniqueVarStore;
    VariableSet queriedStaticVariables;
    VariableSet queriedDomainVariables;
    std::vector<shared_ptr<ProblemPart>> problemParts;

    VariableSet relevantStaticVariables;
    VariableSet relevantDomainVariables;
};

template <class SolverType>
bool Query::existsSolution(std::shared_ptr<RunningPlan> rp) {
    SolverType* solver = rp->getAlicaEngine()->getSolver<SolverType>();

    std::vector<std::shared_ptr<ProblemDescriptor>> cds;
    VariableSet relevantVariables;
    int domOffset;
    if (!collectProblemStatement(rp, solver, cds, relevantVariables, domOffset)) {
        return false;
    }
    return solver->existsSolution(relevantVariables, cds);
}

template <class SolverType, typename ResultType>
bool Query::getSolution(std::shared_ptr<RunningPlan> rp, std::vector<ResultType>& result) {
    result.clear();

    // Collect the complete problem specification
    std::vector<std::shared_ptr<ProblemDescriptor>> cds;
    VariableSet relevantVariables;
    int domOffset;
    SolverType* solver = rp->getAlicaEngine()->getSolver<SolverType>();
    if (solver == nullptr) {
        std::cerr << "Query::getSolution: The engine does not have a suitable solver for the given type available."
                  << std::endl;
        return false;
    }

    if (!this->collectProblemStatement(rp, solver, cds, relevantVariables, domOffset)) {
        return false;
    }

#ifdef Q_DEBUG
    std::cout << "Query: " << (*this->uniqueVarStore) << std::endl;
#endif
    // TODO: get rid of the interrim vector (see below how) 
    std::vector<ResultType> solverResult;
    // let the solver solve the problem
    bool ret = solver->getSolution(relevantVariables, cds, solverResult);


    if (ret && solverResult.size() > 0) {
        int i = 0;
        VariableSyncModule* rs = rp->getAlicaEngine()->getResultStore();
        for (const ResultType& value : solverResult) {
            rs->postResult(relevantVariables[i]->getId(), Variant(value));
            ++i;
        }

        //TODO this can be done in place. The queried static should be at the beginning of the array anyway
        // create a result vector that is filtered by the queried variables
        for (const Variable* staticVariable : queriedStaticVariables) {
            result.push_back(solverResult[uniqueVarStore->getIndexOf(staticVariable)]);
        }
        //Again, the queried domain variables should be at the beginning of the domain variable segment
        //So a simple move and resize should do the trick
        for (int i = 0; i < queriedDomainVariables.size(); ++i) {
            for (int j = 0; j < relevantDomainVariables.size(); ++j) {
                if (relevantDomainVariables[j] == queriedDomainVariables[i]) {
                    result.push_back(solverResult[domOffset + j]);
                    break;
                }
            }
        }
    }
    return ret;
}
} /* namespace alica */
