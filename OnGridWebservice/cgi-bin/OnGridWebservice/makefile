CC	= g++
CPPFLAGS= -g -Wno-deprecated -pthread -O2 -c
LDFLAGS	=  -O2 -pthread
MYSQLCFLAGS=`mysql_config --cflags`
MYSQLLIBS=`mysql_config --libs`
SOURCES	= main.cpp Greedylabel.cpp Graph.cpp Utilityfunction.cpp BFS.cpp Util.cpp Data.cpp Matrixdata.cpp Singledata.cpp Log.cpp DBManager.cpp
OBJECTS	= $(SOURCES:.cpp=.o)
EXECUTABLE=OnGrid_Web_Service

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(MYSQLLIBS)

.cpp.o : 
	$(CC) $(CPPFLAGS) $(MYSQLCFLAGS) $< -o $@

rm:
	-rm -f *.o
