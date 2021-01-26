#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include <G4UserEventAction.hh>
#include <globals.hh>

class EventAction : public G4UserEventAction
{
public:
  void BeginOfEventAction(const G4Event* event);
  void EndOfEventAction(const G4Event* event) override;

private:
    // Numerical IDs for hit collections (-1 means unknown yet)
    G4int fScorerId { -1 };
    G4int fDetectorId_x { -1 };
    G4int fDetectorId_z { -1 };
  //G4int fScintillatorId { -1 };
    G4int fDetectorETId { -1 };
  //G4int fScintillatorETId { -1 };
    G4double fPhi { -1 };
    G4int fScorer_sum { 0 };

};

#endif
