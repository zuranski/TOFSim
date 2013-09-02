#ifndef TOFAnalysisManager_h
#define TOFAnalysisManager_h 1

#include "globals.hh"
#include <vector>
#include "G4ThreeVector.hh"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TF1.h"
#include "TRandom.h"
#include "TH2F.h"

class G4Run;
class G4Event;

class TOFAnalysisManager {
public:

  TOFAnalysisManager(G4int num);
  virtual ~TOFAnalysisManager();


public:
  virtual void BeginOfRun(const G4Run*);
  virtual void EndOfRun(const G4Run*);
  virtual void BeginOfEvent(const G4Event*);
  virtual void EndOfEvent(const G4Event*);


private:

  int HHC1ID;
  int HHC2ID;

  TFile* file;
  TTree* pmtree;
  TTree* final;

//photon variables
  double x;
  double y;
  double z;
  double time;
  double lambda;
  double energy;
  double angle;
  int npmt;
  int n;
  int refl;

//functions
   void AddSignal(TH1F* hist, int st, double g);
   void initPMTs();
   double GetGain();
   double binning;
   double h(double t);

//pmt variables
   double gain;
   double gain_sigma;
   double transit_t;
   double tts;
   double ts;
   double transit_sigma;
   double conv;

//for ntuple
   double t[4];
   double treshold;
   double tresh;
   double delta_t;
   double momentum;
   double iangle;
   double hgain[4];
   double risetime[4];
   double pos_x;
   double pos_y;
   int nn[4];
   TF1* ftransit;
   TF1* fgain;
   TH1F* pmt[4];
   TF1* fdyn1;
   TH1F* cfd[4];
   TH1F* avpulse[4];
   TH1F* cut[4];
};

#endif

