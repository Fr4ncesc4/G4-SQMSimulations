
/// \file CalorimeterSD.hh
/// \brief Definition of the B4c::CalorimeterSD class

#ifndef B4cCalorimeterSD_h
#define B4cCalorimeterSD_h 1

#include "CalorHit.hh" //  Definisce il tipo di hit che questo sensitive detector produrrà.

                       //Contiene la struttura dati che memorizza energia depositata, posizione, ecc.

#include "G4VSensitiveDetector.hh" 
#include "globals.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

namespace B4c
{

/// Calorimeter sensitive detector class
///
/// In Initialize(), it creates one hit for each calorimeter layer and one more
/// hit for accounting the total quantities in all layers.
///
/// The values are accounted in hits in ProcessHits() function which is called
/// by Geant4 kernel at each step.

class CalorimeterSD : public G4VSensitiveDetector
{
  public:
    CalorimeterSD(const G4String& name, 
                  const G4String& hitsCollectionName, 
                  G4int nofCells);
    ~CalorimeterSD() override = default;

    // methods from base class
    void Initialize(G4HCofThisEvent* hitCollection) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    void EndOfEvent(G4HCofThisEvent* hitCollection) override;

   //methods from CosmicCherenkov simulations

    G4bool Mcross(const G4Step* aStep,
                  const G4String& vol1_name,
                  const G4String& vol2_name);

    void Kill(G4Step* step);

    void TellVolume(const G4Step* aStep);



  private:
    CalorHitsCollection* fHitsCollection = nullptr;
    G4int fNofCells = 0;
};

}  // namespace B4c

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
