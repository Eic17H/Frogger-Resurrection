#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <ncurses.h>

#include "struttureDati.h"

void granata(int fdScrittura, Posizione posPartenza, int direzione) {
    Messaggio messaggio;
    Posizione pos = posPartenza;

    messaggio.mittente = GRANATA;
    
    messaggio.posAttuale = pos;
    // inizialmente la posizione vecchia è la stessa di quella di partenza
    messaggio.posVecchia = pos;

    messaggio.pid = getpid();
    if (messaggio.pid < 0) {perror("Errore getpid()"); _exit(2);}

    write(fdScrittura, &messaggio, sizeof(Messaggio));

    while (1) {
        messaggio.posVecchia = pos;

        pos.x += direzione;

        messaggio.posAttuale = pos;
        write(fdScrittura, &messaggio, sizeof(Messaggio));
        usleep(20000);
    }
}