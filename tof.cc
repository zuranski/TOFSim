#include "G4RunManager.hh"

#include "TOFDetectorConstruction.hh"
#include "TOFPhysicsList.hh"
#include "TOFPrimaryGeneratorAction.hh"
#include "TOFSteppingAction.hh"
#include "TOFEventAction.hh"
#include "TOFAnalysisManager.hh"
#include "TOFRunAction.hh"
#include "TOFTrackingAction.hh"


#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "G4UImanager.hh"
#include "G4UItcsh.hh"
#include "G4UIterminal.hh"


int main(int argc, char *argv[]){

  if (argc==1) {G4cout << "Usage: run seed vis.macro" << G4endl; return 0;}

  int n = atoi(argv[1]);
  G4long myseed = 34517*n -12*n*n; //time doesn't really work for batch
  CLHEP::HepRandom::setTheSeed(myseed);

  G4RunManager* runManager = new G4RunManager;


  runManager->SetUserInitialization(new TOFDetectorConstruction);
  runManager->SetUserInitialization(new TOFPhysicsList);

  TOFAnalysisManager* analysisManager = new TOFAnalysisManager(n);

  runManager->SetUserAction(new TOFPrimaryGeneratorAction);
  runManager->SetUserAction(new TOFEventAction(analysisManager));
  runManager->SetUserAction(new TOFRunAction(analysisManager));
  runManager->SetUserAction(new TOFSteppingAction());
  runManager->SetUserAction(new TOFTrackingAction());  
  runManager->Initialize();

  runManager->BeamOn(3);

  #ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  G4UImanager* UI = G4UImanager::GetUIpointer();
  
  G4UIsession* session =0;
  session = new G4UIterminal();
  G4String command = "/control/execute ";
  G4String fileName = argv[2];
  UI->ApplyCommand(command+fileName);
  session->SessionStart();

  delete visManager;
  #endif


  delete runManager;
  return 0;

}

