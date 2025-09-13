#include <unistd.h>
#include <stdbool.h>
#include <ncurses.h>

#include "main.h"
#include "regole.h"
#include "struttureDati.h"
#include "thread.h"

void sparo(Mittente mittente, Posizione posPartenza, int direzione, TuttoBuffer* buffer) {
    Messaggio messaggio;
    Posizione pos = posPartenza;

    messaggio.mittente = mittente;
    
    messaggio.posAttuale = pos;
    // inizialmente la posizione vecchia è la stessa di quella di partenza
    messaggio.posVecchia = pos;

    messaggio.id = pthread_self();

    invia(buffer, messaggio);

    while (!fuoriSchermo(messaggio.posAttuale, mittente, direzione) && terminaProcessi != 1) {
        messaggio.posVecchia = pos;

        pos.x += direzione;

        messaggio.posAttuale = pos;
        invia(buffer, messaggio);
        usleep(20000);
    }
}