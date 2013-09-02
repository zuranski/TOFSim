#include "TOFEventAction.hh"
#include "TOFAnalysisManager.hh"

#include "G4Event.hh"
#include "G4ios.hh"

TOFEventAction::TOFEventAction(TOFAnalysisManager* aAnalysisManager)
:fAnalysisManager(aAnalysisManager)
{
}

TOFEventAction::~TOFEventAction()
{
}

void TOFEventAction::BeginOfEventAction(const G4Event* aEvent)
{
  
  if(fAnalysisManager) fAnalysisManager->BeginOfEvent(aEvent);
  

}

void TOFEventAction::EndOfEventAction(const G4Event* aEvent)
{

  if(fAnalysisManager) fAnalysisManager->EndOfEvent(aEvent);
  G4cout << "Analyzed " << aEvent->GetEventID() << " event" << G4endl;
}

