CC = g++
CFLAGS = -std=c++11 -Wall -g
DEPS =

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sniff: main.o Params.o Sniff.o FileID.o tools.o
	$(CC) $(CFLAGS) -o sniff main.o Params.o Sniff.o FileID.o tools.o
 
main.o: main.cpp FileID.hpp Params.hpp Sniff.hpp tools.hpp
	$(CC) $(CFLAGS) -c main.cpp

clean:
	rm -f *.o sniff
