#ifndef TOFSteppingAction_H
#define TOFSteppingAction_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

class TOFEventAction;

class TOFSteppingAction : public G4UserSteppingAction
{
  public:
     TOFSteppingAction();
    ~TOFSteppingAction();
     virtual void UserSteppingAction(const G4Step*);

  private:
};

#endif
