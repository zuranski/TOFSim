#include "TOFPrimaryGeneratorAction.hh"
#include <assert.h>
#include "Randomize.hh"
#include <CLHEP/Random/RandGeneral.h>

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

TOFPrimaryGeneratorAction::TOFPrimaryGeneratorAction()
{
}

TOFPrimaryGeneratorAction::~TOFPrimaryGeneratorAction()
{
  delete particleGun;
}

void TOFPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

// create a particle beam
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);
  particleGun->SetParticleTime(0.0*ns); //set the initial time

//Cosmic Muon Flux (half mu+ half mu-, angular cos^2(theta), momentum - Kremer et al PRL 1999)

  double theta = cosmicAngle();
  double phi = 2.0*3.14159*G4UniformRand();
  double momentum = cosmicMomentum();
  double x = 15.*G4UniformRand() -7.5;
  double y = 15.*G4UniformRand() -7.5;

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

  if(G4UniformRand() < 0.5){
    G4ParticleDefinition* particle = particleTable->FindParticle("mu+");
    particleGun->SetParticleDefinition(particle);
  }
  else {
    G4ParticleDefinition* particle = particleTable->FindParticle("mu-");
    particleGun->SetParticleDefinition(particle);
  }

// test beam like particle

  particleGun->SetParticlePosition(G4ThreeVector(-12.05*cm,0*cm,0*cm));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
  particleGun->SetParticleMomentum(1.*GeV);


// cosmic like particle

/*
  particleGun->SetParticlePosition(G4ThreeVector(-5.05*cm,x*cm,y*cm));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(cos(theta),sin(theta)*cos(phi),sin(theta)*sin(phi))); //angle
  particleGun->SetParticleMomentum(momentum*120*GeV); //momentum
*/
  particleGun->GeneratePrimaryVertex(anEvent); //fire

}

double TOFPrimaryGeneratorAction::cosmicAngle()
{
  const int npdf=128;
  static double pdf[npdf];
  const double thetamax = 90.0*deg;
  static bool init = true;

  if(init){
    G4double dtheta = thetamax/npdf;
    for(int i=0;i<npdf;i++){
      double c = cos(dtheta*i);
      pdf[i] = c*c;
    }
  }

  CLHEP::RandGeneral generator(pdf,npdf);
  return generator.shoot()*thetamax;

}

double TOFPrimaryGeneratorAction::cosmicMomentum()
{
static double vals[] = {
		0.0,	0.0,	0.0,	0.0,
		0.2,	0.25,	14.0,	11.0,
		0.3,	0.35,	16.8,	13.6,
		0.4,	0.47,	17.2,	14.4,
		0.55,	0.62,	16.6,	13.5,
		0.70,	0.78,	15.6,	13.3,
		0.85,	0.92,	14.8,	12.1,
		1.0,	1.1,	13.0,	11.0,
		1.2,	1.3,	12.0,	10.1,
		1.4,	1.5,	10.2,	8.7,
		1.6,	1.84,	9.1,	7.3,
		2.1,	2.49,	6.6,	5.2,
		2.94,	3.49,	4.12,	3.38,
		4.12,	4.78,	2.53,	1.98,
		5.5,	6.21,	1.61,	1.25,
		7.0,	8.37,	0.90,	0.69,
		10.0,	12.42,	0.389,	0.309,
		15.5,	18.85,	0.138,	0.108,
		23.0,	26.68,	0.063,	0.046,
		31.1,	36.69,	0.028,	0.019,
		43.6,	51.47,	0.0099,	0.0071,
		61.1,	72.08,	0.0036,	0.0030,
		85.6,	100.96,	0.0014,	0.0012,
		120.0,	120.0,	0.0,	0.0}; // cutoff at 120 GeV/c
	const int nvals = sizeof(vals)/sizeof(vals[0]);
	const int nbins = nvals/4 - 1;
	const int npdf=256;
	static double pdf[npdf];
	static double pmax = vals[4*nbins];
        static bool init=true;

	if(init) {
		// RandGeneral needs equal-sized bins for pdf[]
		// it returns a value in the range [0,1)
		double hitsPerM2PerSecPerSterrad = 0.0;
		for(int i=0,ibin=0; i<npdf; ++i) {
			double p = (i+0.5)*pmax/npdf;
			while(p >= vals[4*ibin+5]) ++ibin;
			assert(ibin <= nbins);
			double f = (p - vals[4*ibin+1]) /
						(vals[4*ibin+5]-vals[4*ibin+1]);
			assert(0.0 <= f && f <= 1.0);
			pdf[i] = (1.0-f)*(vals[4*ibin+2]+vals[4*ibin+3]) +
					f*(vals[4*ibin+6]+vals[4*ibin+7]);
			hitsPerM2PerSecPerSterrad += pdf[i] * pmax/npdf;
		}
		init = false;
	}

	CLHEP::RandGeneral generator(pdf,npdf); // BUG in RandGeneral - cannot use new
	return generator.shoot();
}
