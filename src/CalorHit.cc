/// \file CalorHit.cc
/// \brief Implementation of the B4c::CalorHit class
//immagazzina l'informazione fisica raccolta nel detector sensibile, inserisce gli oggetti che ha in una Hitscollection

#include "CalorHit.hh"

#include "G4UnitsTable.hh"

#include <iomanip>
#include "G4SystemOfUnits.hh"
namespace B4c
{

G4ThreadLocal G4Allocator<CalorHit>* CalorHitAllocator = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool CalorHit::operator==(const CalorHit& right) const
{
  return (this == &right) ? true : false;
}

void B4c::CalorHit::AddPhoton(G4double, G4double time)
{
    fPhotonCount++;

    if (fFirstPhotonTime < 0.)
        fFirstPhotonTime = time;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


  void CalorHit::Print()
{
  G4cout << "Edep: " << std::setw(7) << G4BestUnit(fEdep, "Energy")
         << " track length: " << std::setw(7) << G4BestUnit(fTrackLength, "Length")
         << G4endl;

  
G4cout << "Photons detected: " << fPhotonCount << G4endl;
G4cout << "First photon time: " << fFirstPhotonTime/ns << " ns" << G4endl;
}


  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B4c
