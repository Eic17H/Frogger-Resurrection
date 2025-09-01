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

            char stringa[5];
            stringa[0] = posPartenza.x %100 /10 +'0';
            stringa[1] = posPartenza.x %10 /1 +'0';
            stringa[2] = ';';
            stringa[3] = posPartenza.y %100 /10 +'0';
            stringa[4] = posPartenza.y %10 /1 +'0';
            mvaddstr(0,7,stringa);
        messaggio.posVecchia = pos;

        pos.x += direzione;

        messaggio.posAttuale = pos;
        write(fdScrittura, &messaggio, sizeof(Messaggio));
        usleep(20000);
    }
}