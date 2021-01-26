#include "globals.hh"
#include "PhysicsList.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ios.hh"
#include <iomanip>
#include "G4VUserPhysicsList.hh"
#include "G4VPhysicsConstructor.hh"


// This is a pre-defined physics list for CRY library. You can find it on the CRY library page for G4
// This file defines all the processes which are used for each particle. Here I had to change some things because some of the default options didn't work. Still I think neutrons don't properly work here, but since they are not CURRENTLY used it doesn't matter. Will need to be re-assessed in the future

//----------------------------------------------------------------------------//

PhysicsList::PhysicsList() : G4VUserPhysicsList()
{
  defaultCutValue = 1.*CLHEP::mm;
  SetVerboseLevel(1);
}

//----------------------------------------------------------------------------//

PhysicsList::~PhysicsList()
{
}

//----------------------------------------------------------------------------//
// Create all particles that can appear in this application.
//----------------------------------------------------------------------------//

void PhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 

  ConstructBosons();
  ConstructLeptons();
  ConstructMesons();
  ConstructBaryons();
  ConstructIons();
}

//These call all the constructors for each particles
void PhysicsList::ConstructBosons()
{ 
  // pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();

  // gamma
  G4Gamma::GammaDefinition();
}
void PhysicsList::ConstructLeptons()
{
  // leptons
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();

  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
}

void PhysicsList::ConstructMesons()
{
 //  mesons
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4PionZero::PionZeroDefinition();
  G4Eta::EtaDefinition();
  G4EtaPrime::EtaPrimeDefinition();
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();
  G4KaonZero::KaonZeroDefinition();
  G4AntiKaonZero::AntiKaonZeroDefinition();
  G4KaonZeroLong::KaonZeroLongDefinition();
  G4KaonZeroShort::KaonZeroShortDefinition();
}

void PhysicsList::ConstructBaryons()
{
//  baryons
  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();
  G4Neutron::NeutronDefinition();
  G4AntiNeutron::AntiNeutronDefinition();
}

void PhysicsList::ConstructIons()
{
  //  nuclei
  G4Alpha::AlphaDefinition();
  G4Deuteron::DeuteronDefinition();
  G4Triton::TritonDefinition();
  G4He3::He3Definition();
  //  generic ion
  G4GenericIon::GenericIonDefinition();
}
//----------------------------------------------------------------------------//
// Create all particle transportation processes
//----------------------------------------------------------------------------//
void PhysicsList::ConstructProcess()
{
  AddTransportation();
  ConstructInteractions();
}



//----------------------------------------------------------------------------//
// Define electromagnetic transportation processes
//----------------------------------------------------------------------------//
// gamma
#include "G4PhotoElectricEffect.hh" 
#include "G4ComptonScattering.hh"  
#include "G4PolarizedCompton.hh"
#include "G4GammaConversion.hh" 
// e-
// e+
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
// muon
#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh" 
#include "G4MuBremsstrahlung.hh" 
#include "G4MuPairProduction.hh" 
// neutron
#include "G4HadronElasticProcess.hh"
//#include "G4LElastic.hh"
#include "G4HadronElastic.hh"//new
#include "G4NeutronInelasticProcess.hh"
//#include "G4LENeutronInelastic.hh"
#include "G4HadronCaptureProcess.hh"
#include "G4NeutronHPInelastic.hh"
//#include "G4LCapture.hh"
#include "G4NeutronRadCapture.hh"//new
#include "G4HadronFissionProcess.hh"
#include "G4LFission.hh"

#include "G4NeutronHPThermalScattering.hh"
#include "G4NeutronHPElasticData.hh"
#include "G4NeutronHPThermalScatteringData.hh"
#include "G4NeutronHPElastic.hh"


#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"

//----------------------------------------------------------------------------//
void PhysicsList::ConstructInteractions()
{
  auto theParticleIterator = GetParticleIterator();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
     
    if (particleName == "gamma") {
      // Standard classes
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());
      pmanager->AddDiscreteProcess(new G4ComptonScattering());
      pmanager->AddDiscreteProcess(new G4PolarizedCompton());
      pmanager->AddDiscreteProcess(new G4GammaConversion());
    } else if (particleName == "e-") {
      // Standard classes:
      pmanager->AddProcess(new G4eMultipleScattering(),-1, 1,1);
      pmanager->AddProcess(new G4eIonisation(),       -1, 2,2);
      pmanager->AddProcess(new G4eBremsstrahlung(),   -1,-1,3); 

    } else if (particleName == "e+") {
      // Standard classes:
      pmanager->AddProcess(new G4eMultipleScattering(),-1, 1,1);
      pmanager->AddProcess(new G4eIonisation(),       -1, 2,2);
      pmanager->AddProcess(new G4eBremsstrahlung(),   -1,-1,3);
      pmanager->AddProcess(new G4eplusAnnihilation(),  0,-1,4);
      
    } else if (particleName == "neutron") {
      // elastic scattering
      G4HadronElasticProcess* theElasticProcess =
                                    new G4HadronElasticProcess;
      //G4LElastic* theElasticModel = new G4LElastic;
      //G4HadronElastic* theElasticModel = new G4HadronElastic;
      //theElasticProcess->RegisterMe(theElasticModel);
      //pmanager->AddDiscreteProcess(theElasticProcess);
     

      G4NeutronHPElasticData* theHPElasticData = new G4NeutronHPElasticData;
      theElasticProcess->AddDataSet(theHPElasticData);
      G4NeutronHPThermalScatteringData* theHPThermalScatteringData = new G4NeutronHPThermalScatteringData;
      theElasticProcess->AddDataSet(theHPThermalScatteringData);
      G4NeutronHPElastic* theNeutronElasticModel = new G4NeutronHPElastic;
      theNeutronElasticModel->SetMinEnergy(4.0*CLHEP::eV);
      theElasticProcess->RegisterMe(theNeutronElasticModel);
      G4NeutronHPThermalScattering* theNeutronThermalElasticModel = new G4NeutronHPThermalScattering;
      theNeutronThermalElasticModel->SetMaxEnergy(4.0*CLHEP::eV);
      theElasticProcess->RegisterMe(theNeutronThermalElasticModel);
      
      // inelastic scattering 
      G4NeutronInelasticProcess* theInelasticProcess =
                                 new G4NeutronInelasticProcess("inelastic");
      //G4LENeutronInelastic* theInelasticModel = new G4LENeutronInelastic;
      G4NeutronHPInelastic* theInelasticModel = new G4NeutronHPInelastic;
      theInelasticProcess->RegisterMe(theInelasticModel);
      pmanager->AddDiscreteProcess(theInelasticProcess);
      // capture
      G4HadronCaptureProcess* theCaptureProcess =
                               new G4HadronCaptureProcess;
      G4NeutronRadCapture* theCaptureModel = new G4NeutronRadCapture;
      theCaptureProcess->RegisterMe(theCaptureModel);
      pmanager->AddDiscreteProcess(theCaptureProcess);
      // fission
      G4HadronFissionProcess* theFissionProcess =
                                 new G4HadronFissionProcess;
      G4LFission* theFissionModel = new G4LFission;
      theFissionProcess->RegisterMe(theFissionModel);
      pmanager->AddDiscreteProcess(theFissionProcess);
      } else if( particleName == "mu+" ||
               particleName == "mu-"    ) {
    //muon  
     // Construct processes for muons
     pmanager->AddProcess(new G4MuMultipleScattering(),-1,1,1);
     pmanager->AddProcess(new G4MuIonisation(),-1,2,2);
     pmanager->AddProcess(new G4MuBremsstrahlung(),-1,-1,3);
     pmanager->AddProcess(new G4MuPairProduction(),-1,-1,4);

    } else if( particleName == "GenericIon" ) {
      pmanager->AddProcess(new G4hMultipleScattering(),-1,1,1);
      pmanager->AddProcess(new G4hIonisation(),-1,2,2);
    } else {
      if ((particle->GetPDGCharge() != 0.0) &&
          (particle->GetParticleName() != "chargedgeantino")&&
          (!particle->IsShortLived()) ) {
     // all others charged particles except geantino
       pmanager->AddProcess(new G4hMultipleScattering(),-1,1,1);
       pmanager->AddProcess(new G4hIonisation(),-1,2,2);
      }
    }
  }
}


//----------------------------------------------------------------------------//
void PhysicsList::SetCuts()
{  SetCutsWithDefault();   
   if (verboseLevel > 0) DumpCutValuesTable();  
}
