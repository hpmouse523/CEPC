
#include "mySteppingAction.hh"
#include "myDetectorConstruction.hh"
#include "myEventAction.hh"
#include "G4Track.hh"
#include "G4TouchableHistory.hh"
#include "myAnalysis.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include <string>
using std::string;
////#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

mySteppingAction::mySteppingAction(myDetectorConstruction *det,
                                   myEventAction *evt)
    : detector(det), eventaction(evt)
{
  //G4cout<<"mySteppingAction::mySteppingAction\n";
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

mySteppingAction::mySteppingAction()
{
  detector = nullptr;
  eventaction = nullptr;
}

mySteppingAction::~mySteppingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void mySteppingAction::UserSteppingAction(const G4Step *aStep)
{

  const G4Event *evt = G4RunManager::GetRunManager()->GetCurrentEvent();

  if (detector == nullptr)
  {
    G4cout << "Can't find detector information" << G4endl;
    exit(1);
  }

  auto analysisManager = G4AnalysisManager::Instance();
  G4ThreeVector position;
  G4Box *cell = dynamic_cast<G4Box *>(detector->GetCellLV()->GetSolid());

  if (aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume() == detector->GetCalorimeterLV())
  {
    position = aStep->GetPreStepPoint()->GetPosition();
    analysisManager->FillNtupleIColumn(1, 0, evt->GetEventID());
    analysisManager->FillNtupleDColumn(1, 1, position.x() / cm);
    analysisManager->FillNtupleDColumn(1, 2, position.y() / cm);
    analysisManager->FillNtupleDColumn(1, 3, position.z() / cm);
    analysisManager->FillNtupleDColumn(1, 4, aStep->GetTotalEnergyDeposit() / MeV);
    analysisManager->AddNtupleRow(1);
  }
  if (aStep->GetTrack()->GetParentID() == 0)
  {
    position = aStep->GetPreStepPoint()->GetPosition();
    analysisManager->FillNtupleIColumn(2, 0, evt->GetEventID());
    analysisManager->FillNtupleDColumn(2, 1, position.x() / cm);
    analysisManager->FillNtupleDColumn(2, 2, position.y() / cm);
    analysisManager->FillNtupleDColumn(2, 3, position.z() / cm);
    analysisManager->FillNtupleDColumn(2, 4, aStep->GetPreStepPoint()->GetGlobalTime());
    analysisManager->FillNtupleDColumn(2, 5, aStep->GetPreStepPoint()->GetKineticEnergy());
    analysisManager->AddNtupleRow(2);
    // G4cout << "Time: " << aStep->GetPreStepPoint()->GetGlobalTime() << "\t\t" << position.x() / cm << "\t\t" << position.y() / cm << "\t\t" << position.z() / cm << G4endl;
    // G4cout << "Kinetic energy : " << aStep->GetPreStepPoint()->GetKineticEnergy() << G4endl;
  }
}