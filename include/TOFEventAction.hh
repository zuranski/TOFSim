#ifndef TOFEventAction_H
#define TOFEventAction_H 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class TOFAnalysisManager;

class TOFEventAction : public G4UserEventAction
{
  public:
    TOFEventAction(TOFAnalysisManager* = 0);
    virtual ~TOFEventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

  private:
    TOFAnalysisManager* fAnalysisManager;
   
    int nn;

};

#endif

