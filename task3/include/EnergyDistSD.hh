#ifndef ENERGYDIST_HH
#define ENERGYDIST_HH

#include <G4VSensitiveDetector.hh>

#include "EnergyDistHit.hh"

class EnergyDistSD : public G4VSensitiveDetector
{
public:
    EnergyDistSD(G4String name);

    void Initialize(G4HCofThisEvent*) override;

protected:
    G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) override;

private:
  EnergyDistHitsCollection* fHitsCollection { nullptr };
  G4int fHitsCollectionId { -1 };
};

#endif
