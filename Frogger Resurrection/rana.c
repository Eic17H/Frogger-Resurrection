#include <ncurses.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "costanti.h"
#include "rana.h"
#include "granata.h"

void rana(int fdScrittura) {
    int kCode, spazioNuovaTestaRana = SALTO_RANA + W_RANA, wRanaSenzaTesta = (W_RANA - 1);
    Posizione pos, posPartenzaGranata;
    Messaggio messaggio;
    time_t start = 0, ora = 0;
    _Bool sparato = false;

    pos.x = X_PARTENZA_RANA; 
    pos.y = Y_PARTENZA_RANA;

    // scrittura primo messaggio
    messaggio.mittente = RANA;
    messaggio.pid = getpid();
    if (messaggio.pid < 0) {perror("Errore getpid()"); _exit(2);}
    messaggio.posAttuale = pos;
    // all'inizio la vecchia posizione è la stessa di quella attuale
    messaggio.posVecchia = pos;

    write(fdScrittura, &messaggio, sizeof(Messaggio));

    while (1) {
        messaggio.posVecchia = pos;

        kCode = getch();

        switch(kCode) {
            case KEY_UP:
                if (pos.y - SALTO_RANA > 0) {
                    pos.y -= SALTO_RANA;
                }
            break;

            case KEY_DOWN:
                if (pos.y + SALTO_RANA < DIM_LINES) {
                    pos.y += SALTO_RANA;
                } 
            break;
            
            /* La x è la posizione della testa della rana. Muovendosi a destra, la nuova posizione della testa sarà in x + spazioNuovaTestaRana.
            * A sinistra si ha x - spazioNuovaTestaRana. */
            case KEY_RIGHT:
                // Per evitare di uscire dal bordo destro, la testa dopo il salto + il corpo della rana non devono superare il confine */   
                if (pos.x + spazioNuovaTestaRana + wRanaSenzaTesta < DIM_COLS) {
                    pos.x += spazioNuovaTestaRana;
                }
            break;
            
            case KEY_LEFT:
                // Per evitare di uscire dal bordo sinistro, la testa dopo il salto non deve superare il confine */   
                if (pos.x - spazioNuovaTestaRana >= 0) {
                    pos.x -= spazioNuovaTestaRana;
                }
            break;

            case KEY_BARRA_SPAZIATRICE:
                if (!sparato || ora - start >= TEMPO_RICARICA_GRANATA) {
                    sparato = true;
                    time(&start);
                    // posizione granata destra
                    posPartenzaGranata.x = pos.x + W_RANA;
                    posPartenzaGranata.y = pos.y;
                    creaProcessoGranata(fdScrittura, posPartenzaGranata, AVANZAMENTO_DX);    
                    // posizione granata sinistra
                    posPartenzaGranata.x = pos.x - 1;
                    creaProcessoGranata(fdScrittura, posPartenzaGranata, AVANZAMENTO_SX);
                }
                break;

            default:
            break;
        }

        messaggio.posAttuale = pos;
        write(fdScrittura, &messaggio, sizeof(Messaggio));

        usleep(1000);

        time(&ora);
    }
}

void creaProcessoGranata(int fdScrittura, Posizione posPartenza, int direzione) {
    pid_t pid_granata = fork();
    if (pid_granata < 0) {perror("Errore fork() granata"); _exit(2);}

    if (pid_granata == 0) { // processo granata (eredita il fd chiuso in lettura)
        granata(fdScrittura, posPartenza, direzione);
    }
    // processo padre continua l'esecuzione
}
