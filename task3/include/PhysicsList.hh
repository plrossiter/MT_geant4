#ifndef PHYSICS_LIST_HH
#define PHYSICS_LIST_HH

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

//#include <G4VModularPhysicsList.hh>

class PhysicsList : public G4VUserPhysicsList
{
public:
  PhysicsList();
  ~PhysicsList();

  protected:
    // Construct particle and physics
    void ConstructParticle();
    void ConstructProcess();

    // Define tracking cuts (step length, etc)
    void SetCuts();
   
  // protected:
    // these methods Construct particles 
    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBaryons();
    void ConstructIons();

  protected:
    // these methods Construct physics processes and register them
    void ConstructGeneral();
    void ConstructInteractions();

};

#endif
