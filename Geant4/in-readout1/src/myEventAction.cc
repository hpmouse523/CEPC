
#include "myEventAction.hh"

#include "G4Event.hh"
#include "myDetectorConstruction.hh"
#include "myAnalysis.hh"
#include "G4TrajectoryContainer.hh"

#include "G4SystemOfUnits.hh"

using namespace std;

myEventAction::myEventAction()
    : G4UserEventAction()
{
  G4cout<<"Finish construct event"<<G4endl;
}

myEventAction::~myEventAction()
{
  G4cout<<"Finish DEconstruct event"<<G4endl;
}

void myEventAction::BeginOfEventAction(const G4Event *event)
{
  G4cout << "EventID:" << event->GetEventID() << G4endl;
  auto analysisManager = G4AnalysisManager::Instance();

  if (event->GetNumberOfPrimaryVertex() != 1)
  {
    G4cout << "There are " << event->GetNumberOfPrimaryVertex() << " primary vertex in this event" << G4endl;
  }
  auto primaryVertex = event->GetPrimaryVertex();
  if (primaryVertex->GetNumberOfParticle() != 1)
  {
    G4cout << "There are " << primaryVertex->GetNumberOfParticle() << " primary vertex in this event" << G4endl;
  }
  auto primaryParticle=primaryVertex->GetPrimary();
  auto momentumDirection=primaryParticle->GetMomentumDirection();
  analysisManager->FillNtupleIColumn(0,0,event->GetEventID());
  analysisManager->FillNtupleDColumn(0,1,primaryVertex->GetX0()/cm);
  analysisManager->FillNtupleDColumn(0,2,primaryVertex->GetY0()/cm);
  analysisManager->FillNtupleDColumn(0,3,primaryVertex->GetZ0()/cm);
  analysisManager->FillNtupleDColumn(0,4,momentumDirection.x());
  analysisManager->FillNtupleDColumn(0,5,momentumDirection.y());
  analysisManager->FillNtupleDColumn(0,6,momentumDirection.z());
  analysisManager->FillNtupleDColumn(0,7,primaryParticle->GetTotalEnergy()/MeV);
  analysisManager->AddNtupleRow(0);
  // G4cout<<"Finish filling primary data"<<G4endl;

}

void myEventAction::EndOfEventAction(const G4Event *evt)
{
}
