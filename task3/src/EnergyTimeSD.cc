#include "EnergyTimeSD.hh"

#include <G4SDManager.hh>
#include <G4SystemOfUnits.hh>

EnergyTimeSD::EnergyTimeSD(G4String name) :
  G4VSensitiveDetector(name)
{
    collectionName.insert("energy_time");
}

G4bool EnergyTimeSD::ProcessHits(G4Step* aStep, G4TouchableHistory* /*ROhist*/)
{

    
  G4double energy_deposit = aStep->GetTotalEnergyDeposit();
  G4double time = aStep->GetPostStepPoint()->GetGlobalTime();
  G4ThreeVector position = aStep->GetPostStepPoint()->GetPosition();
  G4String name = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();
  G4int particleId = aStep->GetTrack()->GetParticleDefinition()->GetParticleDefinitionID();
  G4ThreeVector momentum_dir_pre = aStep->GetPreStepPoint()->GetMomentumDirection(); 
  G4ThreeVector momentum_dir_post = aStep->GetPostStepPoint()->GetMomentumDirection(); 
  G4ThreeVector momentum_pre = aStep->GetPreStepPoint()->GetMomentum(); 
  G4ThreeVector momentum_post = aStep->GetPostStepPoint()->GetMomentum(); 
  G4double kinetic_energy = aStep->GetTrack()->GetKineticEnergy();
  G4double theta = std::acos(aStep->GetTrack()->GetMomentumDirection().z());
  
  EnergyTimeHit* hit = new EnergyTimeHit();
  // Fill in the hit properties                                                                                                                          
  
  hit->SetDeltaEnergy(energy_deposit);
  hit->SetTime(time);
  hit->SetPosition(position);
  hit->SetParticleName(name);
  hit->SetParticleID(particleId);
  hit->SetPreStepMomDir(momentum_dir_pre);
  hit->SetPostStepMomDir(momentum_dir_post);
  hit->SetPreStepMom(momentum_pre);
  hit->SetPostStepMom(momentum_post);
  hit->SetKinetic_energy(kinetic_energy);
  hit->SetTheta(theta);


  
  // Add the hit to the collection 
  fHitsCollection->insert(hit);
    
  return true;
}

void EnergyTimeSD::Initialize(G4HCofThisEvent* hcof)
{
    fHitsCollection = new EnergyTimeHitsCollection(SensitiveDetectorName, collectionName[0]);
    if (fHitsCollectionId < 0)
    {
        fHitsCollectionId = G4SDManager::GetSDMpointer()->GetCollectionID(GetName() + "/" + collectionName[0]);
    }
    hcof->AddHitsCollection(fHitsCollectionId, fHitsCollection);
}
