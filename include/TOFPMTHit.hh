#ifndef TOFPMTHit_h
#define TOFPMTHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"


class TOFPMTHit : public G4VHit
{
  public:

      TOFPMTHit(G4int i,G4double t,G4ThreeVector position, G4double energy, G4int refl, G4double angle);
      virtual ~TOFPMTHit();
      TOFPMTHit(const TOFPMTHit &right);
      const TOFPMTHit& operator=(const TOFPMTHit &right);
      int operator==(const TOFPMTHit &right) const;


      inline void *operator new(size_t);
      inline void operator delete(void*aHit);

      void Draw();
      void Print();

  private:
      G4double alpha;
      G4int ref;
      G4double e;
      G4int id;
      G4double time;
      G4ThreeVector pos;
      const G4LogicalVolume* pLogV;

  public:
      inline G4double GetE() const {return e;}
      inline G4int GetID() const { return id; }
      inline G4double GetTime() const { return time; }
      inline void SetTime(G4double val) { time = val; }
      inline void SetPos(G4ThreeVector xyz) { pos = xyz; }
      inline G4ThreeVector GetPos() const { return pos; }
      inline void SetLogV(G4LogicalVolume* val) { pLogV = val; }
      inline const G4LogicalVolume* GetLogV() const { return pLogV; }
      inline G4int GetRefl() const {return ref;}
      inline G4double GetAlpha() const {return alpha;}
};

typedef G4THitsCollection<TOFPMTHit> TOFPMTHitsCollection;

extern G4Allocator<TOFPMTHit> TOFPMTHitAllocator;

inline void* TOFPMTHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*)TOFPMTHitAllocator.MallocSingle();
  return aHit;
}

inline void TOFPMTHit::operator delete(void*aHit)
{
  TOFPMTHitAllocator.FreeSingle((TOFPMTHit*) aHit);
}

#endif


