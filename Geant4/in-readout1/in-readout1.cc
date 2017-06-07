/***********************************
 * beamtest.cc
 * by shanlimin 2005-10-19
 * *******************************/

// #ifdef G4MULTITHREADED
// #include "G4MTRunManager.hh"
// #else
#include "G4RunManager.hh"
// #endif

#include "G4UImanager.hh"
// Your choice of User Interface driver
#include "G4UIGAG.hh"
#include "G4UItcsh.hh"
#include "G4UIterminal.hh"

#include "myDetectorConstruction.hh"
#include "QGSP_BERT.hh"
#include <QGS_BIC.hh>
#include "myActionInitialization.hh"
#include "G4Threading.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif


/* This global file is used to store relevant data for
     analysis with external tools */
//using namespce

int main(int argc, char **argv)
{

  

  G4UIExecutive *ui = 0;
  if (argc == 1)
  {
    ui = new G4UIExecutive(argc, argv);
  }

  G4long seed = time(0);
  CLHEP::HepRandom::setTheSeed(seed);

  // Construct the default run manager
  // #ifdef G4MULTITHREADED
  //   G4MTRunManager *runManager = new G4MTRunManager;
  //   runManager->SetNumberOfThreads(G4Threading::G4GetNumberOfCores());
  // #else
  G4RunManager *runManager = new G4RunManager;
  // #endif

  // set mandatory initialization classes
  auto detector = new myDetectorConstruction();
  runManager->SetUserInitialization(detector);
  //runManager->SetUserInitialization(new myPhysicsList);
  runManager->SetUserInitialization(new QGSP_BERT);
  //  runManager->SetUserInitialization(new QGSP);

  runManager->SetUserInitialization(new myActionInitialization(detector));

  G4VisManager *visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager *UImanager = G4UImanager::GetUIpointer();

  runManager->Initialize();

  if (!ui)
  {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  }
  else
  {
    // interactive mode
    UImanager->ApplyCommand("/control/execute vis.mac");
    ui->SessionStart();
    delete ui;
    G4cout << "ui deleted" << G4endl;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  G4cout << "visManager deleted" << G4endl;
  delete runManager;
  G4cout << "RunManager deleted" << G4endl;
}
