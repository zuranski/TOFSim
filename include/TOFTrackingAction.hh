#ifndef TOFTrackingAction_h
#define TOFTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class TOFTrackingAction : public G4UserTrackingAction {

public:  
  TOFTrackingAction();
  ~TOFTrackingAction() {};
  
  void PreUserTrackingAction(const G4Track*); 
  void PostUserTrackingAction(const G4Track*);
 
};

#endif
