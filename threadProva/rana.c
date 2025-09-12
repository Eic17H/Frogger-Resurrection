#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "costanti.h"
#include "rana.h"
#include "thread.h"

void* rana(void* args) {
    TuttoBuffer* argomenti = (TuttoBuffer*)args;

    int kCode, spazioNuovaTestaRana = SALTO_RANA + W_RANA, wRanaSenzaTesta = (W_RANA - 1);
    Posizione pos = {0, 0}, posPartenzaGranata;
    Messaggio messaggio;
    time_t start = 0, ora = 0;

    // scrittura primo messaggio
    messaggio.mittente = RANA;
    messaggio.id = pthread_self();
    messaggio.posAttuale = pos;
    // all'inizio la vecchia posizione è la stessa di quella attuale
    messaggio.posVecchia = pos;

    invia(argomenti, messaggio);

    while (1) {
        messaggio.posVecchia = pos;

        kCode = getch();

        switch(kCode) {
            case KEY_UP:
                    pos.y = -SALTO_RANA;
            break;

            case KEY_DOWN:
                    pos.y = SALTO_RANA;
            break;
            
            case KEY_RIGHT:
                    pos.x = SALTO_RANA + W_RANA;
            break;
            
            case KEY_LEFT:
                    pos.x = - SALTO_RANA - W_RANA;
            break;

            case KEY_BARRA_SPAZIATRICE:
                if (ora - start >= TEMPO_RICARICA_GRANATA) {
                    time(&start);
                    pos.x = CODICE_GRANATA_SPARATA;
                    pos.y = CODICE_GRANATA_SPARATA;
                    // posizione granata destra
                    posPartenzaGranata.x = pos.x + W_RANA;
                    posPartenzaGranata.y = pos.y;
                    //creaProcessoGranata(fdScrittura, posPartenzaGranata, AVANZAMENTO_DX);    
                    // posizione granata sinistra
                    posPartenzaGranata.x = pos.x - 1;
                    //creaProcessoGranata(fdScrittura, posPartenzaGranata, AVANZAMENTO_SX);
                }
                break;

            default:
            break;
        }

        if (messaggio.posVecchia.x == pos.x && messaggio.posVecchia.y == pos.y) {
            pos.x = 0; pos.y = 0;
        }
        messaggio.posAttuale = pos;
        invia(argomenti, messaggio);

        usleep(4000);

        time(&ora);
    }
}
