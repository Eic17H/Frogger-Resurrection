#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include "costanti.h"
#include "inizializzazione.h"
#include "struttureDati.h"
#include "altrecose.h"

void coccodrillo(int fdScrittura, Flusso flussoAttuale) {
    Posizione pos, posPartenzaSparo; 
    Messaggio messaggio;
    time_t start, ora;
    _Bool sparato = false;
    int timerSparo, offsetSparo = 0;
    srand(time(NULL));

    pos = flussoAttuale.posIniziale;
    posPartenzaSparo.y = pos.y + 1; // +1 perché deve la bocca è sotto

    // scrittura primo messaggio
    messaggio.mittente = COCCO;
    messaggio.pid = getpid();
    if (messaggio.pid < 0) {perror("Errore getpid()"); _exit(2);}
    messaggio.posAttuale = pos;
    messaggio.posVecchia = pos;
    write(fdScrittura, &messaggio, sizeof(Messaggio));

    // offsetSparo usato per far partire il proiettile dalla giusta x. +3 usato per prevedere lo spostamento del coccodrillo
    if (flussoAttuale.verso == AVANZAMENTO_DX) offsetSparo = W_COCCODRILLO + flussoAttuale.velocità;
    else offsetSparo = - flussoAttuale.velocità - 1;

    timerSparo = RAND_TRA(ATTESA_MIN_PROIETTILE_S, ATTESA_MAX_PROIETTILE_S);
    // si parte sincronizzati
    time(&start);
    time(&ora);

    while (1) {
        messaggio.staPerSparare = false;

        time(&ora);
        if (timerSparo - (ora - start) <= PREAVVISO_SPARO_S) {
            messaggio.staPerSparare = true;
        }

        if (ora - start >= timerSparo) {
            posPartenzaSparo.x = pos.x + offsetSparo;
            creaProcessoProiettile(PROIETTILE, fdScrittura, posPartenzaSparo, flussoAttuale.verso);
            
            timerSparo = ATTESA_MIN_PROIETTILE_S + rand() % (ATTESA_MAX_PROIETTILE_S - ATTESA_MIN_PROIETTILE_S + 1);

            time(&start);
        }
        
        messaggio.posVecchia = pos;

        pos.x += flussoAttuale.velocità * flussoAttuale.verso;
        // aggiornamento coordinate
        messaggio.posAttuale = pos;
        write(fdScrittura, &messaggio, sizeof(Messaggio));

        usleep(400000);
    }
}
