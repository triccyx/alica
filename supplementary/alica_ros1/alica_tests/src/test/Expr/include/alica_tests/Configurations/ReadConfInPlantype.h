#pragma once

#include <alica_tests/DomainCondition.h>
#include <alica_tests/DomainPlan.h>
#include <boost/dll/alias.hpp>
#include <engine/BasicUtilityFunction.h>
#include <engine/DefaultUtilityFunction.h>
#include <engine/UtilityFunction.h>

namespace alica
{
class ReadConfInPlantype : public DomainPlan
{
public:
    ReadConfInPlantype(PlanContext& context);
};

BOOST_DLL_ALIAS(alica::BasicPlan::create, ReadConfInPlantype)
BOOST_DLL_ALIAS(alica::BasicUtilityFunction::create, ReadConfInPlantypeUtilityFunction)
} /* namespace alica */
