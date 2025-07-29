CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c11 -O0 -ggdb -no-pie
LIBS:=-lm

all: cachesim

cachesim: cachesim.o parser.o cache.o listaDobleEnlazada.o lectorArchivo.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

cachesim.o: cachesim.c parser.h cache.h lectorArchivo.h
	$(CC) $(CFLAGS) -c $< -o $@

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c $< -o $@

cache.o: cache.c cache.h listaDobleEnlazada.h
	$(CC) $(CFLAGS) -c $< -o $@	

listaDobleEnlazada.o: listaDobleEnlazada.c listaDobleEnlazada.h
	$(CC) $(CFLAGS) -c $< -o $@

lectorArchivo.o: lectorArchivo.c lectorArchivo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f cachesim

	

