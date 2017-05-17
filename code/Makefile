CFLAGS = -c -g -std=c99 -Wall -O3

all: run

run : prog 
	./prog

prog: code.o bruteforce.o simons.o
	gcc -o prog code.o bruteforce.o simons.o

code.o: code.c struct.h bruteforce.h 
	gcc $(CFLAGS) code.c

bruteforce.o: bruteforce.c struct.h
	gcc $(CFLAGS) bruteforce.c

simons.o: simons.c 
	gcc $(CFLAGS) simons.c


clean:
	rm -f *.o