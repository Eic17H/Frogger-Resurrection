out: main.o altrecose.o coccodrillo.o sparo.o inizializzazione.o listaCoccodrillo.o listaGranate.o processi.o rana.o regole.o visualizzazione.o manche.o thread.o listaThreadDaTerminare.o
	gcc main.o altrecose.o coccodrillo.o sparo.o inizializzazione.o listaCoccodrillo.o listaGranate.o processi.o rana.o regole.o visualizzazione.o manche.o thread.o listaThreadDaTerminare.o -g -l ncurses -l pthread -o Frogger
	rm -f *.o
main.o: main.h costanti.h struttureDati.h thread.h main.c
	gcc -g -c main.c
listaThreadDaTerminare.o: listaThreadDaTerminare.h listaThreadDaTerminare.c
	gcc -g -c listaThreadDaTerminare.c
thread.o: thread.h thread.c
	gcc -g -c thread.c
manche.o: manche.h manche.c
	gcc -g -c manche.c
altrecose.o: altrecose.h altrecose.c
	gcc -g -c altrecose.c
coccodrillo.o: coccodrillo.h coccodrillo.c
	gcc -g -c coccodrillo.c
sparo.o: sparo.h sparo.c
	gcc -g -c sparo.c
inizializzazione.o: inizializzazione.h inizializzazione.c
	gcc -g -c inizializzazione.c
listaCoccodrillo.o: listaCoccodrillo.h listaCoccodrillo.c
	gcc -g -c listaCoccodrillo.c
listaGranate.o: listaGranate.h listaGranate.c
	gcc -g -c listaGranate.c
processi.o: processi.h processi.c
	gcc -g -c processi.c
rana.o: rana.h rana.c
	gcc -g -c rana.c
regole.o: regole.h regole.c
	gcc -g -c regole.c
visualizzazione.o: visualizzazione.h visualizzazione.c
	gcc -g -c visualizzazione.c