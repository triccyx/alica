#pragma once

//#define PSDEBUG

#include <vector>
#include <list>
#include <unordered_set>
#include <memory>
#include <sstream>

#include "engine/Types.h"

namespace supplementary {
class AgentID;
}

namespace alica {
class RunningPlan;
class AbstractPlan;
class TeamObserver;
class RunningPlan;
class AbstractPlan;
class PlanType;
class Plan;
class AlicaEngine;
class PartialAssignmentPool;

/**
 * Implements the task allocation algorithm
 */
class PlanSelector {
public:
    PlanSelector(AlicaEngine* ae, PartialAssignmentPool* pap);
    virtual ~PlanSelector();

    virtual std::shared_ptr<RunningPlan> getBestSimilarAssignment(std::shared_ptr<RunningPlan> rp);
    virtual std::shared_ptr<RunningPlan> getBestSimilarAssignment(std::shared_ptr<RunningPlan> rp, const AgentSet& robots);
    virtual std::shared_ptr<std::list<std::shared_ptr<RunningPlan>>> getPlansForState(
            std::shared_ptr<RunningPlan> planningParent, const AbstractPlanSet& plans, const AgentSet& robotIDs);

    std::shared_ptr<RunningPlan> createRunningPlan(std::weak_ptr<RunningPlan> planningParent, const PlanSet& plans,
            const AgentSet& robotIDs, std::shared_ptr<RunningPlan> oldRp,
            const PlanType* relevantPlanType);

private:
    PartialAssignmentPool* pap;
    TeamObserver* to;
    AlicaEngine* ae;
    std::shared_ptr<std::list<std::shared_ptr<RunningPlan>>> getPlansForStateInternal(
            std::shared_ptr<RunningPlan> planningParent, const AbstractPlanSet& plans,
            const AgentSet& robotIDs);
};

} /* namespace alica */
