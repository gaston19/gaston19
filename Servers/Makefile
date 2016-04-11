CC 		= g++ 

all : browser servers

servers : servers.c funciones.h definiciones.h funciones.o
	cc -o servers servers.c funciones.o -pthread

browser : browser.c funciones.h definiciones.h funciones.o
	cc -o browser browser.c funciones.o

funciones.o : funciones.c funciones.h
	cc -c funciones.c
	
clean: 
	rm -rf *o browser servers *~
