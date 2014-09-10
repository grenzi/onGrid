CC = g++
CPPFLAGS = -g -Wno-deprecated -O2 -c
LDFLAGS	=  -O2 -lboost_thread
MYSQLCFLAGS = `mysql_config --cflags`
MYSQLLIBS = `mysql_config --libs`
SOURCES = main.cpp LDPWrapper.cpp layeredDP.cpp Dictionary.cpp Util.cpp Log.cpp DBManager.cpp DBJob.cpp
OBJECTS	= $(SOURCES:.cpp=.o)
EXECUTABLE = OnGrid_LDP_Service

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(MYSQLLIBS)

.cpp.o : 
	$(CC) $(CPPFLAGS) $(MYSQLCFLAGS) $< -o $@

rm:
	-rm -f *.o
