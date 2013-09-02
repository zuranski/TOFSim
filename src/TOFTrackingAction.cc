#include "TOFTrackingAction.hh"
#include "TOFUserTrackInformation.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

#include "Randomize.hh"
#include <CLHEP/Random/RandGeneral.h>

TOFTrackingAction::TOFTrackingAction()
{}

void TOFTrackingAction::PreUserTrackingAction(const G4Track*)
{

  fpTrackingManager->SetUserTrackInformation(new TOFUserTrackInformation);

}


void TOFTrackingAction::PostUserTrackingAction(const G4Track* ){ 

}
















