/// \file EventAction.cc
/// \brief Implementation of the B4c::EventAction class

//gestisce le azioni a inizio e fine di ogni evento 

#include "EventAction.hh"

#include "CalorHit.hh"

#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"

#include <iomanip>

namespace B4c
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CalorHitsCollection* EventAction::GetHitsCollection(G4int hcID, const G4Event* event) const
{
  auto hitsCollection = static_cast<CalorHitsCollection*>(event->GetHCofThisEvent()->GetHC(hcID));

  if (!hitsCollection) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID;
    G4Exception("EventAction::GetHitsCollection()", "MyCode0003", FatalException, msg);
  }

  return hitsCollection;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::PrintEventStatistics(G4double absoEdep, G4double absoTrackLength,
                                       G4double scintEdep, G4double scintTrackLength) const
{
  // print event statistics
  G4cout << "   Absorber: total energy: " << std::setw(7) << G4BestUnit(absoEdep, "Energy")
         << "       total track length: " << std::setw(7) << G4BestUnit(absoTrackLength, "Length")
         << G4endl << "        Scint: total energy: " << std::setw(7) << G4BestUnit(scintEdep, "Energy")
         << "       total track length: " << std::setw(7) << G4BestUnit(scintTrackLength, "Length")
         << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* /*event*/) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  // Get hits collections IDs (only once)
  if (fAbsHCID == -1) {
    fAbsHCID = G4SDManager::GetSDMpointer()->GetCollectionID("AbsorberHitsCollection");
    fScintHCID = G4SDManager::GetSDMpointer()->GetCollectionID("ScintHitsCollection");
  }

  // Get hits collections
  auto absoHC = GetHitsCollection(fAbsHCID, event);
  auto scintHC = GetHitsCollection(fScintHCID, event);

  // Get Calorimeter hits collection ID (solo la prima volta)
  if (fCalorHCID == -1) {
    //fCalorHCID = G4SDManager::GetSDMpointer()->GetCollectionID("CalorimeterHitsCollection");
    fCalorHCID = G4SDManager::GetSDMpointer()

               ->GetCollectionID("ScintHitsCollection");
               G4cout << "Calor HCID = " << fCalorHCID << G4endl;
    if (fCalorHCID == -1) {
    auto sdManager = G4SDManager::GetSDMpointer();
    fCalorHCID = sdManager->GetCollectionID("ScintHitsCollection");

}

if (fCalorHCID < 0) {
    G4Exception("EventAction::EndOfEventAction",
                "MyCode001",
                FatalException,
                "CalorimeterHitsCollection not found.");
}
}

auto calorHC = GetHitsCollection(fCalorHCID, event);
G4double firstPhotonTime = -1.0; // inizializzazione

if (calorHC && calorHC->entries() > 0) {  

    for (size_t i = 0; i < calorHC->entries(); ++i) {
        B4c::CalorHit* hit = (*calorHC)[i];
        G4double t = hit->GetFirstPhotonTime(); // usa il getter
        if (t >= 0. && (firstPhotonTime < 0. || t < firstPhotonTime)) {
            firstPhotonTime = t;
        }
    }
} else {
    G4cout << "Warning: CalorimeterHitsCollection not found or empty in this event!" << G4endl;
}


  // Get hit with total values
  auto absoHit = (*absoHC)[absoHC->entries() - 1];
  auto scintHit = (*scintHC)[scintHC->entries() - 1];
  G4int nPhotons = absoHit->GetPhotonCount();

  // Print per event (modulo n)
  //
  auto eventID = event->GetEventID();
  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ((printModulo > 0) && (eventID % printModulo == 0)) {
    PrintEventStatistics(absoHit->GetEdep(), absoHit->GetTrackLength(), scintHit->GetEdep(),
                         scintHit->GetTrackLength());


    G4cout << "--> End of event: " << eventID << "\n" << G4endl;
  }

  // Fill histograms, ntuple
  //

  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // fill histograms
  analysisManager->FillH1(0, absoHit->GetEdep());
  analysisManager->FillH1(1, scintHit->GetEdep());
  analysisManager->FillH1(2, absoHit->GetTrackLength());
  analysisManager->FillH1(3, scintHit->GetTrackLength());

  // fill ntuple
  analysisManager->FillNtupleDColumn(0, absoHit->GetEdep());
  analysisManager->FillNtupleDColumn(1, scintHit->GetEdep());
  analysisManager->FillNtupleDColumn(2, absoHit->GetTrackLength());
  analysisManager->FillNtupleDColumn(3, scintHit->GetTrackLength());
  analysisManager->FillNtupleIColumn(4, nPhotons);
  analysisManager->FillNtupleDColumn(5, firstPhotonTime); 
  analysisManager->AddNtupleRow();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B4c
