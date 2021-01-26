#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH

#include <G4UserSteppingAction.hh>
#include "/home/rossiter/MuonTomography_2020/MuonTomographySIMS_210201/gitRepo_Sarah_2102/cry_v1.7/src/CRYSetup.h"
#include "/home/rossiter/MuonTomography_2020/MuonTomographySIMS_210201/gitRepo_Sarah_2102/cry_v1.7/src/CRYGenerator.h"

class RunAction;

class SteppingAction : public G4UserSteppingAction
{
public:
    //! constructor
    SteppingAction(RunAction*);

    void UserSteppingAction(const G4Step*) override;

private:
    RunAction* fRunAction;
};

#endif
