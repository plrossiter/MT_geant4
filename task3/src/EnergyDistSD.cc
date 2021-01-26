#include "EnergyDistSD.hh"

#include <G4SDManager.hh>
#include <G4SystemOfUnits.hh>

EnergyDistSD::EnergyDistSD(G4String name) :
  G4VSensitiveDetector(name)
{
    collectionName.insert("energy_dist");
}

G4bool EnergyDistSD::ProcessHits(G4Step* aStep, G4TouchableHistory* /*ROhist*/)
{

    

  G4double trackEnergy = aStep->GetTrack()->GetTotalEnergy();
  G4double trackKineticEnergy = aStep->GetTrack()->GetKineticEnergy();
  G4double trackTheta = std::acos(aStep->GetTrack()->GetMomentumDirection().z());
  G4String particleId_scorer = aStep->GetTrack()->GetParticleDefinition()->GetParticleDefinitionID();
  G4int parentId_scorer = aStep->GetTrack()->GetParentID();
  G4int trackId_scorer = aStep->GetTrack()->GetTrackID();
  G4String particleName_scorer = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();
  G4ThreeVector trackMomDir = aStep->GetPreStepPoint()->GetMomentumDirection();
  G4ThreeVector trackPos = aStep->GetPreStepPoint()->GetPosition();
  

  EnergyDistHit* hit = new EnergyDistHit();
  // Fill in the hit properties                                                                                                                          
  
  hit->SetTrackEnergy(trackEnergy);
  hit->SetTrackKineticEnergy(trackKineticEnergy);
  hit->SetTrackTheta(trackTheta);
  hit->SetParticleID_scorer(particleId_scorer);
  hit->SetTrackID_scorer(trackId_scorer);
  hit->SetParentID_scorer(parentId_scorer);
  hit->SetParticleName_scorer(particleName_scorer);
  hit->SetTrackMomDir(trackMomDir);
  hit->SetTrackPosition(trackPos);


  
  // Add the hit to the collection 
  fHitsCollection->insert(hit);
    
  return true;
}

void EnergyDistSD::Initialize(G4HCofThisEvent* hcof)
{
    fHitsCollection = new EnergyDistHitsCollection(SensitiveDetectorName, collectionName[0]);
    if (fHitsCollectionId < 0)
    {
        fHitsCollectionId = G4SDManager::GetSDMpointer()->GetCollectionID(GetName() + "/" + collectionName[0]);
    }
    hcof->AddHitsCollection(fHitsCollectionId, fHitsCollection);
}
