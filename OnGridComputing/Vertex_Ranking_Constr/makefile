CC	= g++
CPPFLAGS= -g -Wno-deprecated -O2 -c
LDFLAGS	=  -O2
SOURCES	= main.cpp
OBJECTS	= $(SOURCES:.cpp=.o)
EXECUTABLE=VertexRankingConstr

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o : 
	$(CC) $(CPPFLAGS) $< -o $@

rm:
	-rm -f *.o
