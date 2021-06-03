CPP=g++.exe
CPPVERSION=-std=c++17
CPPFLAGS=-c -Wall
LDFLAGS=
PROJECT=checksum_inator
LINKER=

HEADER=

SOURCE=$(PROJECT).cpp ci_io_handler.cpp $(HEADER)
#SOURCE=$(PROJECT).cpp ci_io_handler.cpp $(HEADER) console/sega_8.cpp
OBJECTS=$(SOURCE:.cpp=.o)
EXECUTABLE=../bin/$(PROJECT).exe
RM= rm -f

all: $(SOURCE) $(EXECUTABLE)

clean:
	${RM} $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CPP) $(LDFLAGS) $(OBJECTS) $(LINKER) -static-libgcc -s -o $@

.cpp.o:
	$(CPP) $(CPPVERSION) $(CPPFLAGS) -s $< -o $@ 

#A little macro of sorts, just to avoid make "clean && make all" at the command prompt :3
full: clean all