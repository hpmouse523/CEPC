//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef mySteppingAction_h
#define mySteppingAction_h 1

#include "G4UserSteppingAction.hh"
class myDetectorConstruction;
class myEventAction;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class mySteppingAction : public G4UserSteppingAction
{
  public:
    mySteppingAction(myDetectorConstruction*, myEventAction*);
    mySteppingAction();
    virtual ~mySteppingAction();
    virtual void UserSteppingAction(const G4Step*) override;
  private:
    myDetectorConstruction* detector;
    myEventAction*          eventaction;  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
