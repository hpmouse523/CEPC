
name := in-readout1
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

#include hadronic_list.gmk

.PHONY: all
all:   lib bin

had_list:
	@echo "This example uses the QGSP 'educated guess physics list'"
	@echo "(see $(G4INSTALL)/physics_lists/hadronic)."
	@echo "The hadronic physics lists should be compiled before"
	@echo "compiling this example."

include $(G4INSTALL)/config/binmake.gmk

########################### ROOT #################################
 
ifdef ROOTSYS
ifndef G4UI_USE_ROOT
  ROOTCPPFLAGS   = $(shell $(ROOTSYS)/bin/root-config --cflags)
  #ROOTCPPFLAGS  := $(filter-out -D_REENTRANT,$(ROOTCPPFLAGS))
  #ROOTCPPFLAGS  := $(filter-out -pthread,$(ROOTCPPFLAGS))
  CPPFLAGS      += -DG4ANALYSIS_USE_ROOT $(ROOTCPPFLAGS)
  ROOTLIBS       = $(shell $(ROOTSYS)/bin/root-config --nonew --glibs)
  ROOTLIBS      := $(filter-out -lNew,$(ROOTLIBS))
  #ROOTLIBS      := $(filter-out -lThread,$(ROOTLIBS))
  #ROOTLIBS      := $(filter-out -lpthread,$(ROOTLIBS))
  #ROOTLIBS      := $(filter-out -pthread,$(ROOTLIBS))
  ROOTLIBS      += -lMinuit -lHtml
  LDLIBS        += $(ROOTLIBS)
endif
endif
