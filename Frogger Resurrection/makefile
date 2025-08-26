out: main.o altrecose.o coccodrillo.o granata.o inizializzazione.o listaCoccodrillo.o processi.o rana.o regole.o visualizzazione.o
	gcc main.o altrecose.o coccodrillo.o granata.o inizializzazione.o listaCoccodrillo.o processi.o rana.o regole.o visualizzazione.o -l ncurses -o Frogger
	rm -f *.o
main.o: main.h costanti.h struttureDati.h main.c
	gcc -c main.c
altrecose.o: altrecose.h altrecose.c
	gcc -c altrecose.c
coccodrillo.o: coccodrillo.h coccodrillo.c
	gcc -c coccodrillo.c
granata.o: granata.h granata.c
	gcc -c granata.c
inizializzazione.o: inizializzazione.h inizializzazione.c
	gcc -c inizializzazione.c
listaCoccodrillo.o: listaCoccodrillo.h listaCoccodrillo.c
	gcc -c listaCoccodrillo.c
processi.o: processi.h processi.c
	gcc -c processi.c
rana.o: rana.h rana.c
	gcc -c rana.c
regole.o: regole.h regole.c
	gcc -c regole.c
visualizzazione.o: visualizzazione.h visualizzazione.c
	gcc -c visualizzazione.c