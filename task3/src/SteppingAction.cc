#include "SteppingAction.hh"
#include "RunAction.hh"

#include <G4Step.hh>
#include <G4MuonPlus.hh>
#include <G4MuonMinus.hh>

SteppingAction::SteppingAction(RunAction* runAction)
    : fRunAction(runAction)
{
}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{

    G4VPhysicalVolume* volume= aStep->GetPreStepPoint()->GetTouchable()->GetVolume();
  if(volume->GetName()){

    if(volume->GetName() == "world" && (aStep->GetTrack()->GetParticleDefinition() == G4MuonPlus::MuonPlus() || aStep->GetTrack()->GetParticleDefinition() == G4MuonMinus::MuonMinus())){

      G4double trackLength = aStep->GetTrack()->GetStepLength();
        fRunAction->AddTrackLength(trackLength);

    }


    }

}
