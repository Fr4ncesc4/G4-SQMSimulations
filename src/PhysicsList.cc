#include "PhysicsList.hh"

// EM standard
#include "G4EmStandardPhysics_option4.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4OpticalPhysics.hh"

// Unità e definizioni
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

PhysicsList::PhysicsList()
: G4VModularPhysicsList()
{
    // Livello di verbosità
    SetVerboseLevel(1);
    RegisterPhysics(new G4EmStandardPhysics_option4());

    // (Opzionale) puoi aggiungere altre fisiche qui:
    //RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4HadronElasticPhysics());
    RegisterPhysics(new G4OpticalPhysics());
}

PhysicsList::~PhysicsList()
{
}
//void PhysicsList::ConstructParticle()
//{
//G4IonConstructor ionconstructor;
//ionconstructor.ConstructParticle(); 
//}
void PhysicsList::SetCuts()
{
    // Cut di default
    SetCutsWithDefault();

    // Stampa dei cut (utile per debug)
    if (verboseLevel > 0)
    {
        DumpCutValuesTable();
    }
}



