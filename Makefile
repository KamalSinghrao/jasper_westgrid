#ROOT
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs) -lm -ldl
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs) -lm -ldl

#gcc
ARCH          = -m64
OPT           = -g -march=core2 -mfpmath=sse -fopenmp
CXX           = g++
CXXFLAGS      = -Wall $(OPT) $(ARCH) -fPIC
INCS          = -I$(ROOTSYS)/include -I$(RATROOT)/include/
LD            = g++ -fPIC
LDFLAGS       = $(OPT) $(ARCH)
SOFLAGS       = -Wl,-soname -shared 
LIB_EXTRA     = /home/singhrao/Documents/RAT/ratcage/rat/lib/librat.a -Llib -L/home/singhrao/Documents/RAT/ratcage/rat/lib -L/home/singhrao/Documents/RAT/ratcage/root-5.34.12/lib -L/home/singhrao/Documents/RAT/ratcage/local/lib64 -L/usr/kerberos/lib64 -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lm -lMinuit2 -lSpectrum -lRooFitCore -lRooFit -lFoam -lMinuit -lPyROOT -lG4Tree -lG4FR -lG4GMocren -lG4visHepRep -lG4RayTracer -lG4VRML -lG4vis_management -lG4modeling -lG4interfaces -lG4persistency -lG4analysis -lG4error_propagation -lG4readout -lG4physicslists -lG4run -lG4event -lG4tracking -lG4parmodels -lG4processes -lG4digits_hits -lG4track -lG4particles -lG4geometry -lG4materials -lG4graphics_reps -lG4intercoms -lG4global -lG4clhep -lcurl -ldl -lgssapi_krb5 -lkrb5 -lk5crypto -lcom_err -lidn -lssl -lcrypto -lz -lbz2
LIBS          = $(ROOTGLIBS) $(LIB_EXTRA) -L/usr/X11/lib -lXpm -lX11

exe:	db_fit.exe

db_fit.exe: db_fit.o
	$(LD) $(LDFLAGS) -o db_fit.exe db_fit.o $(LIBS) 

%.o:	%.cxx
	$(CXX) -c $(INCS) $(CXXFLAGS) $<

%.o:	%.cpp
	$(CXX) -c $(INCS) $(CXXFLAGS) $<

%.o:	%.C
	$(CXX) -c $(INCS) $(CXXFLAGS) $<

clean:
	rm -f *.o
	rm -f *~
	rm -f .*~

distclean: clean
	rm -f db_fit.exe
	


