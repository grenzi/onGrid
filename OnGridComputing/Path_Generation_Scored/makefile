CC	= g++
CPPFLAGS= -g -Wno-deprecated -O2 -c
LDFLAGS	=  -O2
SOURCES	= main.cpp Greedylabel.cpp Graph.cpp Utilityfunction.cpp BFS.cpp
OBJECTS	= $(SOURCES:.cpp=.o)
EXECUTABLE=kDLSPath

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o : 
	$(CC) $(CPPFLAGS) $< -o $@

rm:
	-rm -f *.o
