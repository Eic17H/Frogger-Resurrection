out: main.o altrecose.o coccodrillo.o sparo.o inizializzazione.o listaCoccodrillo.o listaGranate.o processi.o rana.o regole.o visualizzazione.o manche.o
	gcc main.o altrecose.o coccodrillo.o sparo.o inizializzazione.o listaCoccodrillo.o listaGranate.o processi.o rana.o regole.o visualizzazione.o manche.o -l ncurses -l pthread -o Frogger
	rm -f *.o
main.o: main.h costanti.h struttureDati.h main.c
	gcc -c main.c
manche.o: manche.h manche.c
	gcc -c manche.c
altrecose.o: altrecose.h altrecose.c
	gcc -c altrecose.c
coccodrillo.o: coccodrillo.h coccodrillo.c
	gcc -c coccodrillo.c
sparo.o: sparo.h sparo.c
	gcc -c sparo.c
inizializzazione.o: inizializzazione.h inizializzazione.c
	gcc -c inizializzazione.c
listaCoccodrillo.o: listaCoccodrillo.h listaCoccodrillo.c
	gcc -c listaCoccodrillo.c
listaGranate.o: listaGranate.h listaGranate.c
	gcc -c listaGranate.c
processi.o: processi.h processi.c
	gcc -c processi.c
rana.o: rana.h rana.c
	gcc -c rana.c
regole.o: regole.h regole.c
	gcc -c regole.c
visualizzazione.o: visualizzazione.h visualizzazione.c
	gcc -c visualizzazione.c