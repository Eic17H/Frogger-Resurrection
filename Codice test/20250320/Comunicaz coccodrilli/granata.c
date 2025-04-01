#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "main.h"

void granata(int fdScrittura, int xPartenza, int yPartenza, int direzione) {
    Messaggio messaggio;
    int x = xPartenza, y = yPartenza;

    messaggio.mittente = GRANATA;
    
    messaggio.posAttuale.x = x;
    messaggio.posAttuale.y = y;

    messaggio.posVecchia.x = x;
    messaggio.posVecchia.y = y;

    messaggio.pid = getpid();
    if (messaggio.pid < 0) {perror("Errore getpid()"); _exit(2);}

    write(fdScrittura, &messaggio, sizeof(Messaggio));

    while (1) {
        messaggio.posVecchia.x = x;

        x += direzione;

        messaggio.posAttuale.x = x;
        write(fdScrittura, &messaggio, sizeof(Messaggio));
        usleep(20000);
    }
}