
#ifndef myEventAction_h
#define myEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4Event;

class myEventAction : public G4UserEventAction
{
  public:
    myEventAction();
    virtual ~myEventAction();

  public:
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

  private:
    // G4int ECHCID;
    // G4String drawFlag;
    // G4int CsICollID;
     //G4double Energy[300];
  // public:
  //   inline void SetDrawFlag(G4bool val)
  //   { drawFlag = val; };
};

#endif

    
