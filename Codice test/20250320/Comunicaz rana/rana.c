#include <ncurses.h>
#include <unistd.h>

#include "rana.h"
#include "main.h"

void rana(int n, int fd[n]) {
    int kCode, x = X_PARTENZA_RANA, y = Y_PARTENZA_RANA, spazioNuovaTestaRana = SALTO_RANA + W_RANA, wRanaSenzaTesta = W_RANA - 1;
    Messaggio messaggio;

    // scrittura primo messaggio
    messaggio.mittente = RANA;
    messaggio.pid = getpid();
    if (messaggio.pid < 0) {perror("Errore getpid()"); _exit(2);}
    messaggio.pos.x = x;
    messaggio.pos.y = y;

    write(fd[1], &messaggio, sizeof(Messaggio));

    while (1) {
        kCode = getch();

        switch(kCode) {
            case KEY_UP:
                if (y - SALTO_RANA > 0) {
                    y -= SALTO_RANA;
                }
            break;

            case KEY_DOWN:
                if (y + SALTO_RANA < LINES) {
                    y += SALTO_RANA;
                } 
            break;
            
            /* La x è la posizione della testa della rana. Muovendosi a destra, la nuova posizione della testa sarà in x + spazioNuovaTestaRana.
            * A sinistra si ha x - spazioNuovaTestaRana. */
            case KEY_RIGHT:
                // Per evitare di uscire dal bordo destro, la testa dopo il salto + il corpo della rana non devono superare il confine */   
                if (x + spazioNuovaTestaRana + wRanaSenzaTesta < COLS) {
                    x += spazioNuovaTestaRana;
                }
            break;
            
            case KEY_LEFT:
                // Per evitare di uscire dal bordo sinistro, la testa dopo il salto non deve superare il confine */   
                if (x - spazioNuovaTestaRana >= 0) {
                    x -= spazioNuovaTestaRana;
                }
            break;
            
            default:
            break;
        }

        // aggiornamento coordinate
        messaggio.pos.x = x;
        messaggio.pos.y = y;
        write(fd[1], &messaggio, sizeof(Messaggio));

        usleep(10000);
    }

}