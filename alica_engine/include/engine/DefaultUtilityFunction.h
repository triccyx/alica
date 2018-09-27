#pragma once

#include "engine/UtilityFunction.h"

namespace alica
{

class Plan;

using std::make_shared;
using std::shared_ptr; // TODO: remove once autogenerated files have been reworked
/**
 * A default implementation for a plan's utility function. The only occuring summand referrs to the task-role
 * preferences.

 */
// TODO convert this into a free function once autogeneration templates can be changed
class DefaultUtilityFunction : public UtilityFunction
{
public:
    DefaultUtilityFunction(const Plan* plan);
};
} /* namespace alica */
