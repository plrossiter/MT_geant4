#include "PrimaryGeneratorAction.hh"
#include <iomanip>
#include <G4ParticleTable.hh>
#include <G4Event.hh>
#include <G4SystemOfUnits.hh>
#include <G4ParticleGun.hh>
#include <G4GeneralParticleSource.hh>
#include <Randomize.hh>
#include "PrimaryGeneratorMessenger.hh"


using namespace std;

PrimaryGeneratorAction::PrimaryGeneratorAction(const char *inputfile)
{
    
  //All this deals with the CRY input

  particleGun = new G4ParticleGun();

  // Read the cry input file
  std::ifstream inputFile;
  inputFile.open(inputfile,std::ios::in);
  char buffer[1000];


  if (inputFile.fail()) {
    if( *inputfile !=0)  //....only complain if a filename was given
      G4cout << "PrimaryGeneratorAction: Failed to open CRY input file= " << inputfile << G4endl;
    InputState=-1;
  }else{
    std::string setupString("");
    while ( !inputFile.getline(buffer,1000).eof()) {
      setupString.append(buffer);
      setupString.append(" ");
    }
    
    //This is from the cry library, I include this as cry_v1.7.tar.gz
    CRYSetup *setup=new CRYSetup(setupString,"/home/rossiter/MuonTomography_2020/MuonTomographySIMS_210201/gitRepo_Sarah_2102/cry_v1.7/data");

    gen = new CRYGenerator(setup);
  

    // set random number generator
    RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
    setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
    InputState=0;
  }
  // create a vector to store the CRY particle properties
  vect=new std::vector<CRYParticle*>;
  
  // Create the table containing all particle names
  particleTable = G4ParticleTable::GetParticleTable();

  // Create the messenger file
  gunMessenger = new PrimaryGeneratorMessenger(this);


}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{

}

//----------------------------------------------------------------------------//
void PrimaryGeneratorAction::InputCRY()
{
  InputState=1;
}

//----------------------------------------------------------------------------//
void PrimaryGeneratorAction::UpdateCRY(std::string* MessInput)
{
  CRYSetup *setup=new CRYSetup(*MessInput,"/home/rossiter/MuonTomography_2020/MuonTomographySIMS_210201/gitRepo_Sarah_2102/cry_v1.7/data");

  gen = new CRYGenerator(setup);

  // set random number generator
  RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
  setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
  InputState=0;

}

//----------------------------------------------------------------------------//

void PrimaryGeneratorAction::CRYFromFile(G4String newValue)//again CRY stuff, all taken from the CRY library for G4 implementation in cry_v1.7.tar.gz
{
  // Read the cry input file
  std::ifstream inputFile;
  inputFile.open(newValue,std::ios::in);
  char buffer[1000];

  if (inputFile.fail()) {
    G4cout << "Failed to open input file " << newValue << G4endl;
    G4cout << "Make sure to define the cry library on the command line" << G4endl;
    InputState=-1;
  }else{
    std::string setupString("");
    while ( !inputFile.getline(buffer,1000).eof()) {
      setupString.append(buffer);
      setupString.append(" ");
    }

    CRYSetup *setup=new CRYSetup(setupString,"/home/rossiter/MuonTomography_2020/MuonTomographySIMS_210201/gitRepo_Sarah_2102/cry_v1.7/data");

    gen = new CRYGenerator(setup);

  // set random number generator
    RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
    setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
    InputState=0;
  }
}

//----------------------------------------------------------------------------//

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  
  
  if (InputState != 0) {
    G4String* str = new G4String("CRY library was not successfully initialized");
    //G4Exception(*str);
    G4Exception("PrimaryGeneratorAction", "1",
                RunMustBeAborted, *str);
  }
  G4String particleName;
  vect->clear();
  gen->genEvent(vect);
  
  //....debug output
  /*G4cout << "\nEvent=" << anEvent->GetEventID() << " "
         << "CRY generated nparticles=" << vect->size()
         << G4endl;*/
  
  for ( unsigned j=0; j<vect->size(); j++) {
    particleName=CRYUtils::partName((*vect)[j]->id());
    
    //....debug output  
    /*cout << "  "          << particleName << " "
         << "charge="      << (*vect)[j]->charge() << " "
         << setprecision(4)
         << "energy (MeV)=" << (*vect)[j]->ke()*CLHEP::MeV << " "
         << "pos (m)"
         << G4ThreeVector((*vect)[j]->x(), (*vect)[j]->y(), (*vect)[j]->z())
         << " " << "direction cosines "
         << G4ThreeVector((*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w())
         << " " << endl;*/


    //CRY basically works by configuring the particle gun
    
    particleGun->SetParticleDefinition(particleTable->FindParticle((*vect)[j]->PDGid()));
    particleGun->SetParticleEnergy((*vect)[j]->ke()*CLHEP::MeV);

    //particleGun->SetParticlePosition(G4ThreeVector((*vect)[j]->x()*CLHEP::m, (*vect)[j]->y()*CLHEP::m, (*vect)[j]->z()*CLHEP::m));
    particleGun->SetParticlePosition(G4ThreeVector((*vect)[j]->x()*CLHEP::m, (*vect)[j]->y()*CLHEP::m, 3.1*CLHEP::m)); //initial position
    particleGun->SetParticleMomentumDirection(G4ThreeVector((*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w()));
    particleGun->SetParticleTime((*vect)[j]->t());
    particleGun->GeneratePrimaryVertex(anEvent);

    delete (*vect)[j];
  }
}
