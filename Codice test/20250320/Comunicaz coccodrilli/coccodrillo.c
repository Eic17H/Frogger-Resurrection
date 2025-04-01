#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include "main.h"

void coccodrillo(int fdScrittura, Flusso flussoAttuale) {
    //int kCode, x = 0, xVecchia, y = 0, yVecchia, spazioNuovaTestaRana = SALTO_RANA + W_RANA, wRanaSenzaTesta = (W_RANA - 1);
    Posizione pos; 
    Messaggio messaggio;
    time_t start = 0, ora = 0;
    _Bool sparato = false;

    pos = flussoAttuale.posIniziale;

    // scrittura primo messaggio
    messaggio.mittente = COCCOD;
    messaggio.pid = getpid();
    if (messaggio.pid < 0) {perror("Errore getpid()"); _exit(2);}
    messaggio.posAttuale = pos;

    messaggio.posVecchia = pos;

    write(fdScrittura, &messaggio, sizeof(Messaggio));

    while (1) {
        messaggio.posVecchia = pos;

        pos.x += flussoAttuale.velocità * flussoAttuale.verso;
        // aggiornamento coordinate
        messaggio.posAttuale = pos;
        write(fdScrittura, &messaggio, sizeof(Messaggio));

        usleep(1000000);
    }

}
