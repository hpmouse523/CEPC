#ifndef myDetectorConstruction_h
#define myDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"
#define PI 3.14159265397

class G4Material;
class G4Box;
class G4Trd;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VPVParameterisation;

class myDetectorConstruction : public G4VUserDetectorConstruction
{
private:
  void ConstructMaterials();
  G4Material *Pb;
  G4Material *Si;
  G4Material *Al;
  G4Material *CsI;
  G4Material *Air;
  G4Material *Vacuum;
  G4Material *Casing;
  G4Material *NE102;
  G4Material *LYSO;
  G4Material *BC420;
  G4Material *Tyvek;
  G4Material *Mylar;
  G4Material *Steel;
  G4Material *W;
  G4Material *FR4;
  G4Material *ArCo2;
  G4double Casing_thickness;

  G4Box *solidHallBox;
  G4LogicalVolume *logicHallBox;
  G4VPhysicalVolume *physiHallBox;

  G4Box *solidCell;
  G4LogicalVolume *logicCell;
  G4VPhysicalVolume *physiCell;

  G4Box *solidCal;
  G4LogicalVolume *logicCal;
  G4VPhysicalVolume *physiCal;

  G4VPVParameterisation *crystalParam;
  G4int NbOfCrystals;    // Nb of chambers
  G4double CrystalWidth; // width of the chambers
  G4double CrystalSpacing;

  // G4double Scin_x, Scin_y, Scin_z;
  G4double Tungsten_x, Tungsten_y, Tungsten_z;
  // G4double Tyvek_x, Tyvek_y, Tyvek_z;
  G4double Cell_x, Cell_y, Cell_z;
  G4double Cal_Cell_x, Cal_Cell_y, Cal_Cell_z;

public:
  static int numOfLayers;
  static int numOfCalX;
  static int numOfCalY;
  inline G4LogicalVolume *GetCellLV() const { return logicCell; }
  inline G4LogicalVolume *GetCalorimeterLV() const { return logicCal; }
  inline G4LogicalVolume *GetLayerLV() const { return logicCell; }
  inline G4ThreeVector GetLayerSize() const { return G4ThreeVector(Cell_x, Cell_y, Cell_z * numOfLayers); }

public:
  myDetectorConstruction();
  ~myDetectorConstruction();

public:
  virtual G4VPhysicalVolume *Construct() override;
};
#endif
