//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "myPrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
// #include "G4GeneralParticleSource.hh"
// #include "G4SPSAngDistribution.hh"
// #include "G4SPSEneDistribution.hh"
// #include "G4SPSPosDistribution.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myPrimaryGeneratorAction::myPrimaryGeneratorAction(myDetectorConstruction *det)
    : G4VUserPrimaryGeneratorAction()
{
    particleGun = new G4ParticleGun(1);
    detector = det;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myPrimaryGeneratorAction::~myPrimaryGeneratorAction()
{
    delete particleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
    // static double m = -40. * cm, n = -40. * cm;
    // m += 5 * cm;
    // n += 5 * cm;
    G4ThreeVector layerSize = detector->GetLayerSize();
    particleGun->SetParticlePosition(G4ThreeVector((G4UniformRand() * layerSize.x() - 0.5 * layerSize.x()) / 2., (G4UniformRand() * layerSize.y() - 0.5 * layerSize.y()) / 2., 0));
    G4ThreeVector direction;
    direction.setX(G4UniformRand() - 0.5);
    direction.setY(G4UniformRand() - 0.5);
    direction.setZ(1);
    direction = direction / direction.mag();
    particleGun->SetParticleMomentumDirection(direction);
    // static double particleEnergy = 30.0  * GeV;
    // static int count = 1;
    // particleGun->SetParticleEnergy(particleEnergy);
    // if (!(count % 200))
    //     particleEnergy = particleEnergy + 0.5 * GeV;
    // G4cout << "Primary Energy: " << particleEnergy << G4endl;
    // G4cout << "count: " << count << G4endl;
    particleGun->GeneratePrimaryVertex(anEvent);
    // count++;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
