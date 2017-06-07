#include "myActionInitialization.hh"

#include "myRunAction.hh"
#include "myEventAction.hh"
#include "mySteppingAction.hh"
#include "myPrimaryGeneratorAction.hh"

myActionInitialization::myActionInitialization( myDetectorConstruction *dec)
    : G4VUserActionInitialization()
{
    detector = dec;
}

myActionInitialization::~myActionInitialization() {}

void myActionInitialization::BuildForMaster() const
{
    SetUserAction(new myRunAction(detector));
}

void myActionInitialization::Build() const
{
    SetUserAction(new myPrimaryGeneratorAction(detector));
    SetUserAction(new myRunAction(detector));
    SetUserAction(new myEventAction);
    SetUserAction(new mySteppingAction(detector, nullptr));
}