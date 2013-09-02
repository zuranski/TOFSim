#include "G4VUserTrackInformation.hh"
#include "globals.hh"

#ifndef TOFUserTrackInformation_h
#define TOFUserTrackInformation_h 1

enum TOFTrackStatus {checked=1,notchecked=0};

class TOFUserTrackInformation : public G4VUserTrackInformation
{
public:
  TOFUserTrackInformation();
  ~TOFUserTrackInformation();
  
  void IncReflections(){reflections++;}
  G4int GetReflectionCount()const {return reflections;}
  int GetTrackStatus() const {return status; }
  void SetTrackStatus(int s) {status=s;}

  inline void Print()const{};
private:
  G4int reflections;
  int status;
};

#endif
