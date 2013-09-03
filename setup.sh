# ROOT is required with root-config command available in PATH

# Geant4 is required as it's a geant4 simulation
source /home/zuranski/soft/geant4.9.6-install/bin/geant4.sh
# enables old-style compilation with UNIX make instead of cmake
source /home/zuranski/soft/geant4.9.6-install/share/Geant4-9.6.2/geant4make/geant4make.sh
export G4WORKDIR=`pwd` 
