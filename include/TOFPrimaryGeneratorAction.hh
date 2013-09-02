#ifndef TOFPrimaryGeneratorAction_h
#define TOFPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;

class TOFPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    TOFPrimaryGeneratorAction();
   ~TOFPrimaryGeneratorAction();



  public:
    void GeneratePrimaries(G4Event*);
    double cosmicAngle(); 
    double cosmicMomentum();

  private:
    G4ParticleGun* particleGun;
};


#endif 
