/// \file CalorimeterSD.cc
/// \brief Implementation of the B4c::CalorimeterSD class

#include "CalorimeterSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4ios.hh" 
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include "G4StepPoint.hh"

namespace B4c
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CalorimeterSD::CalorimeterSD(const G4String& name, 
                             const G4String& hitsCollectionName,
                             G4int nofCells)
  : G4VSensitiveDetector(name), 
  fNofCells(nofCells)
{
  collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CalorimeterSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection = new CalorHitsCollection(SensitiveDetectorName, collectionName[0]);
  G4int hcID = GetCollectionID(0);
  // Add this collection in hce
  // auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);
  

  // Create hits
  // fNofCells for cells + one more for total sums
  for (G4int i = 0; i < fNofCells + 1; i++) {
    fHitsCollection->insert(new CalorHit());
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool CalorimeterSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  // energy deposit
  auto edep = step->GetTotalEnergyDeposit();
   auto pid = step->GetTrack()->GetDefinition()->GetParticleName();
 

  // step length
  G4double stepLength = 0.;
  if (step->GetTrack()->GetDefinition()->GetPDGCharge() != 0.) {
    stepLength = step->GetStepLength();
  }

  if (edep == 0. && stepLength == 0. && pid != "opticalphoton")
    return false;
  

  auto touchable = (step->GetPreStepPoint()->GetTouchable());

  // Get calorimeter cell id
  auto layerNumber = touchable->GetReplicaNumber(1);

  // Get hit accounting data for this cell
  auto hit = (*fHitsCollection)[layerNumber];
  if (!hit) {
    G4ExceptionDescription msg;
    msg << "Cannot access hit " << layerNumber;
    G4Exception("CalorimeterSD::ProcessHits()", "MyCode0004", FatalException, msg);
  }

  // Get hit for total accounting
  auto hitTotal = (*fHitsCollection)[fHitsCollection->entries() - 1];

  // Add values
  hit->Add(edep, stepLength);
  hitTotal->Add(edep, stepLength);

   
    

  /*  if (pid == "opticalphoton" &&
    step->GetPostStepPoint()->GetPhysicalVolume() &&
    step->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "SiPM")
{
    G4double photE    = step->GetPostStepPoint()->GetKineticEnergy();
    G4double photTime = step->GetPostStepPoint()->GetGlobalTime();

    hitTotal->AddPhoton(photE, photTime);

    // fermiamo il fotone per evitare doppi conteggi
    Kill(step);
}*/ 

if (pid == "opticalphoton" &&
    step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary)
{
    G4double photE    = step->GetPreStepPoint()->GetKineticEnergy();
    G4double photTime = step->GetPreStepPoint()->GetGlobalTime();

    hitTotal->AddPhoton(photE, photTime);
    Kill(step);
}
    

  return true;

}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CalorimeterSD::EndOfEvent(G4HCofThisEvent*)
{
  if (verboseLevel > 1) {
    auto nofHits = fHitsCollection->entries();
    G4cout << G4endl << "-------->Hits Collection: in this event they are " << nofHits
           << " hits in the tracker chambers: " << G4endl;
    for (std::size_t i = 0; i < nofHits; ++i)
      (*fHitsCollection)[i]->Print();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......




void CalorimeterSD::TellVolume(const G4Step* aStep)
{
    if (!aStep->GetPreStepPoint()->GetPhysicalVolume() ||
        !aStep->GetPostStepPoint()->GetPhysicalVolume())
        return;

    G4cout << "PreStep: "
           << aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()
           << " *** PostStep: "
           << aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName()
           << G4endl;
}


void CalorimeterSD::Kill(G4Step* step)
{
    step->GetTrack()->SetTrackStatus(fStopAndKill);
}

G4bool CalorimeterSD::Mcross(const G4Step* aStep,
                             const G4String& vol1_name,
                             const G4String& vol2_name)
{
    if (!aStep->GetPreStepPoint()->GetPhysicalVolume() ||
        !aStep->GetPostStepPoint()->GetPhysicalVolume())
        return false;

    auto preName  = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
    auto postName = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();

    return (preName == vol1_name && postName == vol2_name);
}



}  // namespace B4c
