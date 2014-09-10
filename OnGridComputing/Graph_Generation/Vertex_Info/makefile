CFLAGS=-Wall -O3


VInfoGen: parsevertexInfor.o
	g++ $(CFLAGS) $^ -o VInfoGen

%.o: %.cpp
	g++ $(CFLAGS) $< -c -o $@

VSemGen: parsevertexSemanticInfor.cpp
	g++ $(CFLAGS) $^ -o VSemGen	
	
%.o: %.cpp
	g++ $(CFLAGS) $< -c -o $@	

rm:
	-rm -f *.o