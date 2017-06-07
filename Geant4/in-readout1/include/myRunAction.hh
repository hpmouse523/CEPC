
#ifndef myRunAction_h
#define myRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "myDetectorConstruction.hh"
//#include "G4SliceTimer.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;

class myRunAction : public G4UserRunAction
{
  public:
    myRunAction(const myDetectorConstruction* detector);
   ~myRunAction();

  public:
    virtual void BeginOfRunAction(const G4Run*) override;
    virtual void EndOfRunAction(const G4Run*) override;
  private:
  const myDetectorConstruction* detector;
    //G4SliceTimer* timer;
 
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

