/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the B4::PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4PhysicalConstants.hh"

#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"


namespace B4
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);
  
 
  //default particle kinematic

  auto particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("mu-");
  
  fParticleGun->SetParticleDefinition(particleDefinition);  
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
  fParticleGun->SetParticleEnergy(300. * MeV);
  // This function is called at the begining of event

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume
  // from G4LogicalVolumeStore
  //
  G4double worldZHalfLength = 0.;
  auto worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");

  // Check that the world volume has box shape
  G4Box* worldBox = nullptr;
  if (worldLV) {
    worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
  }

  if (worldBox) {
    worldZHalfLength = worldBox->GetZHalfLength();
  }
  else {
    G4ExceptionDescription msg;
    msg << "World volume of box shape not found." << G4endl;
    msg << "Perhaps you have changed geometry." << G4endl;
    msg << "The gun will be place in the center.";
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries()", "MyCode0002", JustWarning, msg);
  }

    // Creazione nucleo esotico
  // G4int Z = 60;      // numero di protoni
  //G4int A = 300;     // numero totale di nucleoni
  //G4double E = 0.0*keV;  // energia di eccitazione nucleare
  /*auto particleDefinition = G4IonTable::GetIonTable()->GetIon(Z, A, E);
  fParticleGun->SetParticleDefinition(particleDefinition);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
  fParticleGun->SetParticleEnergy(90000.*GeV);*/


  
  // Set gun position
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -worldZHalfLength));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  
  fParticleGun->GeneratePrimaryVertex(event);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B4
