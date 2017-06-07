#include "myDetectorConstruction.hh"
// #include "myCsIdetectorSD.hh"

#include "G4UnitsTable.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4Isotope.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4PVReplica.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4Polyline.hh"
#include "G4Point3D.hh"
#include "G4VVisManager.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <fstream>
using std::ifstream;

static ifstream fDetectorParameter("DetectorParameter.txt");

myDetectorConstruction::myDetectorConstruction()
{

    if (!fDetectorParameter.is_open())
    {
        G4cout << "Can't find DetectorParameter.txt" << G4endl;
        exit(1);
    }

    double temp_double;
    char temp_char[1200];
    std::streampos fpos;
    fpos = fDetectorParameter.tellg();
    fDetectorParameter.getline(temp_char, 1200);

    while (!fDetectorParameter.eof() && (temp_char[0] == '#' || temp_char[0] == '\0'))
    {
        fpos = fDetectorParameter.tellg();
        fDetectorParameter.getline(temp_char, 1200);
    }
    if (fDetectorParameter.eof())
    {
        G4cout << "文件参数错误，参数(单位均为cm)顺序应该为:" << '\n'
               << "铅板的长(x)、宽(y)、厚(z)" << '\n'
               << "单个量能器的长(x)、宽(y)、厚(z)" << '\n'
               << "每层中量能器的沿X轴数量numX 、每层中量能器的沿Y轴数量numX" << '\n'
               << "层数" << G4endl;
    }
    fDetectorParameter.seekg(fpos);
    fDetectorParameter >> temp_double;
    Tungsten_x = temp_double * cm;
    fDetectorParameter >> temp_double;
    Tungsten_y = temp_double * cm;
    fDetectorParameter >> temp_double;
    Tungsten_z = temp_double * cm;

    fpos = fDetectorParameter.tellg();
    fDetectorParameter.getline(temp_char, 1200);
    while (!fDetectorParameter.eof() && (temp_char[0] == '#' || temp_char[0] == '\0'))
    {
        fpos = fDetectorParameter.tellg();
        fDetectorParameter.getline(temp_char, 1200);
    }
    if (fDetectorParameter.eof())
    {
        G4cout << "文件参数错误，参数(单位均为cm)顺序应该为:" << '\n'
               << "铅板的长(x)、宽(y)、厚(z)" << '\n'
               << "单个量能器的长(x)、宽(y)、厚(z)" << '\n'
               << "每层中量能器的沿X轴数量numX 、每层中量能器的沿Y轴数量numX" << '\n'
               << "层数" << G4endl;
    }

    fDetectorParameter.seekg(fpos);
    fDetectorParameter >> temp_double;
    Cal_Cell_x = temp_double * cm;
    fDetectorParameter >> temp_double;
    Cal_Cell_y = temp_double * cm;
    fDetectorParameter >> temp_double;
    Cal_Cell_z = temp_double * cm;

    fpos = fDetectorParameter.tellg();
    fDetectorParameter.getline(temp_char, 1200);
    while (!fDetectorParameter.eof() && (temp_char[0] == '#' || temp_char[0] == '\0'))
    {
        fpos = fDetectorParameter.tellg();
        fDetectorParameter.getline(temp_char, 1200);
    }
    if (fDetectorParameter.eof())
    {
        G4cout << "文件参数错误，参数(单位均为cm)顺序应该为:" << '\n'
               << "铅板的长(x)、宽(y)、厚(z)" << '\n'
               << "单个量能器的长(x)、宽(y)、厚(z)" << '\n'
               << "每层中量能器的沿X轴数量numX 、每层中量能器的沿Y轴数量numX" << '\n'
               << "层数" << G4endl;
    }

    fDetectorParameter.seekg(fpos);
    fDetectorParameter >> temp_double;
    numOfCalX = int(temp_double);
    fDetectorParameter >> temp_double;
    numOfCalY = int(temp_double);

    fpos = fDetectorParameter.tellg();
    fDetectorParameter.getline(temp_char, 1200);
    while (!fDetectorParameter.eof() && (temp_char[0] == '#' || temp_char[0] == '\0'))
    {
        fpos = fDetectorParameter.tellg();
        fDetectorParameter.getline(temp_char, 1200);
    }
    fDetectorParameter.seekg(fpos);
    fDetectorParameter >> temp_double;
    numOfLayers = int(temp_double);

    // //铅板的几何大小
    // Tungsten_x = 100.0 * cm;
    // Tungsten_y = 100.0 * cm;
    // Tungsten_z = 0.3 * cm;
    // //单个量能器的几何大小
    // Cal_Cell_x = 1.0 * cm;
    // Cal_Cell_y = 1.0 * cm;
    // Cal_Cell_z = 1.0 * cm;

    Cell_x = Tungsten_x;
    Cell_y = Tungsten_y;
    Cell_z = Tungsten_z + Cal_Cell_z; //Total Cell Thickness

    // G4cout << Tungsten_x << '\t' << Tungsten_y << '\t' << Tungsten_z << G4endl;
    // G4cout << Cal_Cell_x << '\t' << Cal_Cell_y << '\t' << Cal_Cell_z << G4endl;
    // G4cout << numOfCalX << '\t' << numOfCalY << '\t' << numOfLayers << G4endl;
}

//Define the number of layers
int myDetectorConstruction::numOfLayers = 30;
int myDetectorConstruction::numOfCalX = 100; //必须等于Tungsten_x/Cal_Cell_x
int myDetectorConstruction::numOfCalY = 100; //必须等于Tungsten_y/Cal_Cell_z

myDetectorConstruction::~myDetectorConstruction()
{
}
void myDetectorConstruction::ConstructMaterials()
{

    G4String name, symbol;  //a=mass of a mole;
    G4double a, z, density; //z=mean number of protons;
    G4int iz, n, nel;       //iz=number of protons  in an isotope;
    G4int ncomponents, natoms;
    G4double fractionmass;
    G4double abundance, temperature, chamberPressure;
    G4Element *elH = new G4Element("Hydrogen", "H", z = 1, a = 1.0079 * g / mole);
    G4Element *elLi = new G4Element("Lithium", "Li", z = 3, a = 6.941 * g / mole);
    G4Element *elC = new G4Element("Carbon", "C", z = 6, a = 12.0107 * g / mole);
    G4Element *elN = new G4Element("Nitrogen", "N", z = 7, a = 14.0067 * g / mole);
    G4Element *elO = new G4Element("Oxygen", "O", z = 8, a = 15.9994 * g / mole);
    G4Element *elF = new G4Element("Fluorine", "F", z = 9, a = 18.9984 * g / mole);
    G4Element *elNa = new G4Element("Sodium", "Na", z = 11, a = 22.9898 * g / mole);
    G4Element *elMg = new G4Element("Mg", "Mg", z = 12, a = 24.3050 * g / mole);
    G4Element *elAl = new G4Element("Al", "Al", z = 13, a = 26.9815 * g / mole);
    G4Element *elSi = new G4Element("Silicon", "Si", z = 14, a = 28.0855 * g / mole);
    G4Element *elP = new G4Element("Phosphorus", "P", z = 15, a = 30.9738 * g / mole);
    G4Element *elS = new G4Element("S", "S", z = 16, a = 32.066 * g / mole);
    G4Element *elK = new G4Element("Potassium", "K", z = 19, a = 39.0983 * g / mole);
    G4Element *elCa = new G4Element("Calcium", "Ca", z = 20, a = 40.078 * g / mole);
    G4Element *elCr = new G4Element("Cr", "Cr", z = 24, a = 51.9961 * g / mole);
    G4Element *elMn = new G4Element("Mn", "Mn", z = 25, a = 54.9381 * g / mole);
    G4Element *elFe = new G4Element("Fe", "Fe", z = 26, a = 55.847 * g / mole);
    G4Element *elNi = new G4Element("Ni", "Ni", z = 28, a = 58.6934 * g / mole);
    G4Element *elI = new G4Element("Iodine", "I", z = 53., a = 126.90447 * g / mole);
    G4Element *elCs = new G4Element("Cesium", "Cs", z = 55., a = 132.90543 * g / mole);
    G4Element *elLu = new G4Element("Lutetium", "Lu", z = 71., a = 174.967 * g / mole);
    G4Element *elY = new G4Element("Yttrium", "Y", z = 39., a = 88.90585 * g / mole);

    //------------Air--------------------------------------------------------
    Air = new G4Material("Air", 1.29 * mg / cm3, 2, kStateUndefined, 273.15 * kelvin, 1.0 * atmosphere);
    Air->AddElement(elN, 7);
    Air->AddElement(elO, 3);
    //----------------------------------------------------------------------

    //Steel
    Steel = new G4Material("Steel", density = 8.1 * g / cm3, nel = 8);
    Steel->AddElement(elFe, 0.70845);
    Steel->AddElement(elC, 0.0008);
    Steel->AddElement(elMn, 0.02);
    Steel->AddElement(elP, 0.00045);
    Steel->AddElement(elS, 0.00030);
    Steel->AddElement(elSi, 0.01);
    Steel->AddElement(elCr, 0.18);
    Steel->AddElement(elNi, 0.08);

    //-----Pb---------------------------------------------------------------
    Pb = new G4Material(name = "Lead", z = 82., a = 207.2 * g / mole, density = 11.34 * g / cm3);
    //----------------------------------------------------------------------

    //----SI - SILICON--------------------------------------------
    Si = new G4Material(name = "Silicon", z = 14., a = 28.0855 * g / mole, density = 2.33 * g / cm3);
    //------------------------------------------------------------
    //---------------NE102------------------------------------------
    density = 1.06 * g / cm3;
    NE102 = new G4Material(name = "Scintillator", density, 2);
    NE102->AddElement(elC, 2);
    NE102->AddElement(elH, 3);
    //------------------------------------------------------------

    //------------------------------Tungsten----------------------------
    W = new G4Material(name = "Tungsten", z = 74., a = 183.84 * g / mole, density = 19.35 * g / cm3);

    //--------------------------------CsI-----------------------------
    CsI = new G4Material(name = "CsI", density = 4.53 * g / cm3, 2);
    CsI->AddElement(elCs, 1);
    CsI->AddElement(elI, 1);
    //--------------------------------------------------------------------

    //--------------------------------LYSO----------------------------------
    // LYSO = new G4Material(name="LYSO", density=7.4*g/cm3, 4);
    // LYSO->AddElement(elLu,1.6);
    // LYSO->AddElement(elY,0.4);
    // LYSO->AddElement(elSi,1);
    // LYSO->AddElement(elO,5);

    //------------------- Plastic Scintillator BC420-------------------------
    density = 1.032 * g / cm3;
    BC420 = new G4Material("BC420", density, 2);
    BC420->AddElement(elC, 10);
    BC420->AddElement(elH, 11);
    //----------------------------------------------------------------------

    //------------------CO2 Gas---------------------
    G4double CO2density = 1.784e-3 * g / cm3;
    G4Material *CO2 = new G4Material("CO2", CO2density, 2);
    CO2->AddElement(elC, 1);
    CO2->AddElement(elO, 2);
    //---------------------------------------------------

    //---------------------Ar----------------------------
    G4double Ardensity = 3.799e-3 * g / cm3;
    G4Material *Ar = new G4Material(name = "Ar", z = 18., a = 39.93 * g / mole, Ardensity);
    //---------------------------------------------------

    //-----------------------Xe----------------------------------
    G4double Xedensity = 5.89e-3 * g / cm3;
    G4Material *Xe = new G4Material(name = "Xe", z = 54., a = 131.3 * g / mole, Xedensity);

    //----------------THGEM Gas--------------------------------
    G4double ArCo2density = 1.799e-3 * g / cm3;
    ArCo2 = new G4Material(name = "ArCo2", ArCo2density, 2);
    ArCo2->AddMaterial(Ar, 90 * perCent);
    ArCo2->AddMaterial(CO2, 10 * perCent);

    //
    //from http://www.physi.uni-heidelberg.de/~adler/TRD/TRDunterlagen/RadiatonLength/tgc2.htm
    //Epoxy (for FR4 )
    G4Material *SiO2 =
        new G4Material("quartz", density = 2.200 * g / cm3, ncomponents = 2);
    SiO2->AddElement(elSi, natoms = 1);
    SiO2->AddElement(elO, natoms = 2);

    density = 1.2 * g / cm3;
    G4Material *Epoxy = new G4Material("Epoxy", density, ncomponents = 2);
    Epoxy->AddElement(elH, natoms = 2);
    Epoxy->AddElement(elC, natoms = 2);

    //FR4 (Glass + Epoxy)
    density = 1.86 * g / cm3;
    FR4 = new G4Material("FR4", density, ncomponents = 2);
    FR4->AddMaterial(SiO2, fractionmass = 0.528);
    FR4->AddMaterial(Epoxy, fractionmass = 0.472);

    //---------Al---------------------------------------------------------
    Al = new G4Material(name = "Aluminium", z = 13., a = 26.98 * g / mole, density = 2.700 * g / cm3);
    //--------------------------------------------------------------------

    //----------Casing Material--------------------------------------------
    density = 0.344 * g / cm3;
    Tyvek = new G4Material(name = "Polyethylene", density, ncomponents = 2);
    Tyvek->AddElement(elC, natoms = 1);
    Tyvek->AddElement(elH, natoms = 2);

    density = 1.39 * g / cm3;
    Mylar = new G4Material(name = "PolyethyleneTerephthlate", density, ncomponents = 3);
    Mylar->AddElement(elC, natoms = 5);
    Mylar->AddElement(elH, natoms = 4);
    Mylar->AddElement(elO, natoms = 2);
}

G4VPhysicalVolume *myDetectorConstruction::Construct()
{
    //--------------------------------
    ConstructMaterials();

    G4int LayerNum, CellNum;

    LayerNum = numOfLayers;
    CellNum = numOfCalX * numOfCalY;
    if (int(Tungsten_x / Cal_Cell_x) != numOfCalX)
    {
        G4cout << "calculated number of Cal along X is not equal to setting!" << G4endl;
        exit(1);
    }
    if (int(Tungsten_y / Cal_Cell_y) != numOfCalY)
    {
        G4cout << "calculated number of Cal along Y is not equal to setting!" << G4endl;
        exit(1);
    }

    // Visualization attributes
    G4VisAttributes *lightblueColor = new G4VisAttributes(G4Colour(175 / 255., 255 / 255., 255 / 255.));
    G4VisAttributes *skyblueColor = new G4VisAttributes(G4Colour(175 / 255., 164 / 255., 255 / 255.));
    G4VisAttributes *redColor = new G4VisAttributes(G4Colour(255 / 255., 0 / 255., 12 / 255.));
    G4VisAttributes *violetColor = new G4VisAttributes(G4Colour(224 / 255., 0 / 255., 255 / 255.));
    G4VisAttributes *indigoColor = new G4VisAttributes(G4Colour(0 / 255., 0 / 255., 190 / 255.));
    G4VisAttributes *grassColor = new G4VisAttributes(G4Colour(0 / 255., 239 / 255., 0 / 255.));
    G4VisAttributes *orangeColor = new G4VisAttributes(G4Colour(241 / 255., 224 / 255., 0 / 255.));
    G4VisAttributes *purpleColor = new G4VisAttributes(G4Colour(187 / 255., 28 / 255., 0 / 255.));
    G4VisAttributes *brownColor = new G4VisAttributes(G4Colour(225 / 255., 126 / 255., 66 / 255.));
    G4VisAttributes *grayColor = new G4VisAttributes(G4Colour(210 / 255., 213 / 255., 210 / 255.));

    G4VisAttributes *yellow = new G4VisAttributes(G4Colour(255 / 255., 255 / 255., 0 / 255.));

    //--------------------HallBox-----------------------------------------
    solidHallBox = new G4Box("HallBox", Cell_x, Cell_y, Cell_z * 60);
    logicHallBox = new G4LogicalVolume(solidHallBox, //its solid
                                       Air,          //its material
                                       "HallBox",    //its name
                                       0, 0, 0);
    logicHallBox->SetVisAttributes(orangeColor);
    physiHallBox = new G4PVPlacement(0, //rotation
                                     G4ThreeVector(0.0 * cm, 0.0 * cm, 0.0 * cm),
                                     logicHallBox, //its logical volume
                                     "HallBox",    //its name  (2nd constructor)
                                     NULL,         //its mother volume
                                     false,        //no boolean operation
                                     0);           //copy number

    //---------------------Detector Cell----------------------------------
    // Cal include in Cell(Steel)

    solidCell = new G4Box("Cell", Cell_x / 2, Cell_y / 2, Cell_z / 2);
    logicCell = new G4LogicalVolume(solidCell, //its solid
                                    W,        // Steel,              //its material
                                    "Cell",    //its name
                                    0, 0, 0);
    logicCell->SetVisAttributes(redColor);
    //------------------------------------------------------------------

    //-------------------------------------------------------------
    solidCal = new G4Box("Caldetector", Cal_Cell_x / 2, Cal_Cell_y / 2, Cal_Cell_z / 2);
    logicCal = new G4LogicalVolume(solidCal,      //its solid
                                   Si,           //its material
                                   "Caldetector", //its name
                                   0, 0, 0);
    // logicCell->SetVisAttributes(redColor);
    logicCal->SetVisAttributes(G4VisAttributes::Invisible);
    //------------------------------------------------------------


    //-------------------------------------------------------------
    //    solidFR4= new G4Box("Caldetector",FR4_Cell_x/2,FR4_Cell_y/2,FR4_Cell_z/2);
    //    logicFR4 = new G4LogicalVolume(solidFR4,//its solid
    //             FR4,             //its material
    //             "FR4_board" ,          //its name
    //             0,0,0);
    //    logicFR4->SetVisAttributes(skyblueColor);
    //------------------------------------------------------------

    //-------------------------------------------------------------
    //    solidInduce = new G4Box("Induce_gap",Induce_Cell_x/2,Induce_Cell_y/2,Induce_Cell_z/2);
    //    logicInduce = new G4LogicalVolume(solidInduce,//its solid
    //             ArCo2,              //its material
    //             "induce_gap" ,          //its name
    //             0,0,0);
    //    logicInduce->SetVisAttributes(brownColor);
    //------------------------------------------------------------

    //-----------------------Readout----------------------------------
    //    solidReadout = new G4Box("CalReadout",Readout_Cell_x/2,Readout_Cell_y/2,Readout_Cell_z/2);
    //    logicReadout = new G4LogicalVolume(solidReadout,//its solid
    //             FR4,              //its material
    //             "Readout_board" ,          //its name
    //             0,0,0);
    //    logicInduce->SetVisAttributes(indigo);
    //------------------------------------------------------------

    //----------------------------------Cell layer----------------------
    for (int j = 0; j < CellNum; j++)
    {
        G4double Calx, Caly, Calz;
        Calx = -Cell_x / 2. + Cal_Cell_x / 2. + (j % numOfCalX) * Cal_Cell_x;
        Caly = -Cell_y / 2. + Cal_Cell_y / 2. + (j / numOfCalX) * Cal_Cell_y;
        Calz = -Cal_Cell_z / 2 + Cell_z / 2;
        physiCal = new G4PVPlacement(0, G4ThreeVector(Calx, Caly, Calz),
                                     logicCal,  //its logical volume
                                     "Cal",     //its name  (2nd constructor)
                                     logicCell, //its mother volume
                                     false,     //no boolean operation
                                     j);
    }

    //---------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /*   G4double FR4x,FR4y,FR4z;
       FR4x=0.*cm;
       FR4y=0.*cm;
       FR4z=FR4_Cell_z/2+Cal_Cell_z-Cell_z/2;
       physiFR4 = new G4PVPlacement(0,G4ThreeVector(FR4x,FR4y,FR4z),
                    logicFR4,         //its logical volume 
                    "FR4-board",   //its name  (2nd constructor)
                    logicCell,              //its mother volume 
                    false,                 //no boolean operation 
                    0);
       //-------------------------------------------------------------------

   //--------------------------------------------------------------------------
       G4double Inducex,Inducey,Inducez;
       Inducex=0.*cm;
       Inducey=0.*cm;
       Inducez=Induce_Cell_z/2+FR4_Cell_z+Cal_Cell_z-Cell_z/2;
       physiInduce = new G4PVPlacement(0,G4ThreeVector(Inducex,Inducey,Inducez),
                    logicInduce,         //its logical volume 
                    "Induce-gap",   //its name  (2nd constructor)
                    logicCell,              //its mother volume 
                    false,                 //no boolean operation 
                    0);
       //-------------------------------------------------------------------

   //--------------------------------------------------------------------------
       G4double Readoutx,Readouty,Readoutz;
 
     Readoutx=0.*cm;
       Readouty=0.*cm;
       Readoutz=Readout_Cell_z/2+Induce_Cell_z+FR4_Cell_z+Cal_Cell_z-Cell_z/2;
       physiReadout = new G4PVPlacement(0,G4ThreeVector(Readoutx,Readouty,Readoutz),
                    logicReadout,         //its logical volume 
                    "Readout-gap",   //its name  (2nd constructor)
                    logicCell,              //its mother volume 
                    false,                 //no boolean operation 
                    0);
       //-------------------------------------------------------------------

*/
    G4int N1;
    N1 = LayerNum;
    // N1=1;
    for (int i = 0; i < N1; i++)
    {

        G4double x, y, z;
        x = 0. * cm;
        y = 0. * cm;
        z = Cell_z / 2 + i * Cell_z;

        physiCell = new G4PVPlacement(0, G4ThreeVector(x, y, z),
                                      logicCell,    //its logical volume
                                      "Cell",       //its name  (2nd constructor)
                                      logicHallBox, //its mother volume
                                      false,        //no boolean operation
                                      i);
    }

    //------------------------------------------------
    // Sensitive detectors
    //------------------------------------------------

    // G4SDManager *SDman = G4SDManager::GetSDMpointer();
    // G4String CsISDname;
    // myCsIdetectorSD *CsISD = new myCsIdetectorSD(CsISDname = "/MyCsIDetector");
    // SDman->AddNewDetector(CsISD);
    // logicCal->SetSensitiveDetector(CsISD);

    return physiHallBox;
}
