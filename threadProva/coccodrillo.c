#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include "costanti.h"
#include "struttureDati.h"
#include "altrecose.h"
#include "thread.h"
#include "inizializzazione.h"

void coccodrillo(Flusso flussoAttuale, TuttoBuffer* buffer) {
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
    messaggio.id = pthread_self();
    messaggio.posAttuale = pos;
    messaggio.posVecchia = pos;
    invia(buffer, messaggio);

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
            creaProcessoProiettile(PROIETTILE, posPartenzaSparo, flussoAttuale.verso, buffer);
            
            timerSparo = RAND_TRA(ATTESA_MIN_PROIETTILE_S, ATTESA_MAX_PROIETTILE_S);

            time(&start);
        }
        
        messaggio.posVecchia = pos;

        pos.x += flussoAttuale.velocità * flussoAttuale.verso;
        // aggiornamento coordinate
        messaggio.posAttuale = pos;
        invia(buffer, messaggio);

        //usleep(400000);
        usleep(150000);
    }
}
