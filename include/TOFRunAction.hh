#ifndef TOFRunAction_h
#define TOFRunAction_h

#include "G4UserRunAction.hh"

class TOFAnalysisManager;

class TOFRunAction : public G4UserRunAction {

  public: 
    TOFRunAction(TOFAnalysisManager* = 0);
    ~TOFRunAction();
  public:
    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);
  private:
    TOFAnalysisManager* fAnalysisManager;
};

#endif
