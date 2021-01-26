#ifndef ENERGYTIMEHIT_HH
#define ENERGYTIMEHIT_HH

#include <G4VHit.hh>
#include <G4THitsMap.hh>
#include <G4ThreeVector.hh>

/**
  * Custom hit class used in task 4d.
  *
  * It holds infomation about energy deposits and position/time when
  * traversing a layer.
  */
class EnergyTimeHit : public G4VHit
{
public:
  // Memory allocation and de-allocation
  inline void* operator new(size_t);
  inline void  operator delete(void*);
  
  void SetDeltaEnergy(G4double deltaE) { fDeltaEnergy = deltaE; }
  void SetTime(G4double time) { fTime = time; }
  void SetPosition(G4ThreeVector pos) { fPosition = pos; }
  void SetParticleName(G4String name) { fParticleName = name; }
  void SetParticleID(G4int particleId) { fParticleID = particleId; }
  void SetPreStepMomDir(G4ThreeVector pre_mom_dir) { fPreStepMomDir = pre_mom_dir; }
  void SetPostStepMomDir(G4ThreeVector post_mom_dir) { fPostStepMomDir = post_mom_dir; }
  void SetPreStepMom(G4ThreeVector pre_mom) { fPreStepMom = pre_mom; }
  void SetPostStepMom(G4ThreeVector post_mom) { fPostStepMom = post_mom; }
  void SetKinetic_energy(G4double KE) { fKinetic_energy = KE; }
  void SetTheta(G4double Theta) { fTheta = Theta; }

  G4double GetDeltaEnergy() const { return fDeltaEnergy; }
  G4double GetTime() const { return fTime; }
  G4ThreeVector GetPosition() const { return fPosition; }
  G4String GetParticleName() const { return fParticleName; }
  G4int GetParticleID() const { return fParticleID; }
  G4ThreeVector GetPreStepMomDir() const { return fPreStepMomDir; }
  G4ThreeVector GetPostStepMomDir() const { return fPostStepMomDir; }
  G4ThreeVector GetPreStepMom() const { return fPreStepMom; }
  G4ThreeVector GetPostStepMom() const { return fPostStepMom; }
  G4double GetKinetic_energy() const { return fKinetic_energy; }
  G4double GetTheta() const { return fTheta; }

private:
    G4double fDeltaEnergy;
    G4double fTime;
    G4ThreeVector fPosition;
    G4String fParticleName;
    G4int fParticleID;
    G4ThreeVector fPreStepMomDir;
    G4ThreeVector fPostStepMomDir;
    G4ThreeVector fPreStepMom;
    G4ThreeVector fPostStepMom;
    G4double fKinetic_energy;
    G4double fTheta;
};

using EnergyTimeHitsCollection = G4THitsCollection<EnergyTimeHit>;

extern G4ThreadLocal G4Allocator<EnergyTimeHit> *hitAllocator;

inline void* EnergyTimeHit::operator new(size_t)
{
  if (!hitAllocator)
  {
      hitAllocator = new G4Allocator<EnergyTimeHit>;
  }
  return hitAllocator->MallocSingle();
}

inline void EnergyTimeHit::operator delete(void *aHit)
{
    if (!hitAllocator)
    {
        hitAllocator = new G4Allocator<EnergyTimeHit>;
    }
    hitAllocator->FreeSingle((EnergyTimeHit*) aHit);
}

#endif
