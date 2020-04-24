/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class


#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(),
 fAbsorMaterial(0), fLAbsor(0), fContainMaterial(0),
 fLContain(0),fCylinderThickness(0), fCylinderMaterial(0), fLCylinder(0),
 fWorldMaterial(0), fPWorld(0),
 fDetectorMessenger(0)
{
  fAbsorRadius = 40*mm;
  fAbsorLength = 1.*mm;
  fContainThickness = 0.*cm;
  //fContainThickness = 0.025*cm;
  fCylinderThickness = 0.0001*mm;
  DefineMaterials();
  //SetAbsorMaterial  ("BeO");
  //SetAbsorMaterial  ("Be2");
  //SetAbsorMaterial  ("G4_Be");
  SetAbsorMaterial  ("G4_GRAPHITE");
  //SetAbsorMaterial  ("G4_Li");
  //SetAbsorMaterial  ("G4_Al");
  //SetAbsorMaterial  ("AlMg3");
  //SetAbsorMaterial  ("Li2");
  //SetAbsorMaterial  ("D2");
  //SetAbsorMaterial("Vacuum");
  //SetAbsorMaterial("Stainless-Steel");
  SetContainMaterial("Vacuum");
  //SetCylinderMaterial("Stainless-Steel");
  SetCylinderMaterial("Vacuum");

  fDetectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ delete fDetectorMessenger;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
  G4NistManager* nist = G4NistManager::Instance();
  G4int ncomponents, natoms;
  G4Element* D = new G4Element("Deuterium" ,"D"  ,1.  ,  2.01*g/mole);
  G4Element* Li = new G4Element("Lithium"  ,"Li" ,  3.,  6.94*g/mole);
  G4Element* Be = new G4Element("Beryllium","Be" ,  4.,  9.01*g/mole);
  G4Element* N  = new G4Element("Nitrogen" ,"N"  ,  7., 14.0067*g/mole);
  G4Element* O  = new G4Element("Oxygen"   ,"O"  ,  8., 15.999*g/mole);
  G4Element* Ar  = new G4Element("Argon"   ,"Ar"  ,  18., 39.948*g/mole);
  G4Element* Cr = new G4Element("Chromium" ,"Cr" , 24., 51.99*g/mole);
  G4Element* Fe = new G4Element("Iron"     ,"Fe" , 26., 55.84*g/mole);
  G4Element* Ni = new G4Element("Nickel"   ,"Ni" , 28., 58.69*g/mole);
  

  //G4Material* beryllium = nist->FindOrBuildMaterial("G4_Be");
  //G4Material* lithium = nist->FindOrBuildMaterial("G4_Li");
  G4Material* aluminum = nist->FindOrBuildMaterial("G4_Al");
  G4Material* magnesium = nist->FindOrBuildMaterial("G4_Mg");
  //G4Material* graphite = nist->FindOrBuildMaterial("G4_Graphite");
  //G4Material* lead = nist->FindOrBuildMaterial("G4_Pb");

  G4Material* BeO = 
  new G4Material("BeO", 3.05*g/cm3, ncomponents=2);
  BeO->AddElement(Be, natoms=1);
  BeO->AddElement( O, natoms=1);
  
  G4Material* Be2 =
  new G4Material("Be2", 1.848*g/cm3, ncomponents=1);
  Be2->AddElement(Be, natoms=1);

  G4Material* Li2 =
  new G4Material("Li2", 0.534*g/cm3, ncomponents=1);
  Li2->AddElement(Li, natoms=1);

  G4Material* D2 =
  new G4Material("D2", 0.0017*g/cm3, ncomponents=1);
  D2->AddElement(D, natoms=1);

  G4Material* inox = 
  new G4Material("Stainless-Steel", 8*g/cm3, ncomponents=3);
  inox->AddElement(Fe, 74*perCent);
  inox->AddElement(Cr, 18*perCent);
  inox->AddElement(Ni,  8*perCent);

  G4Material* Air = 
  new G4Material("Air", 1.250*mg/cm3, ncomponents=3);
  Air->AddElement(N, 75.577*perCent);
  Air->AddElement(O, 23.135*perCent);
  Air->AddElement(Ar, 1.288*perCent);

  G4Material* AlMg3 =
  new G4Material("AlMg3", 2.66*g/cm3, ncomponents=2);
  AlMg3->AddMaterial(aluminum, 97.*perCent);
  AlMg3->AddMaterial(magnesium, 3.*perCent);


//Vacuum
  G4double atomicNumber = 1.;
  G4double massOfMole = 1.008*g/mole;
  G4double density = 1.e-25*g/cm3;
  G4double temperature = 2.73*kelvin;
  G4double pressure = 3.e-18*pascal;
  G4Material* Vacuum =
  new G4Material("Vacuum", atomicNumber, massOfMole, density,kStateGas, temperature, pressure);

  //set World material
  //fWorldMaterial = Air;
  fWorldMaterial = Vacuum;
  
  ///G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* DetectorConstruction::MaterialWithSingleIsotope( G4String name,
                           G4String symbol, G4double density, G4int Z, G4int A)
{
 // define a material from an isotope
 //
 G4int ncomponents;
 G4double abundance, massfraction;

 G4Isotope* isotope = new G4Isotope(symbol, Z, A);
 
 G4Element* element  = new G4Element(name, symbol, ncomponents=1);
 element->AddIsotope(isotope, abundance= 100.*perCent);
 
 G4Material* material = new G4Material(name, density, ncomponents=1);
 material->AddElement(element, massfraction=100.*perCent);

 return material;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  
  // compute dimensions
  G4double ContainRadius = fAbsorRadius + fContainThickness;
  G4double ContainLength = fAbsorLength + 2*fContainThickness;
  
  //G4double WorldSizeXY = 2.4*ContainRadius;
  //G4double WorldSizeZ  = 2.00001*ContainLength;
  //G4double WorldSizeXY = 4.00001*ContainRadius;
  G4double WorldSizeXY = 12*cm;
  G4double WorldSizeZ  = 20*cm;
  
  // World
  //
  G4Box*
  sWorld = new G4Box("World",                                    //name
              0.5*WorldSizeXY,0.5*WorldSizeXY,0.5*WorldSizeZ);   //dimensions
                   
  G4LogicalVolume*
  lWorld = new G4LogicalVolume(sWorld,                  //shape
                             fWorldMaterial,            //material
                             "World");                  //name

  fPWorld = new G4PVPlacement(0,                        //no rotation
                            G4ThreeVector(),            //at (0,0,0)
                            lWorld,                     //logical volume
                            "World",                    //name
                            0,                          //mother volume
                            false,                      //no boolean operation
                            0);                         //copy number

  // Container
  //
  G4Tubs* 
  sContain = new G4Tubs("Container",                            //name
             0., ContainRadius, 0.5*ContainLength, 0., twopi);  //dimensions

  fLContain = new G4LogicalVolume(sContain,            //shape
                       fContainMaterial,               //material
                       fContainMaterial->GetName());   //name

           new G4PVPlacement(0,                        //no rotation
                       G4ThreeVector(),                //at (0,0,0)
                       fLContain,                      //logical volume
                       fContainMaterial->GetName(),    //name
                       lWorld,                         //mother  volume
                       false,                          //no boolean operation
                       0);                             //copy number

  // Absorber
  //
  G4Tubs* 
  sAbsor = new G4Tubs("Absorber",                                //name
               0., fAbsorRadius, 0.5*fAbsorLength, 0., twopi);    //dimensions

  fLAbsor = new G4LogicalVolume(sAbsor,                //shape
                       fAbsorMaterial,                 //material
                       fAbsorMaterial->GetName());     //name

           new G4PVPlacement(0,                        //no rotation
                       G4ThreeVector(),                //at (0,0,0)
                       fLAbsor,                        //logical volume
                       fAbsorMaterial->GetName(),      //name
                       fLContain,                      //mother  volume
                       false,                          //no boolean operation
                       0);                             //copy number

  // Cylinder
  //
  G4Tubs*
  sCylinder = new G4Tubs("Cylinder",                                //name
               0., fAbsorRadius, 0.5*fCylinderThickness, 0., twopi);    //dimensions

  fLCylinder = new G4LogicalVolume(sCylinder,                //shape
                       fCylinderMaterial,                 //material
                       fCylinderMaterial->GetName());     //name

           new G4PVPlacement(0,                        //no rotation
                       G4ThreeVector(0,0,-((fAbsorLength+fCylinderThickness)*0.5)), //on top of Absorber
                       //G4ThreeVector(0,0,-WorldSizeZ*0.49),
                       fLCylinder,                        //logical volume
                       fCylinderMaterial->GetName(),      //name
                       lWorld,                      //mother  volume
                       false,                          //no boolean operation
                       0);                             //copy number


  PrintParameters();
  
  //always return the root volume
  //
  return fPWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintParameters()
{
 G4cout << "\n The Absorber  is a cylinder of " << fAbsorMaterial->GetName()
        << "  radius = " << G4BestUnit(fAbsorRadius,"Length")
        << "  length = " << G4BestUnit(fAbsorLength,"Length") << G4endl;
 G4cout << " The Container is a cylinder of " << fContainMaterial->GetName()
        << "  thickness = " << G4BestUnit(fContainThickness,"Length") << G4endl;
 
 G4cout << "\n" << fAbsorMaterial << G4endl;
 G4cout << "\n" << fContainMaterial << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetAbsorMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =
     G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);   
  
  if (pttoMaterial) { 
    fAbsorMaterial = pttoMaterial;
    if(fLAbsor) { fLAbsor->SetMaterial(fAbsorMaterial); }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetAbsorMaterial : "
           << materialChoice << " not found" << G4endl;
  }              
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetContainMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =
     G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);   
  
  if (pttoMaterial) { 
    fContainMaterial = pttoMaterial;
    if(fLContain) { fLContain->SetMaterial(fContainMaterial); }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetContainMaterial : "
           << materialChoice << " not found" << G4endl;
  }              
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetCylinderMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =
     G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);

  if (pttoMaterial) {
    fCylinderMaterial = pttoMaterial;
    if(fLCylinder) { fLCylinder->SetMaterial(fCylinderMaterial); }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetCylinderMaterial : "
           << materialChoice << " not found" << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetAbsorRadius(G4double value)
{
  fAbsorRadius = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetAbsorLength(G4double value)
{
  fAbsorLength = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetContainThickness(G4double value)
{
  fContainThickness = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetCylinderThickness(G4double value)
{
  fCylinderThickness = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

