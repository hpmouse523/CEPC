#ifndef ExN03PrimaryGeneratorAction_h
#define ExN03PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4UnitsTable.hh"
#include "myDetectorConstruction.hh"
#include "G4ParticleGun.hh"
#define PI 3.14159265397

class G4GeneralParticleSource;
class G4Event;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class myPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
	public:
		myPrimaryGeneratorAction(myDetectorConstruction *);    
		~myPrimaryGeneratorAction();

	public:
		void GeneratePrimaries(G4Event*);

	private:
        G4ParticleGun *particleGun;
		const myDetectorConstruction *detector;
		

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


