#ifndef myActionInitialization_h
#define myActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "myDetectorConstruction.hh"

class myActionInitialization
    : public G4VUserActionInitialization
{
  public:
    myActionInitialization( myDetectorConstruction *);
    virtual ~myActionInitialization();

  public:
    virtual void BuildForMaster() const override;
    virtual void Build() const override;
    private:
    myDetectorConstruction *detector;
};
#endif