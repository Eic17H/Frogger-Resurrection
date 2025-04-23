fine.out: clear main.o processi.o visualizzazione.o regole.o processi.o 
	gcc main.o processi.o visualizzazione.o regole.o processi.o -o fine.out -l ncurses
main.o: costanti.h struttureDati.h main.c
	gcc -c main.c 
processi.o: processi.h processi.c
	gcc -c processi.c
visualizzazione.o: visualizzazione.h visualizzazione.c
	gcc -c visualizzazione.c
regole.o: regole.h regole.c
	gcc -c regole.c
processi.o: processi.h processi.c
	gcc -c processi.c
clear:
	rm -f *.o