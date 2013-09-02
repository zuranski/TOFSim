#include "G4ios.hh"
#include "G4Run.hh"
#include "G4Event.hh"

#include "TOFPMTHit.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include <string>

#include "Randomize.hh"
#include <CLHEP/Random/RandGeneral.h>

#include "TOFAnalysisManager.hh"


TOFAnalysisManager::TOFAnalysisManager(G4int num)
{
  char name[20];
  sprintf(name,"job%d.root",num);
  file = new TFile(name,"RECREATE");

  pmtree = new TTree("pmt","pmt");
  pmtree->Branch("posx",&x,"x/D");
  pmtree->Branch("posy",&y,"y/D");
  pmtree->Branch("posz",&z,"z/D");
  pmtree->Branch("time",&time,"time/D");
  pmtree->Branch("lambda",&lambda,"lambda/D");
  pmtree->Branch("npmt",&npmt,"npmt/I");
  pmtree->Branch("n",&n,"n/I");
  pmtree->Branch("refl",&refl,"refl/I");
  pmtree->Branch("angle",&angle,"angle/D");
  pmtree->Branch("energy",&energy,"energy/D");

  binning = 1000;
  initPMTs();

  final = new TTree("final","final");

// primary particle

  final->Branch("momentum",&momentum,"momentum/D");
  final->Branch("iangle",&iangle,"iangle/D");
  final->Branch("pos_x",&pos_x,"pos_x/D");
  final->Branch("pos_y",&pos_y,"pos_y/D");

//pulses

  final->Branch("hgain",&hgain,"hgain[4]/D");
  final->Branch("risetime",&risetime,"risetime[4]/D");
  final->Branch("t",&t,"t[4]/D");

//histograms for pulse creating
  pmt[0] = new TH1F("pmt1","pmt1",int(30*binning),10,40);
  pmt[1] = new TH1F("pmt2","pmt2",int(30*binning),10,40);
  pmt[2] = new TH1F("pmt3","pmt3",int(30*binning),10,40);
  pmt[3] = new TH1F("pmt4","pmt4",int(30*binning),10,40);


  cfd[0] = new TH1F("cfd1","cfd1",int(30*binning),0.,30);
  cfd[1] = new TH1F("cfd2","cfd2",int(30*binning),0.,30);
  cfd[2] = new TH1F("cfd3","cfd3",int(30*binning),0.,30);
  cfd[3] = new TH1F("cfd4","cfd4",int(30*binning),0.,30);

  avpulse[0] = new TH1F("avpuls1","avpulse1",int(30*binning),0.,30.);
  avpulse[0]->SetBit(TH1::kIsAverage);
  avpulse[1] = new TH1F("avpuls2","avpulse2",int(30*binning),0.,30.);
  avpulse[1]->SetBit(TH1::kIsAverage);
  avpulse[2] = new TH1F("avpuls3","avpulse3",int(30*binning),0.,30.);
  avpulse[2]->SetBit(TH1::kIsAverage);
  avpulse[3] = new TH1F("avpuls4","avpulse4",int(30*binning),0.,30.);
  avpulse[3]->SetBit(TH1::kIsAverage);

  cut[0] = new TH1F("cut1","cut1",int(30*binning),0.,30);
  cut[1] = new TH1F("cut2","cut2",int(30*binning),0.,30);
  cut[2] = new TH1F("cut3","cut3",int(30*binning),0.,30);
  cut[3] = new TH1F("cut4","cut4",int(30*binning),0.,30);

  for(int k=0;k<4;k++)
    nn[k]=0;
}

TOFAnalysisManager::~TOFAnalysisManager()
{
  file->Close();
}

void TOFAnalysisManager::BeginOfEvent(const G4Event*){

  G4String colName;
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  HHC1ID = SDman->GetCollectionID(colName="PMTColl");

}

void TOFAnalysisManager::EndOfEvent(const G4Event* aEvent){

  
  G4HCofThisEvent * HCE = aEvent->GetHCofThisEvent();
  TOFPMTHitsCollection* HHC1 = 0;

  if(HCE)
  {
    HHC1 = (TOFPMTHitsCollection*)(HCE->GetHC(HHC1ID));
  }

  for(int i=0;i<HHC1->entries();i++){
    TOFPMTHit* aHit = (*HHC1)[i];
    x = aHit->GetPos().x();
    y = aHit->GetPos().y();
    z = aHit->GetPos().z();
    time = aHit->GetTime();
    lambda = 1240/(aHit->GetE()*1e6);
    energy = aHit->GetE();
    angle = aHit->GetAlpha();
    if(x<0 && y>0) npmt=1;
    else if(x<0 && y<0) npmt=2;
    else if(x>0 && y>0) npmt=3;
    else if(x>0 && y<0) npmt=4;
    refl = aHit->GetRefl();


    double gaint=GetGain();    
    double timet=ftransit->GetRandom(); 
    int startbin = int ( (time+timet-ts-transit_t + 4)*binning );

    double ef = 1.;

    double eff_factor[4] = {ef*1.,ef*1.,ef*1.,ef*1.};


    if(startbin>0 && G4UniformRand()<eff_factor[npmt-1]){
        AddSignal(pmt[npmt-1], startbin, gaint);
    }
  
  }

//averaging as in the oscilloscope

  for(int k=0;k<4;k++){
    if(pmt[k]->GetMaximum()<tresh) continue;
    int trigger_i=0;

    for(int i=0; i<int(30*binning);i++){
      if(pmt[k]->GetBinContent(i)>tresh){
        trigger_i = i;
        break;
      }
    }  

    G4cout << k << " " << trigger_i << G4endl;

    if(trigger_i > 4000){
      for(int i=0;i<int(30*binning);i++){
        cut[k]->SetBinContent(i,pmt[k]->GetBinContent(trigger_i - 4000 + i));
      }
      avpulse[k]->Add(cut[k]);
      nn[k]++;
    } 
 
    cut[k]->Reset("ICEM");
  }



  double max[4]={0.,0.,0.,0.};
  double low_time[4]={0.,0.,0.,0.};
  double high_time[4]={0.,0.,0.,0.};

  for(int k=0;k<4;k++){

    max[k]=pmt[k]->GetMaximum();
    hgain[k]=max[k]/1000.;
    G4cout << k << " " << hgain[k] << G4endl;
   
 
    if(hgain[k]<treshold){
      for(int j=k;j<4;j++)
       pmt[j]->Reset("ICEM");
      return;
    }


//risetimes

    for(int i=0;i<int(30*binning);i++){
      if(pmt[k]->GetBinContent(i)>0.1*max[k]){
        low_time[k]=i/1000.;
        break;
      }
    }

    for(int i=0;i<int(30*binning);i++){
      if(pmt[k]->GetBinContent(i)>0.9*max[k]){
        high_time[k]=i/1000.;
        break;
      }
    }

    risetime[k]=high_time[k]-low_time[k];

//cfd pulses

    for(int i=0;i<int(30*binning);i++){
      if(i>1500){
        cfd[k]->SetBinContent(i,pmt[k]->GetBinContent(i-2000) - pmt[k]->GetBinContent(i)/5.);
      } 
      else{
        cfd[k]->SetBinContent(i,-pmt[k]->GetBinContent(i)/5.);
      }
    }

//time measurment (cfd)
/*
    for(int i=0;i<int(30*binning);i++){
      if(cfd[k]->GetBinContent(i)>0.1){
        t[k] = i/1000.;
        break;
      }
    }
*/

    for(int i=0;i<int(30*binning);i++){
      if(pmt[k]->GetBinContent(i) > (treshold*1000.)){
        t[k]=i/1000.;
        break;
      }
    }
 
    pmt[k]->Reset("ICEM");
    cfd[k]->Reset("ICEM");

  }

  pos_x=aEvent->GetPrimaryVertex()->GetPosition().y();
  pos_y=aEvent->GetPrimaryVertex()->GetPosition().z(); 

  momentum = 0.001*aEvent->GetPrimaryVertex()->GetPrimary()->GetMomentum().mag();
  iangle = aEvent->GetPrimaryVertex()->GetPrimary()->GetMomentum().angle(G4ThreeVector(1.,0.,0.))*180./3.14159;

  
  final->Fill();


}

void TOFAnalysisManager::BeginOfRun(const G4Run*){
}

void TOFAnalysisManager::EndOfRun(const G4Run*){

  TF1* uni = new TF1("uni","1",0,30);

  for(int k=0;k<4;k++){
    avpulse[k]->Divide(uni,nn[k]);
    avpulse[k]->Write();
    G4cout << nn[k] << G4endl;
  }
  
  //pmtree->Write();
  final->Write();
  //file->Close();
}

void TOFAnalysisManager::AddSignal(TH1F* hist, int st, double g){

  for(int i=0;i<4*binning;i++){
    double x = (double) i/binning;
    if(st+i<30*binning)
      hist->AddBinContent(st + i, g*h(x));
    else break;
  }

}

double TOFAnalysisManager::GetGain(){

  gain_sigma = gain/sqrt((fdyn1->GetRandom()));
  fgain->SetParameters(1/(gain_sigma*2.506628275),gain,gain_sigma);

  return ( (fgain->GetRandom())*conv );

}


void TOFAnalysisManager::initPMTs(){

  conv = 1.6e-7*50.;
  gain = 2000000;
  gain_sigma = 0.;
  transit_t = 10.;
  tts = 0.16;
  transit_sigma = tts/2.35;
  ts = 1.53988; 

  ftransit = new TF1("ftransit","gaus(0)",9.4,10.6);
  ftransit->SetParameters(1/(transit_sigma*2.506628275),transit_t,transit_sigma);

  fgain = new TF1("fgain","gaus(0)",0.,10e+7);

  fdyn1 = new TF1("fdyn1","TMath::Poisson(x,[0])",0,30);
  fdyn1->SetParameter(0,8.);

  
  tresh = 400.;
  treshold = 0.1;
}

double TOFAnalysisManager::h(double t){

  return pow(t,2)*exp(-pow(t,2)/pow(ts,2))/1.617985;

}

