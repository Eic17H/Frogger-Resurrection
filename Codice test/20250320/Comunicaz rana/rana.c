#include <ncurses.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include "main.h"
#include "rana.h"
#include "granata.h"

void rana(int n, int fd[n]) {
    int kCode, x = X_PARTENZA_RANA, xVecchia, y = Y_PARTENZA_RANA, yVecchia, spazioNuovaTestaRana = SALTO_RANA + W_RANA, wRanaSenzaTesta = (W_RANA - 1);
    Messaggio messaggio;
    time_t start = 0, ora = 0;
    _Bool sparato = false;

    // scrittura primo messaggio
    messaggio.mittente = RANA;
    messaggio.pid = getpid();
    if (messaggio.pid < 0) {perror("Errore getpid()"); _exit(2);}
    messaggio.posAttuale.x = x;
    messaggio.posAttuale.y = y;

    messaggio.posVecchia.x = x;
    messaggio.posVecchia.y = y;

    write(fd[1], &messaggio, sizeof(Messaggio));

    while (1) {
        xVecchia = x;
        yVecchia = y;

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

            case KEY_BARRA_SPAZIATRICE:
                if (!sparato || ora - start >= TEMPO_RICARICA_GRANATA) {
                    sparato = true;
                    time(&start);
                    creaProcessoGranata(fd[1], x + W_RANA, y, AVANZAMENTO_DX);    
                    creaProcessoGranata(fd[1], x -  1, y, AVANZAMENTO_SX);
                }
                break;

            default:
            break;
        }

        // aggiornamento coordinate
        messaggio.posVecchia.x = xVecchia;
        messaggio.posVecchia.y = yVecchia;
        messaggio.posAttuale.x = x;
        messaggio.posAttuale.y = y;
        write(fd[1], &messaggio, sizeof(Messaggio));

        usleep(10000);

        time(&ora);
    }

}

void creaProcessoGranata(int fdScrittura, int xPartenza, int yPartenza, int direzione) {
    pid_t pid_granata = fork();
    if (pid_granata < 0) {perror("Errore fork() granata"); _exit(2);}

    if (pid_granata == 0) { // processo granata (eredita il fd chiuso in lettura)
        granata(fdScrittura, xPartenza, yPartenza, direzione);
    }
    // processo padre continua l'esecuzione
}