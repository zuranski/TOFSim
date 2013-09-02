#ifndef TOFPMTSD_h
#define TOFPMTSD_h 1

#include "G4VSensitiveDetector.hh"
#include "TOFPMTHit.hh"
class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class TOFPMTSD : public G4VSensitiveDetector
{

  public:
      TOFPMTSD(G4String name);
     ~TOFPMTSD();

      void Initialize(G4HCofThisEvent*HCE);
      G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
      G4bool ProcessHits_constStep(const G4Step* aStep, G4TouchableHistory* ROhist);
      void EndOfEvent(G4HCofThisEvent*HCE);

  private:
      TOFPMTHitsCollection* hitsCollection;
      G4int HCID;
};

#endif

