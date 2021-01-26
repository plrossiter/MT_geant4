#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"


#include "RunAction.hh"
#include "StackingAction.hh"
#include "SteppingAction.hh"
#include "EventAction.hh"


const char *inputfile = ""; 
void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction(inputfile));

  RunAction* theRunAction = new RunAction();
  SetUserAction(theRunAction);
  SetUserAction(new StackingAction(theRunAction)); 
  SetUserAction(new SteppingAction(theRunAction)); 

  SetUserAction(new EventAction());

}

void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunAction());
}
