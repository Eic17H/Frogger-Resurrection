#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "main.h"

void granata(int fdScrittura, int xPartenza, int yPartenza) {
    Messaggio messaggio;
    int x = xPartenza, y = yPartenza;

    messaggio.mittente = GRANATA;
    messaggio.pos.x = x;
    messaggio.pos.y = y;
    messaggio.pid = getpid();
    if (messaggio.pid < 0) {perror("Errore getpid()"); _exit(2);}
    messaggio.sparo = false;

    write(fdScrittura, &messaggio, sizeof(Messaggio));
    
    while (1) {
        x++;
        messaggio.pos.x = x;
        write(fdScrittura, &messaggio, sizeof(Messaggio));
        
        usleep(10000);
    }
}