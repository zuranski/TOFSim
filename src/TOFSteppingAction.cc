#include "TOFSteppingAction.hh"
#include "TOFPMTSD.hh"
#include "TOFUserTrackInformation.hh"
#include "TOFTrackingAction.hh"

#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4ParticleDefinition.hh"
#include "G4OpBoundaryProcess.hh"

TOFSteppingAction::TOFSteppingAction()
{
}

TOFSteppingAction::~TOFSteppingAction()
{
}

void TOFSteppingAction::UserSteppingAction(const G4Step* theStep)
{
  G4Track* theTrack = theStep->GetTrack();
  G4StepPoint* thePostPoint = theStep->GetPostStepPoint();
  G4OpBoundaryProcessStatus boundaryStatus=Undefined;
  static G4OpBoundaryProcess* boundary=NULL;

  TOFUserTrackInformation* trackInfo = (TOFUserTrackInformation*)theTrack->GetUserInformation();

  G4ParticleDefinition* particleType = theTrack->GetDefinition();
  if(particleType==G4OpticalPhoton::OpticalPhotonDefinition()){
    if(trackInfo->GetTrackStatus()==notchecked){
      if(G4UniformRand()>(0.45*0.2)){
        theTrack->SetTrackStatus(fStopAndKill);
      }
      trackInfo->SetTrackStatus(checked);
    }

  }

  //find the boundary process only once
  if(!boundary){
      G4ProcessManager* pm = theStep->GetTrack()->GetDefinition()->GetProcessManager();
      G4int nprocesses = pm->GetProcessListLength();
      G4ProcessVector* pv = pm->GetProcessList();
      G4int i;
      for( i=0;i<nprocesses;i++){
        if((*pv)[i]->GetProcessName()=="OpBoundary"){
          boundary = (G4OpBoundaryProcess*)(*pv)[i];
          break;
        }
     }
  }


  if(particleType==G4OpticalPhoton::OpticalPhotonDefinition()){


    boundaryStatus=boundary->GetStatus();
    if(thePostPoint->GetStepStatus()==fGeomBoundary){
      //Detection
      if(boundaryStatus==Detection){
        G4SDManager* SDman = G4SDManager::GetSDMpointer();
        G4String sdName="/pmtSD";
        TOFPMTSD* pmtSD = (TOFPMTSD*)SDman->FindSensitiveDetector(sdName);
        if(pmtSD){
          pmtSD->ProcessHits_constStep(theStep,NULL);
        }
      }
      
    }
  }

}

