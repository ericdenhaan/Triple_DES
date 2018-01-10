#CCC = g++11
CCC = g++

#CCCFLAGS = -Wall -g -std=c++11 -lpthread -lX11
CCCFLAGS = -Wall -g -std=c++11 -lpthread

all : main

main : main.o des.o
	$(CCC) $(CCCFLAGS) -o $@ $^

%.o : %.cc
	$(CCC) -c $(CCCFLAGS) $<

des.o : des.h

clean:
	rm -f *.o *~ *% *# .#* *_encrypted.txt *_decrypted.txt

clean-all: clean
	rm -f main
