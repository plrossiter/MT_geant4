#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH

#include <G4VUserPrimaryGeneratorAction.hh>

#include "G4ThreeVector.hh"
#include "G4DataVector.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"
#include "Randomize.hh"
#include "globals.hh"
#include "/home/rossiter/MuonTomography_2020/MuonTomographySIMS_210201/gitRepo_Sarah_2102/cry_v1.7/src/CRYSetup.h"
#include "/home/rossiter/MuonTomography_2020/MuonTomographySIMS_210201/gitRepo_Sarah_2102/cry_v1.7/src/CRYGenerator.h"
#include "/home/rossiter/MuonTomography_2020/MuonTomographySIMS_210201/gitRepo_Sarah_2102/cry_v1.7/src/CRYParticle.h"
#include "/home/rossiter/MuonTomography_2020/MuonTomographySIMS_210201/gitRepo_Sarah_2102/cry_v1.7/src/CRYUtils.h"
#include "vector"
#include "RNGWrapper.hh"
#include "PrimaryGeneratorMessenger.hh"

//class G4ParticleGun;
//class G4GeneralParticleSource;

class G4Event;
// Task 2b.1 Include the proper header file or forward-declare the class for GPS

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction(const char * filename);
  ~PrimaryGeneratorAction();
  //  void GeneratePrimaries(G4Event* anEvent) override;

public:
  void GeneratePrimaries(G4Event* anEvent);
  void InputCRY();
  void UpdateCRY(std::string* MessInput);
  void CRYFromFile(G4String newValue);
  
  
private:
	// Task 2b.1: Replace the gun with a GPS instance called fGPS;
  //G4ParticleGun* fGun;
  //G4GeneralParticleSource* fGPS;
  std::vector<CRYParticle*> *vect; // vector of generated particles
  G4ParticleTable* particleTable;
  G4ParticleGun* particleGun;
  CRYGenerator* gen;
  G4int InputState;
  PrimaryGeneratorMessenger* gunMessenger;
};

#endif
