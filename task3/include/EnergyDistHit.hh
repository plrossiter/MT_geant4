#ifndef ENERGYDISTHIT_HH
#define ENERGYDISTHIT_HH

#include <G4VHit.hh>
#include <G4THitsMap.hh>
#include <G4ThreeVector.hh>

/**
  * Custom hit class used in task 4d.
  *
  * It holds infomation about initial distribution of particles when
  * traversing scoring layer.
  */
class EnergyDistHit : public G4VHit
{
public:
  // Memory allocation and de-allocation
  inline void* operator new(size_t);
  inline void  operator delete(void*);
  
  void SetTrackEnergy(G4double trackE) { fTrackEnergy = trackE; }
  void SetTrackKineticEnergy(G4double trackKE) { fTrackKineticEnergy = trackKE; }
  void SetTrackTheta(G4double theta) { fTrackTheta = theta; }
  void SetParticleID_scorer(G4String id) { fParticleId_scorer = id; }
  void SetParentID_scorer(G4int parent_id) { fParentId_scorer = parent_id; }
  void SetTrackID_scorer(G4int track_id) { fTrackId_scorer = track_id; }
  void SetParticleName_scorer(G4String name) { fParticleName_scorer = name; }
  void SetTrackMomDir(G4ThreeVector momdir) { fTrackMomDir = momdir; }
  void SetTrackPosition(G4ThreeVector pos) { fTrackPos = pos; }


  G4double GetTrackEnergy() const { return fTrackEnergy; }
  G4double GetTrackKineticEnergy() const { return fTrackKineticEnergy; }
  G4double GetTrackTheta() const { return fTrackTheta; }
  G4String GetParticleId_scorer() const { return fParticleId_scorer; }
  G4int GetParentId_scorer() const { return fParentId_scorer; }
  G4int GetTrackId_scorer() const { return fTrackId_scorer; }
  G4String GetParticleName_scorer() const { return fParticleName_scorer; }
  G4ThreeVector GetTrackMomDir() const { return fTrackMomDir; }
  G4ThreeVector GetTrackPosition() const { return fTrackPos; }

private:

  G4double fTrackEnergy;
  G4double fTrackKineticEnergy;
  G4double fTrackTheta;
  G4String fParticleName_scorer;
  G4String fParticleId_scorer;
  G4int fParentId_scorer;
  G4int fTrackId_scorer;
  G4ThreeVector fTrackMomDir;
  G4ThreeVector fTrackPos;

};

using EnergyDistHitsCollection = G4THitsCollection<EnergyDistHit>;

extern G4ThreadLocal G4Allocator<EnergyDistHit> *hitAllocator_ED;

inline void* EnergyDistHit::operator new(size_t)
{
  if (!hitAllocator_ED)
  {
      hitAllocator_ED = new G4Allocator<EnergyDistHit>;
  }
  return hitAllocator_ED->MallocSingle();
}

inline void EnergyDistHit::operator delete(void *aHit_ED)
{
    if (!hitAllocator_ED)
    {
        hitAllocator_ED = new G4Allocator<EnergyDistHit>;
    }
    hitAllocator_ED->FreeSingle((EnergyDistHit*) aHit_ED);
}

#endif
