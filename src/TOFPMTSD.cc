#include "TOFPMTSD.hh"
#include "TOFPMTHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4OpticalPhoton.hh"

TOFPMTSD::TOFPMTSD(G4String name)
:G4VSensitiveDetector(name)
{
  G4String HCname;
  collectionName.insert(HCname="PMTColl");
  HCID = -1;
}

TOFPMTSD::~TOFPMTSD(){;}

void TOFPMTSD::Initialize(G4HCofThisEvent*HCE)
{
  hitsCollection = new TOFPMTHitsCollection
                   (SensitiveDetectorName,collectionName[0]);
  if(HCID<0)
  { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(hitsCollection); }
  HCE->AddHitsCollection(HCID,hitsCollection);
}


G4bool TOFPMTSD::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist){
  return ProcessHits_constStep(aStep,ROhist);
}

G4bool TOFPMTSD::ProcessHits_constStep(const G4Step* aStep,G4TouchableHistory* /*ROhist*/)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep==0.) {return true;}
  G4Track* theTrack = aStep->GetTrack();
  G4ThreeVector dir = theTrack->GetMomentumDirection();

if(theTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition()){

  G4StepPoint* postStepPoint = aStep->GetPostStepPoint();
  G4TouchableHistory* theTouchable = (G4TouchableHistory*)(postStepPoint->GetTouchable());
  G4int copyNo = theTouchable->GetVolume()->GetCopyNo();
  G4double hitTime = postStepPoint->GetGlobalTime();
  G4ThreeVector hitPos = postStepPoint->GetPosition();

  G4double alpha=0.;
  if(hitPos.y()>0) alpha = dir.angle(G4ThreeVector(0.,1.,0.))*180/3.141593;
  if(hitPos.y()<0) alpha = dir.angle(G4ThreeVector(0.,-1.,0.))*180/3.141593;

  TOFPMTHit* aHit = new TOFPMTHit(copyNo,hitTime,hitPos,edep,-1,alpha);
  G4VPhysicalVolume* thePhysical = theTouchable->GetVolume();
  aHit->SetLogV(thePhysical->GetLogicalVolume());
  hitsCollection->insert( aHit );
}

  return true;
}

void TOFPMTSD::EndOfEvent(G4HCofThisEvent* /*HCE*/)
{;}

