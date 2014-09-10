CC	= g++
CPPFLAGS= -Wno-deprecated -O2 -c
LDFLAGS	= -O2
SOURCES	= main.cpp umls.cpp
OBJECTS	= $(SOURCES:.cpp=.o)
EXECUTABLE=GraphGenerator

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o : 
	$(CC) $(CPPFLAGS) $< -o $@

rm:
	-rm -f *.o
