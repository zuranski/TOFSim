name := tof
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib bin
	cp $(G4WORKDIR)/bin/$(G4SYSTEM)/$(name) $(G4WORKDIR)/


CPPFLAGS+= `root-config --cflags`
EXTRALIBS+= `root-config --glibs`
EXTRALIBS+=-L/usr/lib/x86_64-linux-gnu -lXi

include $(G4INSTALL)/config/binmake.gmk

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*
