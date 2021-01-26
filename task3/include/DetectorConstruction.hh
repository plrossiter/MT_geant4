#ifndef DETECTOR_CONSTRUCTION_HH
#define DETECTOR_CONSTRUCTION_HH

#include <G4VUserDetectorConstruction.hh>

class G4LogicalVolume;


class DetectorConstruction : public G4VUserDetectorConstruction
{
public:

  G4LogicalVolume* detectorLog_x;
  G4LogicalVolume* detectorLog_z;
  G4LogicalVolume* containerLog;

  G4LogicalVolume* blockLog_lead;
  G4LogicalVolume* blockLog_water;
  G4LogicalVolume* blockLog_concrete;

  G4LogicalVolume* scorerLog;

  G4LogicalVolume* blockLog_banana;
  G4LogicalVolume* blockLog_cocaine;
  
  G4VPhysicalVolume* Construct() override;
  
  void ConstructSDandField() override;

private:

};

#endif
