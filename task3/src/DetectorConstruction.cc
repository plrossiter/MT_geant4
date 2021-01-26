#include "DetectorConstruction.hh"

#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4VisAttributes.hh>
#include <G4SubtractionSolid.hh>
#include <G4Box.hh>
#include <G4Orb.hh>
#include <G4SDManager.hh>
#include <G4GlobalMagFieldMessenger.hh>
#include <G4MultiFunctionalDetector.hh>
#include <G4PSEnergyDeposit.hh>
#include <EnergyTimeSD.hh>
#include <EnergyDistSD.hh>



using namespace std;

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    G4NistManager* nist = G4NistManager::Instance();
    G4double worldSizeX = 15 * m;
    G4double worldSizeY = 15 * m;
    G4double worldSizeZ = 15 * m;

    G4VSolid* worldBox = new G4Box("world", worldSizeX / 2, worldSizeY / 2, worldSizeZ / 2);

    G4LogicalVolume* worldLog = new G4LogicalVolume(worldBox, nist->FindOrBuildMaterial("G4_AIR"), "world");
    
    G4VisAttributes* visAttr = new G4VisAttributes();
    visAttr->SetVisibility(true);
    worldLog->SetVisAttributes(visAttr);

    G4VPhysicalVolume* worldPhys = new G4PVPlacement(nullptr, {}, worldLog, "world", nullptr, false, 0);


    //here we define materials from their chemical composition, I made bananas and cocaine as these are some items we might eventually want to discriminate between

    G4Element* elH = new G4Element("Hydrogen", "H", 1., 1.0079 * g/mole);
    G4Element* elC = new G4Element("Carbon", "C", 6., 12.01 * g/mole);
    G4Element* elO = new G4Element("Oxygen", "O", 8., 15.99 * g/mole);
    G4Element* elN = new G4Element("Nitrogen", "N", 7., 14.007 * g/mole);

    G4Material* starch = new G4Material("Starch", 1.5 * g/cm3, 3); 

    //lets make a banana    

    starch->AddElement(elH, 10);
    starch->AddElement(elC, 6);
    starch->AddElement(elO, 5);

    G4Material* banana = new G4Material("Banana", 1 * g/cm3, 2); 

    banana->AddMaterial(nist->FindOrBuildMaterial("G4_WATER"), 75.*perCent);
    banana->AddMaterial(starch, 25.*perCent);

    //lets make cocaine

    G4Material* cocaine = new G4Material("Cocaine", 0.61 * g/cm3, 4); 
    cocaine->AddElement(elC, 17);
    cocaine->AddElement(elH, 12);
    cocaine->AddElement(elN, 1);
    cocaine->AddElement(elO, 4);


    G4double thickness = 0.1 * mm;
    G4double widthy = 7 * m; 
    G4double widthx = 4 * m; 

    //here also define detectors in the x and z directions, now have detectors surrounding the container

    G4VSolid* detectorBox_z = new G4Box("detectorBox_z", widthx / 2 , widthy / 2, thickness / 2);
    G4VSolid* detectorBox_x = new G4Box("detectorBox_x", thickness / 2, widthy / 2, widthx / 2);

    //this is creating the actual shipping container volume
    G4VSolid *b1 = new G4Box("b1",2.44*m/2, 6.1*m/2, 2.59*m/2);
    G4VSolid *b2 = new G4Box("b2", 2.436*m/2, 6.096*m/2, 2.586*m/2);
    G4VSolid* shipping_containerBox= new G4SubtractionSolid("b1_minus_b2", b1, b2);


    G4VSolid* blockBox_water = new G4Box("blockBox_water", 25*cm , 25*cm, 25*cm);
    G4VSolid* blockBox_lead = new G4Box("blockBox_lead", 25*cm , 25*cm, 25*cm);
    
    G4VSolid* blockBox_concrete = new G4Box("blockBox_concrete", 25*cm , 25*cm, 25*cm);

    G4VSolid* blockBox_banana = new G4Box("blockBox_banana", 2.5*cm , 2.5*cm, 7.5*cm);
    G4VSolid* blockBox_cocaine = new G4Box("blockBox_cocaine", 25*cm , 25*cm, 25*cm);

    G4VSolid* scorerBox = new G4Box("scorerBox",  worldSizeX / 2, worldSizeY / 2, thickness / 2);


    detectorLog_z = new G4LogicalVolume(detectorBox_z, nist->FindOrBuildMaterial("G4_Galactic"), "detector_z");
    detectorLog_x = new G4LogicalVolume(detectorBox_x, nist->FindOrBuildMaterial("G4_Galactic"), "detector_x");

    containerLog = new G4LogicalVolume(shipping_containerBox,nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "container");

    blockLog_lead = new G4LogicalVolume(blockBox_lead, nist->FindOrBuildMaterial("G4_Pb"), "block_lead");    
    blockLog_water = new G4LogicalVolume(blockBox_water, nist->FindOrBuildMaterial("G4_WATER"), "block_water");    
    blockLog_concrete = new G4LogicalVolume(blockBox_concrete, nist->FindOrBuildMaterial("G4_CONCRETE"), "block_concrete");    

    blockLog_banana = new G4LogicalVolume(blockBox_banana, banana, "block_banana");    
    blockLog_cocaine = new G4LogicalVolume(blockBox_cocaine, cocaine, "block_cocaine");    

    scorerLog = new G4LogicalVolume(scorerBox, nist->FindOrBuildMaterial("G4_Galactic"), "scorer");    



    //set visible attributes
    G4VisAttributes* red = new G4VisAttributes(G4Colour::Red());
    red->SetVisibility(true);
    detectorLog_z->SetVisAttributes(red);

    G4VisAttributes* magenta = new G4VisAttributes(G4Colour::Magenta());
    magenta->SetVisibility(true);
    detectorLog_x->SetVisAttributes(magenta);

    G4VisAttributes* grey = new G4VisAttributes(G4Colour::Grey());
    grey->SetVisibility(true);
    blockLog_lead->SetVisAttributes(grey);
    blockLog_cocaine->SetVisAttributes(grey);
    containerLog->SetVisAttributes(grey);

    G4VisAttributes* blue = new G4VisAttributes(G4Colour::Blue());
    blue->SetVisibility(true);
    blockLog_water->SetVisAttributes(blue);

    G4VisAttributes* yellow = new G4VisAttributes(G4Colour::Yellow());
    yellow->SetVisibility(true);
    blockLog_concrete->SetVisAttributes(yellow);
    blockLog_banana->SetVisAttributes(yellow);

    G4VisAttributes* green = new G4VisAttributes(G4Colour::Green());
    green->SetVisibility(true);
    scorerLog->SetVisAttributes(green);


    // We have already provided the positions, you finish the rest...

    G4int numberOfZDetectorLayers = 10;
    G4int numberOfXDetectorLayers = 10;
    G4int numberOfBlocks = 1;

    vector<G4ThreeVector> detectorPositions_z;
    vector<G4ThreeVector> detectorPositions_x;
    vector<G4ThreeVector> containerPositions;
    vector<G4ThreeVector> bananaPositions;
    vector<G4ThreeVector> cocainePositions;
    vector<G4ThreeVector> waterblockPositions;
    vector<G4ThreeVector> leadblockPositions;
    vector<G4ThreeVector> concreteblockPositions;
 
    G4ThreeVector scorerPosition = {0, 0, 3.05*m};
    
    /////positions of detector in z direction
    detectorPositions_z.push_back({0, 0, 3.00*m});
    detectorPositions_z.push_back({0, 0, 2.75*m});
    detectorPositions_z.push_back({0, 0, 2.50*m});
    detectorPositions_z.push_back({0, 0, 2.25*m});
    detectorPositions_z.push_back({0, 0, 2.00*m});
    detectorPositions_z.push_back({0, 0, -2.00*m});
    detectorPositions_z.push_back({0, 0, -2.25*m});
    detectorPositions_z.push_back({0, 0, -2.50*m});
    detectorPositions_z.push_back({0, 0, -2.75*m});
    detectorPositions_z.push_back({0, 0, -3.00*m});

     /////positions of detector in x direction
    detectorPositions_x.push_back({3.00*m, 0, 0});
    detectorPositions_x.push_back({2.75*m, 0, 0});
    detectorPositions_x.push_back({2.50*m, 0, 0});
    detectorPositions_x.push_back({2.25*m, 0, 0});
    detectorPositions_x.push_back({2.00*m, 0, 0});
    detectorPositions_x.push_back({-2.00*m, 0, 0});
    detectorPositions_x.push_back({-2.25*m, 0, 0});
    detectorPositions_x.push_back({-2.50*m, 0, 0});
    detectorPositions_x.push_back({-2.75*m, 0, 0});
    detectorPositions_x.push_back({-3.00*m, 0, 0});

    ///location of container
    containerPositions.push_back({0, 0, 0});
    cocainePositions.push_back({0, 0, 0});

    /////positions of blocks
    leadblockPositions.push_back({0, 0, 0});
    waterblockPositions.push_back({0, 1.5*m, 0});
    concreteblockPositions.push_back({0, -1.5*m, 0});


    //define positioning for different things 
    double banana_y_up = 2.85;
    double banana_y_down = -2.95;

    double banana_x_up = 1.22;
    double banana_x_down = -1.22;

    double banana_z_up = 1.2;
    double banana_z_down = -1.2;

    double amount_x = 0;
    double amount_y = 0;    
    double amount_z = 0;
    int numberOfBanana = 0;
    int numberOfBanana_x = 0;
    int numberOfBanana_y = 0;
    int numberOfBanana_z = 0;


    //this is for filling the whole container with bananas - not really needed for most applications was just for a quick test
    while (banana_y_down < banana_y_up)
      {
	if (numberOfBanana_y % 5 == 0 && numberOfBanana_y != 0) amount_y = 0.5;
	else amount_y = 0.055;
	banana_y_down += amount_y;
	numberOfBanana_y++;

	while (banana_x_down < banana_x_up)
	  {
	    if (numberOfBanana_x % 5 == 0 && numberOfBanana_x != 0) amount_x = 0.5;
	    else amount_x = 0.055;
	    banana_x_down += amount_x;
	    numberOfBanana_x++;

	    while (banana_z_down < banana_z_up)
	      {
		if (numberOfBanana_z % 5 == 0 && numberOfBanana_z != 0) amount_z = 0.5;
		else amount_z = 0.055;
		banana_z_down += amount_z;
		numberOfBanana_z++;
	       
		bananaPositions.push_back({banana_x_down*m, banana_y_down*m, banana_z_down*m});

	      }

	    banana_z_down = -1.2;
	
	  }

	banana_x_down = -1.22;
	
      }



    numberOfBanana = numberOfBanana_x + numberOfBanana_y + numberOfBanana_z;

     for (int i = 0; i < numberOfZDetectorLayers; i++)
    {
        ostringstream aName; aName << "detector_z" << i;
	new G4PVPlacement(nullptr, detectorPositions_z[i], 
                          detectorLog_z, aName.str(), worldLog, 0, i);
    }

      for (int i = 0; i < numberOfXDetectorLayers; i++)
      {
        ostringstream aName; aName << "detector_x" << i;
	new G4PVPlacement(nullptr, detectorPositions_x[i], 
                          detectorLog_x, aName.str(), worldLog, 0, i);
			  }

     for (int i = 0; i < numberOfBlocks; i++){
      ostringstream aName; aName << "block_water" << i;
      new G4PVPlacement(nullptr, waterblockPositions[i], 
			blockLog_water, aName.str(), worldLog, 0, i);
			}

    for (int i = 0; i < numberOfBlocks; i++){
      ostringstream aName; aName << "block_lead" << i;
      new G4PVPlacement(nullptr, leadblockPositions[i], 
			blockLog_lead, aName.str(), worldLog, 0, i);
    }
    
    
    for (int i = 0; i < numberOfBlocks; i++){
      ostringstream aName; aName << "block_concrete" << i;
      new G4PVPlacement(nullptr, concreteblockPositions[i], 
			blockLog_concrete, aName.str(), worldLog, 0, i);
			}


    //locate the container    
     for (int i = 0; i < numberOfBlocks; i++){
      ostringstream aName; aName << "container" << i;
      new G4PVPlacement(nullptr, containerPositions[i], 
			containerLog, aName.str(), worldLog, 0, i);
			}

     /*for (int i = 0; i < numberOfBlocks; i++){
      ostringstream aName; aName << "cocaine" << i;
      new G4PVPlacement(nullptr, cocainePositions[i], 
			blockLog_cocaine, aName.str(), worldLog, 0, i);
			}

    for (int i = 0; i < numberOfBanana; i++){
      ostringstream aName; aName << "banana" << i;


      if( bananaPositions[i].x() == 0 && bananaPositions[i].y() == 0 && bananaPositions[i].z() == 0) continue;
      new G4PVPlacement(nullptr, bananaPositions[i], 
      		blockLog_banana, aName.str(), worldLog, 0, i);
		}*/

    new G4PVPlacement(nullptr, scorerPosition, scorerLog, "scorer", worldLog, 0, 0);

    return worldPhys;
}


void DetectorConstruction::ConstructSDandField()
{

    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    sdManager->SetVerboseLevel(2);  

    EnergyTimeSD* detectorET_z = new EnergyTimeSD("detectorET_z");
    sdManager->AddNewDetector(detectorET_z);
    SetSensitiveDetector("detector_z", detectorET_z);

    EnergyTimeSD* detectorET_x = new EnergyTimeSD("detectorET_x");
    sdManager->AddNewDetector(detectorET_x);
    SetSensitiveDetector("detector_x", detectorET_x);

    EnergyDistSD* scorer_ED = new EnergyDistSD("scorer_ED");
    sdManager->AddNewDetector(scorer_ED);
    SetSensitiveDetector("scorer", scorer_ED);

}


