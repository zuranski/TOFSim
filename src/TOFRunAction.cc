#include "TOFAnalysisManager.hh"

#include "TOFRunAction.hh"

TOFRunAction::TOFRunAction(TOFAnalysisManager* aAnalysisManager)
:fAnalysisManager(aAnalysisManager){}

TOFRunAction::~TOFRunAction(){}

void TOFRunAction::BeginOfRunAction(const G4Run* aRun){
  if(fAnalysisManager) fAnalysisManager->BeginOfRun(aRun);
}

void TOFRunAction::EndOfRunAction(const G4Run* aRun){
  if(fAnalysisManager) fAnalysisManager->EndOfRun(aRun);
}

