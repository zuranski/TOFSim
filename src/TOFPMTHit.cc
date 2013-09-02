#include "TOFPMTHit.hh"

G4Allocator<TOFPMTHit> TOFPMTHitAllocator;

TOFPMTHit::TOFPMTHit(G4int i,G4double t, G4ThreeVector position, G4double energy, G4int refl, G4double angle)
{
  alpha = angle;
  ref = refl;
  e = energy;
  pos = position;
  id = i;
  time = t;
  pLogV = 0;
}

TOFPMTHit::~TOFPMTHit()
{;}

TOFPMTHit::TOFPMTHit(const TOFPMTHit &right)
    : G4VHit() {
  alpha = right.alpha;
  ref = right.ref;
  e = right.e;
  id = right.id;
  time = right.time;
  pos = right.pos;
  pLogV = right.pLogV;
}

const TOFPMTHit& TOFPMTHit::operator=(const TOFPMTHit &right)
{
  alpha = right.alpha;
  ref = right.ref;
  e = right.e;
  id = right.id;
  time = right.time;
  pos = right.pos;
  pLogV = right.pLogV;
  return *this;
}

int TOFPMTHit::operator==(const TOFPMTHit &/*right*/) const
{
  return 0;
}

void TOFPMTHit::Draw()
{
}

void TOFPMTHit::Print()
{
    G4cout << "  PMT[" << id << "] " << time/ns << " (nsec)" << " " << pos << " " << e <<   G4endl;
}



