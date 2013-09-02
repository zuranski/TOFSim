//app headers
#include "TOFDetectorConstruction.hh"

//G4 headers
#include "G4SDManager.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ElementTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4Trd.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4IntersectionSolid.hh"

TOFPMTSD* TOFDetectorConstruction::pmtSD;

TOFDetectorConstruction::TOFDetectorConstruction(){

  SetDimensions();

}

TOFDetectorConstruction::~TOFDetectorConstruction()
{;}

G4VPhysicalVolume* TOFDetectorConstruction::Construct(){

  DefineMaterials();
  DefineSurfaces();
  return ConstructDetector();

}


void TOFDetectorConstruction::DefineMaterials(){

//general materials

  G4NistManager* manager = G4NistManager::Instance();
 
  G4Element* H = new G4Element("H", "H", 1., 1.01*g/mole);
  G4Element* C = new G4Element("C", "C", 6., 12.01*g/mole);

  Sci = manager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  Air = manager->FindOrBuildMaterial("G4_AIR");
  Plexi = manager->FindOrBuildMaterial("G4_PLEXIGLASS");
  Al = new G4Material("Al", 13., 26.98*g/mole, 2.7*g/cm3);  
  Glass = new G4Material("Glass", 1.032*g/cm3,2);
  Glass->AddElement(C,91.533*perCent);
  Glass->AddElement(H,8.467*perCent);      
  quartz = manager->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

//
// ------------ Generate & Add Material Properties Table ------------
//

  const G4int nEntries = 15;

  G4double PhotonEnergy[nEntries] =
            {1.9*eV, 2.06*eV, 2.25*eV, 2.43*eV, 2.53*eV, 2.64*eV, 2.75*eV, 2.88*eV, 3.02*eV, 3.18*eV, 3.35*eV, 3.54*eV, 4.13*eV, 4.96*eV, 6.2*eV};
//
// Sci
//

  G4double RefractiveIndex1[nEntries] = { 1.58, 1.58,  1.58, 1.58,  1.58, 1.58,  1.58, 1.58,  1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58};
  G4double Absorption1[nEntries] =  {110.*cm, 110.*cm, 110.*cm, 110.*cm, 110.*cm, 110.*cm, 110.*cm, 110.*cm, 110.*cm, 110.*cm, 110.*cm, 110.*cm, 110.*cm, 110.*cm, 110.*cm };
  G4double Scintillation[nEntries] = {0.,0.,0.,0.,0.001, 0.03, 0.075, 0.2, 0.5, 0.95, 0.5, 0.01,0.,0.,0. };

  myMPT1 = new G4MaterialPropertiesTable();

  myMPT1->AddProperty("RINDEX",       PhotonEnergy, RefractiveIndex1,nEntries);
  myMPT1->AddProperty("ABSLENGTH",    PhotonEnergy, Absorption1,     nEntries);
  myMPT1->AddProperty("FASTCOMPONENT", PhotonEnergy, Scintillation,  nEntries);

  myMPT1->AddConstProperty("SCINTILLATIONYIELD",10000./MeV);
  myMPT1->AddConstProperty("FASTTIMECONSTANT", 0.9*ns);
  myMPT1->AddConstProperty("RESOLUTIONSCALE",1.0);
  
  Sci->SetMaterialPropertiesTable(myMPT1);

//
// Air
//
  G4double RefractiveIndex2[nEntries] = {1.,1.,1.,1.,1.,1., 1., 1., 1., 1., 1., 1., 1., 1., 1.};

  myMPT2 = new G4MaterialPropertiesTable();
  myMPT2->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex2, nEntries);
  
  Air->SetMaterialPropertiesTable(myMPT2);

//
// Plexiglass
//
  G4double RefractiveIndex3[nEntries] = {1.51,1.51,1.51,1.51,1.51,1.51,1.51,1.51,1.51,1.51,1.51,1.51,1.51,1.51,1.51};
  G4double plexi_abslength[nEntries] = {210.*cm, 210.*cm, 210.*cm,210.*cm, 210.*cm, 210.*cm,210.*cm, 210.*cm, 210.*cm,210.*cm, 210.*cm, 210.*cm,21.*cm, 2.*cm, 2.*cm};
  myMPT3 = new G4MaterialPropertiesTable();
  myMPT3->AddProperty("RINDEX",PhotonEnergy,RefractiveIndex3, nEntries);
  myMPT3->AddProperty("ABSLENGTH",PhotonEnergy,plexi_abslength, nEntries);

  Plexi->SetMaterialPropertiesTable(myMPT3);

//
//Glass
//
  G4double RefractiveIndex4[nEntries] = {1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49};
  G4double glass_abslength[nEntries] = {420.*cm,420.*cm,420.*cm,420.*cm,420.*cm,420.*cm,420.*cm,420.*cm,420.*cm,420.*cm,420.*cm,420.*cm,420.*cm,420.*cm,420.*cm};

  myMPT4 = new G4MaterialPropertiesTable();
  myMPT4->AddProperty("RINDEX",PhotonEnergy,RefractiveIndex4,nEntries);
  myMPT4->AddProperty("ABSLENGTH",PhotonEnergy,glass_abslength,nEntries);

  Glass->SetMaterialPropertiesTable(myMPT4);

//
//Quartz
// 
  G4double RefractiveIndex5[nEntries] = {1.45,1.46,1.46,1.46,1.46,1.46,1.46,1.46,1.46,1.47,1.47,1.47,1.48,1.51,1.55};
  G4double Absorption5[nEntries] = {210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm };

  myMPT5 = new G4MaterialPropertiesTable();
  myMPT5->AddProperty("RINDEX",PhotonEnergy,RefractiveIndex5,nEntries);
  myMPT5->AddProperty("ABSLENGTH",PhotonEnergy,Absorption5,nEntries);

  quartz->SetMaterialPropertiesTable(myMPT5);

  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

}


void TOFDetectorConstruction::SetDimensions(){

//scint positions
  //pos1 = -2.505*cm;
  //pos2 = 2.505*cm;
  pos1 = -7.505*cm;
  pos2 = 7.505*cm;

//scintillators
  sc_x = 5*cm;
  sc_y = 5*cm;
  sc_z = 5*cm;

//trapezoid light guide
  tr_bot = 5*cm;
  tr_up = 1*cm;
  tr_h=5.95*cm;
//  tr_h=0*cm;
//gap (if exists)
  gap_h=0.5*mm;

//tube light guide
  lg_h = 2.61*cm;
//  lg_h=0*cm;

//pmt
  pmt_h = 0.0635*cm;

}

G4VPhysicalVolume* TOFDetectorConstruction::ConstructDetector(){

  G4double hall_x = 0.5*m;
  G4double hall_y = 1.3*m;
  G4double hall_z = 0.3*m;

//world

  expHall_box = new G4Box("expHall_box",hall_x,hall_y,hall_z);
  expHall_log = new G4LogicalVolume(expHall_box,Air,"expHall_log",0,0,0);
  expHall_phys = new G4PVPlacement(0,G4ThreeVector(),expHall_log,"expHall",0,false,0);

//rotations
  G4RotationMatrix* rot_down = new G4RotationMatrix(G4ThreeVector(1.,0.,0.),-90.*deg);
  G4RotationMatrix* rot_up = new G4RotationMatrix(G4ThreeVector(1.,0.,0.),90.*deg);

//scintillators or quartz bar

  scint_box = new G4Box("scint_box",sc_x/2,sc_y/2,sc_z/2);
  scint_log = new G4LogicalVolume(scint_box,Sci,"scint_log",0,0,0);
  scint1_phys = new G4PVPlacement(0,G4ThreeVector(pos1,0*m,0*m),scint_log,"scint",expHall_log,false,0);
  scint2_phys = new G4PVPlacement(0,G4ThreeVector(pos2,0*m,0*m),scint_log,"scint",expHall_log,false,0);

//light guides plexiglass

  lg = new G4Trd("lg", tr_bot/2, tr_up, tr_bot/2, tr_up, tr_h/2);

  lgstage1 = new G4Cons("lgstage1",0*cm, sqrt(2)*cm, 0*cm, tr_up,lg_h/2, 0., 360*deg );
  lgstage2 = new G4Box("lgstage2", tr_up, tr_up, lg_h/2);
  lgbis2 = new G4IntersectionSolid("lgbis2",lgstage1,lgstage2);  

  lgbis_log = new G4LogicalVolume(lgbis2,G4Material::GetMaterial("G4_PLEXIGLASS"),"lgbis_log",0,0,0);

  lg1bis_phys = new G4PVPlacement(rot_up,G4ThreeVector(pos1,sc_y/2+tr_h+lg_h/2,0*cm),lgbis_log,"lgbis",expHall_log,false,0);
  lg2bis_phys = new G4PVPlacement(rot_down,G4ThreeVector(pos1,-(sc_y/2+tr_h+lg_h/2),0*cm),lgbis_log,"lgbis",expHall_log,false,0);
  lg3bis_phys = new G4PVPlacement(rot_up,G4ThreeVector(pos2,sc_y/2+tr_h+lg_h/2,0*cm),lgbis_log,"lgbis",expHall_log,false,0);
  lg4bis_phys = new G4PVPlacement(rot_down,G4ThreeVector(pos2,-(sc_y/2+tr_h+lg_h/2),0*cm),lgbis_log,"lgbis",expHall_log,false,0);

  lg_log = new G4LogicalVolume(lg,G4Material::GetMaterial("G4_PLEXIGLASS"),"lg_log",0,0,0);

  lg1_phys = new G4PVPlacement(rot_up,G4ThreeVector(pos1 ,sc_y/2+tr_h/2,0*m),lg_log,"lg",expHall_log,false,0);
  lg2_phys = new G4PVPlacement(rot_down,G4ThreeVector(pos1, - (sc_y/2+tr_h/2),0*m),lg_log,"lg",expHall_log,false,0);
  lg3_phys = new G4PVPlacement(rot_up,G4ThreeVector(pos2 ,sc_y/2+tr_h/2,0*m),lg_log,"lg",expHall_log,false,0);
  lg4_phys = new G4PVPlacement(rot_down,G4ThreeVector(pos2,-(sc_y/2+tr_h/2),0*m),lg_log,"lg",expHall_log,false,0);


// gap (air or silicon grease)
  
  gap = new G4Tubs("airgap",0*cm,tr_up,gap_h/2,0*deg,360*deg);
  gap_log = new G4LogicalVolume(gap,G4Material::GetMaterial("G4_SILICON_DIOXIDE"),"airgap_log",0,0,0);

  G4double gaph = sc_y/2 + tr_h + lg_h + gap_h/2;
  gap1_phys = new G4PVPlacement(rot_up,G4ThreeVector(pos1,gaph,0*m),gap_log,"gap",expHall_log,false,0);
  gap2_phys = new G4PVPlacement(rot_down,G4ThreeVector(pos1,-gaph,0*m),gap_log,"gap",expHall_log,false,0);
  gap3_phys = new G4PVPlacement(rot_up,G4ThreeVector(pos2,gaph,0*m),gap_log,"gap",expHall_log,false,0);
  gap4_phys = new G4PVPlacement(rot_down,G4ThreeVector(pos2,-gaph,0*m),gap_log,"gap",expHall_log,false,0);

//window


  G4double winh = sc_y/2+tr_h+lg_h + gap_h + pmt_h/4;
  pmt = new G4Tubs("pmt",0*cm,tr_up,pmt_h/4,0*deg,360*deg);
  pmt_log = new G4LogicalVolume(pmt,G4Material::GetMaterial("Glass"),"pmt_log",0,0,0);
  pmt1_phys = new G4PVPlacement(rot_up,G4ThreeVector(pos1,winh,0*m),pmt_log,"pmt_phys",expHall_log,false,0);
  pmt2_phys = new G4PVPlacement(rot_down,G4ThreeVector(pos1,-winh,0*m),pmt_log,"pmt_phys",expHall_log,false,0);
  pmt3_phys = new G4PVPlacement(rot_up,G4ThreeVector(pos2,winh,0*m),pmt_log,"pmt_phys",expHall_log,false,0);
  pmt4_phys = new G4PVPlacement(rot_down,G4ThreeVector(pos2,-winh,0.*cm),pmt_log,"pmt_phys",expHall_log,false,0);


//pmts
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  pmtSD = new TOFPMTSD("/pmtSD");
  SDman->AddNewDetector(pmtSD);

  photocath = new G4Tubs("photocath", 0*cm, tr_up, pmt_h/4, 0*deg, 360*deg);
  photocath_log = new G4LogicalVolume(photocath,G4Material::GetMaterial("Al"),"photocath_log",0,0,0);
  photocath_log->SetSensitiveDetector(pmtSD);
  G4double cathh = winh + pmt_h/2;

  photocath1_phys = new G4PVPlacement(rot_up,G4ThreeVector(pos1,cathh,0*m),photocath_log,"photocath_phys",expHall_log,false,0);
  photocath2_phys = new G4PVPlacement(rot_down,G4ThreeVector(pos1,-cathh,0*m),photocath_log,"photocath_phys",expHall_log,false,0);
  photocath3_phys = new G4PVPlacement(rot_up,G4ThreeVector(pos2,cathh,0*m),photocath_log,"photocath_phys",expHall_log,false,0);
  photocath4_phys = new G4PVPlacement(rot_down,G4ThreeVector(pos2,-cathh,0*m),photocath_log,"photocath_phys",expHall_log,false,0);


  SurfaceProperties(); 
  VisAttributes();

  return expHall_phys;
}

void TOFDetectorConstruction::SurfaceProperties(){

  new G4LogicalBorderSurface("surf",scint1_phys,expHall_phys,OpSciSurface);
  new G4LogicalBorderSurface("surf",scint2_phys,expHall_phys,OpSciSurface);

  new G4LogicalBorderSurface("surf",lg1_phys,expHall_phys,OpSciSurface);
  new G4LogicalBorderSurface("surf",lg2_phys,expHall_phys,OpSciSurface);
  new G4LogicalBorderSurface("surf",lg3_phys,expHall_phys,OpSciSurface);
  new G4LogicalBorderSurface("surf",lg4_phys,expHall_phys,OpSciSurface);

  new G4LogicalBorderSurface("surf",lg1bis_phys,expHall_phys,OpSciSurface);
  new G4LogicalBorderSurface("surf",lg2bis_phys,expHall_phys,OpSciSurface);
  new G4LogicalBorderSurface("surf",lg3bis_phys,expHall_phys,OpSciSurface);
  new G4LogicalBorderSurface("surf",lg4bis_phys,expHall_phys,OpSciSurface);

  new G4LogicalBorderSurface("surf",pmt1_phys,photocath1_phys,CathSurface);
  new G4LogicalBorderSurface("surf",pmt2_phys,photocath2_phys,CathSurface);
  new G4LogicalBorderSurface("surf",pmt3_phys,photocath3_phys,CathSurface);
  new G4LogicalBorderSurface("surf",pmt4_phys,photocath4_phys,CathSurface);
}

void TOFDetectorConstruction::DefineSurfaces(){

  const G4int num =15;
  G4double Eph[num]={1.9*eV, 2.06*eV, 2.25*eV, 2.43*eV, 2.53*eV, 2.64*eV, 2.75*eV, 2.88*eV, 3.02*eV, 3.18*eV, 3.35*eV, 3.54*eV, 4.13*eV, 4.96*eV, 6.2*eV};

// window photocathode
//
  
  CathSurface = new G4OpticalSurface("CathSurface");
  CathSurface->SetFinish(polished);
  CathSurface->SetType(dielectric_metal);
  CathSurface->SetModel(unified);

  G4double ReflCath[num]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
  G4double EffCath[num]={0.,0.02,0.05,0.12,0.15,0.17,0.19,0.2,0.2,0.2,0.2,0.2,0.,0.,0.};
  for(int i=0;i<num;i++) EffCath[i]*=5.;
  
  G4MaterialPropertiesTable* CathSurfPT = new G4MaterialPropertiesTable();
  CathSurfPT->AddProperty("REFLECTIVITY",Eph,ReflCath,num);
  CathSurfPT->AddProperty("EFFICIENCY",Eph,EffCath,num);

  CathSurface->SetMaterialPropertiesTable(CathSurfPT);

//Wrapping surface

  const G4int n=2;
  G4double Ephot[n]={1.9*eV,6.2*eV};

  OpSciSurface = new G4OpticalSurface("OpSciSurface");
  OpSciSurface->SetModel(unified);
  OpSciSurface->SetFinish(groundfrontpainted);
  OpSciSurface->SetSigmaAlpha(0.3);

  G4MaterialPropertiesTable* wrapping = new G4MaterialPropertiesTable();
  G4double Refl[n]={.975,.975};
  G4double Eff[n]={0.,0.};
  G4double Rind[n]={1.,1.};
  wrapping->AddProperty("REFLECTIVITY",Ephot,Refl,n);
  wrapping->AddProperty("EFFICIENCY",Ephot,Eff,n);
  wrapping->AddProperty("RINDEX",Ephot,Rind,n);
  OpSciSurface->SetMaterialPropertiesTable(wrapping);  

}


void TOFDetectorConstruction::VisAttributes(){

  expHall_log->SetVisAttributes(G4VisAttributes::Invisible);

  G4VisAttributes* scintVis= new G4VisAttributes(G4Colour(0.9,0.9,0.9));
  scintVis->SetVisibility(true);
  scint_log->SetVisAttributes(scintVis);

  G4VisAttributes* lgVis = new G4VisAttributes(G4Colour(0.,0.,0.8));
  lgVis->SetVisibility(true);
  lg_log->SetVisAttributes(lgVis);
  lgbis_log->SetVisAttributes(lgVis);

  G4VisAttributes* winVis = new G4VisAttributes(G4Colour(0.8,0.,0.));
  winVis->SetVisibility(true);
  pmt_log->SetVisAttributes(winVis);

  G4VisAttributes* cathVis = new G4VisAttributes(G4Colour(0.,0.5,0.));
  cathVis->SetVisibility(true);
  photocath_log->SetVisAttributes(cathVis);

}
