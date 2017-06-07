#include "myRunAction.hh"

#include <string>
using std::string;

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "myAnalysis.hh"
#include "myDetectorConstruction.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4ThreeVector.hh"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
myRunAction::myRunAction(const myDetectorConstruction *det) : G4UserRunAction()
{
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
  detector = det;

  //Create directories
  analysisManager->SetHistoDirectoryName("histo");
  analysisManager->SetNtupleDirectoryName("ntuple");

  //Open an output file
  if (!analysisManager->OpenFile("RunData"))
  {
    G4cout << "Can't open file RunData" << G4endl;
    exit(1);
  }
  G4ThreeVector layerSize = detector->GetLayerSize();
  //Create example graphic of detector
  analysisManager->CreateH3("Detection example",
                            "Detection example",
                            myDetectorConstruction::numOfCalX, -(layerSize.x() / 2) / cm, (layerSize.x() / 2) / cm,
                            myDetectorConstruction::numOfCalY, -(layerSize.y() / 2) / cm, (layerSize.y() / 2) / cm,
                            myDetectorConstruction::numOfLayers, 0, layerSize.z() / cm);

  //Create ntuple for primary data
  analysisManager->CreateNtuple("PrimaryData", "data");
  analysisManager->CreateNtupleIColumn("EventNum");
  analysisManager->CreateNtupleDColumn("PrimaryVertexX");
  analysisManager->CreateNtupleDColumn("PrimaryVertexY");
  analysisManager->CreateNtupleDColumn("PrimaryVertexZ");
  analysisManager->CreateNtupleDColumn("PrimaryDirectionX");
  analysisManager->CreateNtupleDColumn("PrimaryDirectionY");
  analysisManager->CreateNtupleDColumn("PrimaryDirectionZ");
  analysisManager->CreateNtupleDColumn("PrimaryEnergy");
  analysisManager->FinishNtuple();

  //Create ntuple for stepping data
  analysisManager->CreateNtuple("StepData", "data");
  analysisManager->CreateNtupleIColumn("EventNum");
  analysisManager->CreateNtupleDColumn("PositionX");
  analysisManager->CreateNtupleDColumn("PositionY");
  analysisManager->CreateNtupleDColumn("PositionZ");
  analysisManager->CreateNtupleDColumn("Energy");
  analysisManager->FinishNtuple();

  //Create btuple for primary particle track
  analysisManager->CreateNtuple("PrimaryParticleTrack", "data");
  analysisManager->CreateNtupleIColumn("EventNum");
  analysisManager->CreateNtupleDColumn("PositionX");
  analysisManager->CreateNtupleDColumn("PositionY");
  analysisManager->CreateNtupleDColumn("PositionZ");
  analysisManager->CreateNtupleDColumn("GlobalTime");
  analysisManager->CreateNtupleDColumn("KineticEnergy");
  analysisManager->FinishNtuple();

  G4cout << "myRunAction constructed" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myRunAction::~myRunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//when beamOn invoke
void myRunAction::BeginOfRunAction(const G4Run *aRun)
{
  // G4cout << "myRunAction begins" << G4endl;
  // auto analysisManager = G4AnalysisManager::Instance();
  /**********************************************************************************************************/

  /*********************************************************************************************************/

  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
}

void myRunAction::EndOfRunAction(const G4Run *aRun)
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
