#include <alica_tests/Behaviour/NotToTrigger.h>
#include <memory>

/*PROTECTED REGION ID(inccpp1429017274116) ENABLED START*/
// Add additional includes here
/*PROTECTED REGION END*/

namespace alica
{
/*PROTECTED REGION ID(staticVars1429017274116) ENABLED START*/
// initialise static variables here
/*PROTECTED REGION END*/

NotToTrigger::NotToTrigger(BehaviourContext& context)
        : DomainBehaviour(context)
{
    /*PROTECTED REGION ID(con1429017274116) ENABLED START*/
    // Add additional options here
    this->callCounter = 0;
    this->initCounter = 0;
    /*PROTECTED REGION END*/
}
NotToTrigger::~NotToTrigger()
{
    /*PROTECTED REGION ID(dcon1429017274116) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
void NotToTrigger::run(void* msg)
{
    /*PROTECTED REGION ID(run1429017274116) ENABLED START*/
    // Add additional options here
    callCounter++;

    /*PROTECTED REGION END*/
}
void NotToTrigger::initialiseParameters()
{
    /*PROTECTED REGION ID(initialiseParameters1429017274116) ENABLED START*/
    // Add additional options here
    callCounter = 0;
    initCounter++;

    /*PROTECTED REGION END*/
}
/*PROTECTED REGION ID(methods1429017274116) ENABLED START*/
// Add additional methods here
/*PROTECTED REGION END*/

} /* namespace alica */
