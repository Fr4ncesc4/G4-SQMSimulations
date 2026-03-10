/// \file DetectorConstruction.cc
/// \brief Implementation of the B4c::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "CalorimeterSD.hh"

#include "G4Material.hh" 
#include "G4NistManager.hh" 

#include "G4AutoDelete.hh"
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "G4PhysicalConstants.hh" 

#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"//
#include "G4VisAttributes.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"


#include "G4MaterialPropertiesTable.hh"
#include "G4MaterialPropertyVector.hh"

#include "G4SubtractionSolid.hh"


// Calorimeter = scintillatore 


namespace B4c
{
G4ThreadLocal
G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = nullptr;
 

G4VPhysicalVolume* DetectorConstruction::Construct() //PRIMO METODO PRINCIPALE, 
                                                    //CHIAMA DEFINE MATERIALS E DEFINE VOLUMES,
                                                   // quindi prima dici con cosa sono fatti, 
                                                  //poi come, e poi posizioni i materiali

{
  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
}


void DetectorConstruction::DefineMaterials() //Silicio,Mylar,EJ-208,Galactic
{


// Silicon, Mylar
  auto nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_Si");
  nistManager->FindOrBuildMaterial("G4_MYLAR");

// EJ-208 scintillation material  
  

 auto matEJ208 = nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
 matEJ208->GetIonisation()->SetBirksConstant(0.16*mm/MeV);
  
  const G4int EJ208num = 55;
  G4double EJ208_Energy[EJ208num]={2.481*eV, 2.487*eV, 2.498*eV, 2.515*eV, 2.527*eV, 2.544*eV, 2.558*eV, 2.572*eV, 2.585*eV, 2.594*eV, 2.608*eV, 2.622*eV, 2.638*eV, 2.655*eV, 2.674*eV, 2.687*eV, 2.708*eV, 2.743*eV, 2.762*eV, 2.77*eV, 2.787*eV, 2.798*eV, 2.807*eV, 2.822*eV, 2.831*eV, 2.843*eV, 2.859*eV, 2.88*eV, 2.894*eV, 2.902*eV, 2.916*eV, 2.926*eV, 2.935*eV, 2.94*eV, 2.947*eV, 2.952*eV, 2.955*eV, 2.961*eV, 2.966*eV, 2.971*eV, 2.977*eV, 2.982*eV, 2.989*eV, 2.996*eV, 3.002*eV, 3.007*eV, 3.018*eV, 3.023*eV, 3.034*eV, 3.044*eV, 3.055*eV, 3.07*eV, 3.083*eV, 3.102*eV, 3.124*eV};
  G4double EJ208_SCINT[EJ208num] = {0.05845, 0.06153, 0.07076, 0.08307, 0.09845, 0.11691, 0.1323, 0.15383, 0.17537, 0.19691, 0.22767, 0.26152, 0.30767, 0.3569, 0.39997, 0.42151, 0.45227, 0.52303, 0.56918, 0.5938, 0.64918, 0.68302, 0.71686, 0.76917, 0.80609, 0.85224, 0.90147, 0.95069, 0.98147, 0.99685, 0.99992, 0.99377, 0.98147, 0.94762, 0.90762, 0.8707, 0.84301, 0.80301, 0.73841, 0.68918, 0.63687, 0.58765, 0.53226, 0.46458, 0.41843, 0.36305, 0.30459, 0.26152, 0.20306, 0.16614, 0.11076, 0.04615, 0.01846, 0.00923, 0.00308};
  G4double EJ208_RIND[EJ208num]  = {1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,
  1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58,1.58};
  
  // Attenuation length found between 300 and 400 cm; for the moment we take 380*cm as from NIM A 552 (2005) 449
  
  G4double EJ208_ABSL[EJ208num]  = { 380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm,380*cm};


//assegni al materiale EJ-208 una Material Properties Table (MPT) con spettroemiss,n,Labs,yeld,time
//Creazione del materiale, è il contenitore dove G4 legge 

  auto MPTEJ208 = new G4MaterialPropertiesTable();
  MPTEJ208->AddProperty("SCINTILLATIONCOMPONENT1", EJ208_Energy, EJ208_SCINT, EJ208num);
  MPTEJ208->AddProperty("RINDEX",        EJ208_Energy, EJ208_RIND,  EJ208num);
  MPTEJ208->AddProperty("ABSLENGTH",     EJ208_Energy, EJ208_ABSL,  EJ208num);

  MPTEJ208->AddConstProperty("SCINTILLATIONYIELD",10574/MeV); 

  MPTEJ208->AddConstProperty("RESOLUTIONSCALE",1.0);
  MPTEJ208->AddConstProperty("SCINTILLATIONTIMECONSTANT1",2.1*ns);
  MPTEJ208->AddConstProperty("SCINTILLATIONTIMECONSTANT2",2.1*ns);
  MPTEJ208->AddConstProperty("SCINTILLATIONRISETIME2",0.9*ns);
  MPTEJ208->AddConstProperty("SCINTILLATIONRISETIME1",0.9*ns);
  matEJ208->SetMaterialPropertiesTable(MPTEJ208);





  // Vacuum
  G4double a;
  G4double z;
  G4double density; 
  new G4Material("Galactic", z = 1., a = 1.01 * g / mole, density = universe_mean_density,
                 kStateGas, 2.73 * kelvin, 3.e-18 * pascal);
  auto gal = G4Material::GetMaterial("Galactic");

  const G4int n = 2;
  G4double E[n] = {2.0*eV, 3.5*eV};
  G4double R[n] = {1.0, 1.0};

  auto mptGal = new G4MaterialPropertiesTable();
  mptGal->AddProperty("RINDEX", E, R, n);
  gal->SetMaterialPropertiesTable(mptGal);
  

// Optical grease / coupling layer
G4double densityGrease = 1.0*g/cm3;
auto grease = new G4Material("OpticalGrease", densityGrease, 2);
grease->AddElement(nistManager->FindOrBuildElement("C"), 2);
grease->AddElement(nistManager->FindOrBuildElement("H"), 2);

const G4int nGrease = 2;
G4double eGrease[nGrease]   = {2.0*eV, 3.5*eV};
G4double rGrease[nGrease]   = {1.46, 1.46};
G4double absGrease[nGrease] = {10.*cm, 10.*cm};

auto mptGrease = new G4MaterialPropertiesTable();
mptGrease->AddProperty("RINDEX",    eGrease, rGrease, nGrease);
mptGrease->AddProperty("ABSLENGTH", eGrease, absGrease, nGrease);

grease->SetMaterialPropertiesTable(mptGrease);

 // Silicon (G4_Si) - optical properties for the SiPM
auto si = G4Material::GetMaterial("G4_Si");

const G4int nSi = 2;
G4double eSi[nSi]   = {2.0*eV, 3.5*eV};
G4double rSi[nSi]   = {3.8, 3.8};      // first approximation
G4double absSi[nSi] = {1.*um, 1.*um};  // strong absorption in silicon

auto mptSi = new G4MaterialPropertiesTable();
mptSi->AddProperty("RINDEX",    eSi, rSi, nSi);
mptSi->AddProperty("ABSLENGTH", eSi, absSi, nSi);

si->SetMaterialPropertiesTable(mptSi);

}
//QUI COSTRUISCI mondo, wrappyng in mylar, scint, layer, gap e SiPM

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
  // Geometry parameters //scint = 10x1cm e 0.5 cm di spessore
  fNofLayers = 1;
  //G4double absoThickness = 10. * mm;

  //G4double gapThickness = 5. * mm;
  G4double scintThickness = 5. * mm;
  G4double calorSizeX = 10. * cm; 
  G4double calorSizeY = 1. *cm; 


  G4double mylarThick = 0.10*mm;
  G4double airThick   = 0.10*mm;

  //auto layerThickness = absoThickness + gapThickness;
  //auto layerThickness = gapThickness;
  auto layerThickness = scintThickness;


  auto calorThickness = fNofLayers * layerThickness;
  //auto worldSizeXY = 30.0 * cm;
  //auto worldSizeZ  = 30.0 * cm;
  auto worldSizeXY = 1.0 * m;
  auto worldSizeZ  = 1.0 * m;
  //auto worldSizeXY = 1.2 * calorSizeXY;
  //auto worldSizeZ = 1.2 * calorThickness;


auto mylarOuterS = new G4Box("MylarOuter",
                             calorSizeX/2 + mylarThick,
                             calorSizeY/2 + mylarThick,
                             calorThickness/2 + mylarThick);

auto mylarInnerS = new G4Box("MylarInner",
                             calorSizeX/2,
                             calorSizeY/2,
                             calorThickness/2);

// guscio sottile: box esterno - box interno
auto mylarShellS = new G4SubtractionSolid("MylarShell",
                                          mylarOuterS,
                                          mylarInnerS);

// finestra per il SiPM

G4double winY = 4.0*mm;
G4double winZ = 4.0*mm;
G4double winX = 2.0*mm;

auto winS = new G4Box("MylarWindow", winX/2, winY/2, winZ/2);


// finestra centrata sulla faccia +X del guscio
G4ThreeVector winPos((calorSizeX/2 + mylarThick) - winX/2, 0.0, 0.0);

auto mylarWrapS = new G4SubtractionSolid("MylarWrap",
                                         mylarShellS,
                                         winS,
                                         nullptr,
                                         winPos);





  // GET MATERIALS
  auto defaultMaterial = G4Material::GetMaterial("Galactic");
  //auto absorberMaterial = defaultMaterial; // Assorbitore = vuoto, galactic

  //auto gapMaterial = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  auto scintMaterial = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  //auto defaultMaterial = gapMaterial;
  auto mylarMat = G4Material::GetMaterial("G4_MYLAR");
  auto sipmMaterial = G4Material::GetMaterial("G4_Si");
  auto greaseMaterial = G4Material::GetMaterial("OpticalGrease");
  
  if (!scintMaterial|| !sipmMaterial|| !mylarMat|| !greaseMaterial) {

    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined.";
    G4Exception("DetectorConstruction::DefineVolumes()", "MyCode0001", FatalException, msg);
  }

  // if (!defaultMaterial || !absorberMaterial || !gapMaterial) {
  // G4ExceptionDescription msg;
  //msg << "Cannot retrieve materials already defined.";
  //G4Exception("DetectorConstruction::DefineVolumes()", "MyCode0001", FatalException, msg);
  //}

  

  //
  // World
  //
  auto worldS = new G4Box("World",  // its name
                          worldSizeXY / 2, worldSizeXY / 2, worldSizeZ / 2);  // its size

  auto worldLV = new G4LogicalVolume(worldS,  // its solid
                                     defaultMaterial,  // its material
                                     "World");  // its name

  auto worldPV = new G4PVPlacement(nullptr,  // no rotation
                                   G4ThreeVector(),  // at (0,0,0)
                                   worldLV,  // its logical volume
                                   "World",  // its name
                                   nullptr,  // its mother  volume
                                   false,  // no boolean operation
                                   0,  // copy number
                                   fCheckOverlaps);  // checking overlaps


  auto mylarWrapLV = new G4LogicalVolume(mylarWrapS, mylarMat, "MylarWrapLV");

  new G4PVPlacement(nullptr,
                  G4ThreeVector(),
                  mylarWrapLV,
                  "MylarWrap",
                  worldLV,
                  false,
                  0,
                  fCheckOverlaps);

  // Optical surface: reflective Mylar

auto mylarSurf = new G4OpticalSurface("MylarSurface");
mylarSurf->SetType(dielectric_metal);
mylarSurf->SetModel(unified);
mylarSurf->SetFinish(groundfrontpainted);
mylarSurf->SetSigmaAlpha(0.1);

const G4int nMy = 2;
G4double eMy[nMy] = {2.0*eV, 3.5*eV};
G4double refl[nMy] = {0.98, 0.98};
G4double eff[nMy]  = {0.0,  0.0};

auto mylarMPT = new G4MaterialPropertiesTable();
mylarMPT->AddProperty("REFLECTIVITY", eMy, refl, nMy);
mylarMPT->AddProperty("EFFICIENCY",   eMy, eff,  nMy);
mylarSurf->SetMaterialPropertiesTable(mylarMPT);

new G4LogicalSkinSurface("MylarSkinSurface", mylarWrapLV, mylarSurf);

  //
  // Calorimeter = Scintillator
  //
  auto calorimeterS = new G4Box("Calorimeter",  // its name
                                calorSizeX / 2, calorSizeY / 2, calorThickness / 2);  // its size

  auto calorLV = new G4LogicalVolume(calorimeterS,  // its solid

                                     scintMaterial,//defaultMaterial,  // its material

                                     "Calorimeter");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    G4ThreeVector(),  // at (0,0,0)
                    calorLV,  // its logical volume
                    "Calorimeter",  // its name
                    mylarWrapLV,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    fCheckOverlaps);  // checking overlaps

  //
  // Layer
  //
  auto layerS = new G4Box("Layer",  // its name
                          calorSizeX / 2, calorSizeY / 2, layerThickness / 2);  // its size

  auto layerLV = new G4LogicalVolume(layerS,  // its solid

                                     scintMaterial,//defaultMaterial,  // its material

                                     "Layer");  // its name

  new G4PVReplica("Layer",  // its name
                  layerLV,  // its logical volume
                  calorLV,  // its mother
                  kZAxis,  // axis of replication
                  fNofLayers,  // number of replica
                  layerThickness);  // witdth of replica

  //
  // Absorber
  //
  /*auto absorberS = new G4Box("Abso",  // its name
                             calorSizeXY / 2, calorSizeXY / 2, absoThickness / 2);  // its size

  auto absorberLV = new G4LogicalVolume(absorberS,  // its solid
                                        absorberMaterial,  // its material
                                        "AbsoLV");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    G4ThreeVector(0., 0., -gapThickness / 2),  // its position
                    absorberLV,  // its logical volume
                    "Abso",  // its name
                    layerLV,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    fCheckOverlaps);  // checking overlaps
    
*/
  //
  // Scint
  //

  auto scintS = new G4Box("Scint",  // its name
                        calorSizeX / 2, calorSizeY / 2, scintThickness / 2);  // its size


  auto scintLV = new G4LogicalVolume(scintS,  // its solid
                                   scintMaterial,  // its material
                                   "ScintLV");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    G4ThreeVector(0., 0., 0.),  // its position // al posto del terzo 0 c'era absoThickness / 2

                    scintLV,  // its logical volume
                    "Scint",  // its name

                    layerLV,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    fCheckOverlaps);  // checking overlaps


//
// Optical grease between scintillator and SiPM
//
G4double greaseThick = 0.05 * mm;
G4double greaseX = greaseThick;
G4double greaseY = 3.0 * mm;
G4double greaseZ = 3.0 * mm;

auto greaseS = new G4Box("Grease", greaseX/2, greaseY/2, greaseZ/2);

auto greaseLV = new G4LogicalVolume(greaseS,
                                    greaseMaterial,
                                    "GreaseLV");

G4double greasePosX = calorSizeX/2 + mylarThick + greaseX/2;

new G4PVPlacement(nullptr,
                  G4ThreeVector(greasePosX, 0.0, 0.0),
                  greaseLV,
                  "Grease",
                  worldLV,
                  false,
                  0,
                  fCheckOverlaps);


  
  //
// SiPM
//
G4double sipmX = 3.0 * mm;
G4double sipmY = 3.0 * mm;
G4double sipmZ = 1.0 * mm;

//auto sipmMaterial = G4Material::GetMaterial("SiPM_Silicon");

auto sipmS = new G4Box("SiPM", sipmX/2, sipmY/2, sipmZ/2);

auto sipmLV = new G4LogicalVolume(sipmS,
                                  sipmMaterial,
                                  "SiPMLV");

// Posizionato sopra il calorimetro
/*G4double sipmPosZ = calorThickness/2. + sipmZ/2.;

new G4PVPlacement(nullptr,
                  G4ThreeVector(0., 0., sipmPosZ),
                  sipmLV,
                  "SiPM",
                  worldLV,
                  false,
                  0,
                  fCheckOverlaps);*/
G4double margine = 0.1 * mm ; 
auto rotY = new G4RotationMatrix();
rotY->rotateY(90.*deg);

G4double sipmPosX = calorSizeX/2 + mylarThick + greaseThick + sipmZ/2 + margine;

new G4PVPlacement(rotY,//nullptr,
                  G4ThreeVector(sipmPosX, 0.0, 0.0),
                  //G4ThreeVector(calorSizeX/2 + sipmZ/2 + margine,0.0, 0.0), //(calorSizeX/2 + sipmY/2,0.0,sipmZ/2)
                  sipmLV,
                  "SiPM",
                  worldLV,
                  false,
                  0,
                  fCheckOverlaps);




// Simple SiPM optical detection surface 
auto sipmSurf = new G4OpticalSurface("SiPMSurface");
sipmSurf->SetType(dielectric_metal);
sipmSurf->SetModel(unified);
sipmSurf->SetFinish(polished);

const G4int nSipmSurf = 2;
G4double eSipmSurf[nSipmSurf]    = {2.0*eV, 3.5*eV};
G4double reflSipm[nSipmSurf]     = {0.0, 0.0};
G4double effSipm[nSipmSurf]      = {0.25, 0.25};  // first average PDE approximation

auto sipmSurfMPT = new G4MaterialPropertiesTable();
sipmSurfMPT->AddProperty("REFLECTIVITY", eSipmSurf, reflSipm, nSipmSurf);
sipmSurfMPT->AddProperty("EFFICIENCY",   eSipmSurf, effSipm,  nSipmSurf);

sipmSurf->SetMaterialPropertiesTable(sipmSurfMPT);

new G4LogicalSkinSurface("SiPMSkinSurface", sipmLV, sipmSurf);

// Visualizzazione rossa
auto sipmVis = new G4VisAttributes(G4Colour(1.,0.,0.));
sipmLV->SetVisAttributes(sipmVis);
// Visualizzazione verde 
auto greaseVis = new G4VisAttributes(G4Colour(0.,1.,0.));
greaseLV->SetVisAttributes(greaseVis);

/*
  //
  // print parameters
  //
  G4cout << G4endl << "------------------------------------------------------------" << G4endl
         << "---> The calorimeter is " << fNofLayers << " layers of: [ " << absoThickness / mm
         << "mm of " << absorberMaterial->GetName() << " + " << gapThickness / mm << "mm of "
         << gapMaterial->GetName() << " ] " << G4endl
         << "------------------------------------------------------------" << G4endl;
*/

//
  // print parameters
  //
  G4cout << G4endl << "------------------------------------------------------------" << G4endl

         << "---> The calorimeter is " << fNofLayers << " layers of: [ "<< scintThickness / mm << "mm of "
         << scintMaterial->GetName() << " ] " << G4endl
         << "------------------------------------------------------------" << G4endl;



  //
  // Visualization attributes
  //
  worldLV->SetVisAttributes(G4VisAttributes::GetInvisible());
  calorLV->SetVisAttributes(G4VisAttributes(G4Colour::White()));
 
  //
  // Always return the physical World
  //
  return worldPV;
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
  // G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  //
  // Sensitive detectors
  //
 // auto absoSD = new CalorimeterSD("AbsorberSD", "AbsorberHitsCollection", fNofLayers);
  //G4SDManager::GetSDMpointer()->AddNewDetector(absoSD);
  //SetSensitiveDetector("AbsoLV", absoSD);



//DetectorConstruction crea o registra per esempio CalorimeterSD e lo associa a uno o più LogicalVolume.


  auto scintSD = new CalorimeterSD("ScintSD", "ScintHitsCollection", fNofLayers);
  G4SDManager::GetSDMpointer()->AddNewDetector(scintSD);
  SetSensitiveDetector("ScintLV", scintSD);

  auto sipmSD = new CalorimeterSD("SiPMSD", "AbsorberHitsCollection", 1);
  G4SDManager::GetSDMpointer()->AddNewDetector(sipmSD);
  SetSensitiveDetector("SiPMLV", sipmSD);


  //
  // Magnetic field
  //
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue;
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);

  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B4c



