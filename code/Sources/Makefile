CFLAGS = -c -g -std=c99 -Wall -O3

DIRbin = bin/
DIRdata = ../datas/

all: create_folder run

run : prog 
	./prog

prog: $(DIRbin)main.o $(DIRbin)PAZL.o $(DIRbin)PALL.o $(DIRbin)operations.o $(DIRbin)init.o $(DIRbin)tests.o $(DIRbin)simons.o $(DIRbin)simulations.o
	gcc -o prog $(DIRbin)main.o $(DIRbin)PAZL.o $(DIRbin)PALL.o $(DIRbin)operations.o $(DIRbin)init.o $(DIRbin)tests.o $(DIRbin)simons.o $(DIRbin)simulations.o

$(DIRbin)main.o: main.c
	gcc $(CFLAGS) main.c -o $(DIRbin)main.o 

$(DIRbin)PAZL.o: PAZL.c  
	gcc $(CFLAGS) PAZL.c -o $(DIRbin)PAZL.o

$(DIRbin)PALL.o: PALL.c 
	gcc $(CFLAGS) PALL.c -o $(DIRbin)PALL.o

$(DIRbin)operations.o: operations.c 
	gcc $(CFLAGS) operations.c -o $(DIRbin)operations.o

$(DIRbin)init.o: init.c 
	gcc $(CFLAGS) init.c -o $(DIRbin)init.o

$(DIRbin)tests.o: tests.c 
	gcc $(CFLAGS) tests.c -o $(DIRbin)tests.o

$(DIRbin)simons.o: simons.c 
	gcc $(CFLAGS) simons.c -o $(DIRbin)simons.o

$(DIRbin)simulations.o: simulations.c
	gcc $(CFLAGS) simulations.c -o $(DIRbin)simulations.o

create_folder:
	mkdir -p $(DIRbin)
	mkdir -p $(DIRdata)

clean:
	rm -f $(DIRbin)*