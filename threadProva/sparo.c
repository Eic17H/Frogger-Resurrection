#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <ncurses.h>

#include "struttureDati.h"
#include "thread.h"

void sparo(Mittente mittente, int fdScrittura, Posizione posPartenza, int direzione, TuttoBuffer* buffer) {
    Messaggio messaggio;
    Posizione pos = posPartenza;

    messaggio.mittente = mittente;
    
    messaggio.posAttuale = pos;
    // inizialmente la posizione vecchia è la stessa di quella di partenza
    messaggio.posVecchia = pos;

    messaggio.id = pthread_self();

    invia(buffer, messaggio);

    while (1) {
        messaggio.posVecchia = pos;

        pos.x += direzione;

        messaggio.posAttuale = pos;
        invia(buffer, messaggio);
        usleep(20000);
    }
}