PRG=reverb
#PRG=gcc0.exe

OSTYPE := $(shell uname)
ifeq ($(OSTYPE),Linux)
CYGWIN=
else
CYGWIN=-Wl,--enable-auto-import
endif

GCC=g++
#GCCFLAGS=-g -Wall -Wextra -std=c++11 -pedantic -Wold-style-cast -Woverloaded-virtual -Wsign-promo  -Wctor-dtor-privacy -Wnon-virtual-dtor -Wreorder
GCCFLAGS=-g -Wall -std=c++11 -pedantic -Woverloaded-virtual -Wsign-promo  -Wctor-dtor-privacy -Wnon-virtual-dtor -Wreorder
DEFINE=

VALGRIND_OPTIONS=-q --leak-check=full
DIFF_OPTIONS=-y --strip-trailing-cr --suppress-common-lines

OBJECTS0=
DRIVER0=reverb.cpp reverb.hpp

gcc0:
	g++  $(DRIVER0) $(OBJECTS0) $(GCCFLAGS) $(DEFINE) -lpthread -o $(PRG)
#	thunar sine_wave.wav	
#0 1 2 3 4 5 6:
#	@echo "should run in less than 400 ms"
#	./$(PRG) $@ >studentout$@
#	@echo "lines after the next are mismatches with master output -- see out$@"
#	diff out$@ studentout$@ $(DIFFLAGS)
#7:
#	@echo "should run in less than 5000 ms"
#	./$(PRG) $@ >studentout$@
#	@echo "lines after the next are mismatches with master output -- see out$@"
#	diff out$@ studentout$@ $(DIFFLAGS)
clean:
	rm -f *.exe *.o *.obj
