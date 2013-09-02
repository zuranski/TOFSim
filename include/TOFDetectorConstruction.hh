#ifndef TOFDetectorConstruction_H
#define TOFDetectorConstruction_H 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4IntersectionSolid.hh"

#include "TOFPMTSD.hh"

class TOFDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
   TOFDetectorConstruction();
  ~TOFDetectorConstruction();

  //functions
  void SetDimensions();
  G4VPhysicalVolume* Construct();

  //get parameters

  private:

  void VisAttributes();  
  void DefineMaterials();
  void DefineSurfaces();
  void SurfaceProperties();
  G4VPhysicalVolume* ConstructDetector();

  //world volume
  G4Box* expHall_box;
  G4LogicalVolume* expHall_log;
  G4VPhysicalVolume* expHall_phys;

  //Materials&Elements
  G4Material* Air;
  G4Material* Sci;
  G4Material* Plexi;
  G4Material* Glass;
  G4Material* Al;
  G4Material* quartz;

  //properties tables
  G4MaterialPropertiesTable* myMPT1;
  G4MaterialPropertiesTable* myMPT2;
  G4MaterialPropertiesTable* myMPT3;
  G4MaterialPropertiesTable* myMPT4;
  G4MaterialPropertiesTable* myMPT5;

  //dimensions
  G4double pos1;
  G4double pos2;
  G4double sc_x;
  G4double sc_y;
  G4double sc_z;
  G4double tr_bot;
  G4double tr_up;
  G4double tr_h;
  G4double lg_h;
  G4double pmt_h;
  G4double gap_h;

  //volumes
  G4Box* housing_box;
  G4Box* scint_box;
  G4Trd* lg;
  G4Cons* lgstage1;
  G4Box* lgstage2;
  G4Tubs* pmt;
  G4Tubs* photocath;
  G4IntersectionSolid* lgbis2;
  G4Tubs* gap;

  //logical volumes
  G4LogicalVolume* scint_log;
  G4LogicalVolume* lg_log;
  G4LogicalVolume *lgbis_log;
  G4LogicalVolume *pmt_log;
  G4LogicalVolume *photocath_log;
  G4LogicalVolume * gap_log;

  //Physical volumes
  G4VPhysicalVolume* housing_phys;
  G4VPhysicalVolume* scint1_phys;
  G4VPhysicalVolume* scint2_phys;
  G4VPhysicalVolume* lg1_phys;
  G4VPhysicalVolume* lg2_phys;
  G4VPhysicalVolume* lg3_phys;
  G4VPhysicalVolume* lg4_phys;
  G4VPhysicalVolume* lg1bis_phys;
  G4VPhysicalVolume* lg2bis_phys;
  G4VPhysicalVolume* lg3bis_phys;
  G4VPhysicalVolume* lg4bis_phys;
  G4VPhysicalVolume* pmt1_phys;
  G4VPhysicalVolume* pmt2_phys;
  G4VPhysicalVolume* pmt3_phys;
  G4VPhysicalVolume* pmt4_phys;
  G4VPhysicalVolume* photocath1_phys;
  G4VPhysicalVolume* photocath2_phys;
  G4VPhysicalVolume* photocath3_phys;
  G4VPhysicalVolume* photocath4_phys;
  G4VPhysicalVolume* gap1_phys;
  G4VPhysicalVolume* gap2_phys;
  G4VPhysicalVolume* gap3_phys;
  G4VPhysicalVolume* gap4_phys;

  //optical surfaces
  G4OpticalSurface* OpSciSurface;
  G4OpticalSurface* CathSurface;

  //sensitive detectors
  static TOFPMTSD* pmtSD;
};

#endif
