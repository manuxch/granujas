# \file  Makefile
# \brief Makefile
# \author Manuel Carlevaro [IFLYSIB]
# \versión 0.1 
# \date 2018.12.17
# \note Ver: http://makepp.sourceforge.net/1.19/makepp_tutorial.html


CXX = g++
BOX2D = /home/manuel/Tools/Box2D
LIBPATH = -L$(BOX2D)/Build/bin/x86_64/Debug
INCLUDES = -I$(BOX2D)
LIBS = -lBox2D -lm
CXXFLAGS = -O3 $(INCLUDES)
LDFLAGS = -O3 -Wall $(LIBPATH) -static $(LIBS)

OBJS = selfMag.o globalSetup.o sisAux.o

selfMag: $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) 

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm -f *.o *~ 
 
