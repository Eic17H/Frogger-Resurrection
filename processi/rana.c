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
    Posizione pos = {0, 0}, posPartenzaGranata;
    Messaggio messaggio;
    time_t start = 0, ora = 0;
    _Bool sparato = false;

    // scrittura primo messaggio
    messaggio.tipo = POSIZIONE;
    messaggio.mittente = RANA;
    messaggio.pid = getpid();
    if (messaggio.pid < 0) {perror("Errore getpid()"); _exit(2);}
    messaggio.posAttuale = pos;
    // all'inizio la vecchia posizione è la stessa di quella attuale
    messaggio.posVecchia = pos;

    write(fdScrittura, &messaggio, sizeof(Messaggio));

    // TODO: forse no, forse direttamente la manchee legge il messaggio e aggiorna il punteggio così può usare altre informazioni
    Messaggio punti;
    punti.tipo = PUNTEGGIO;
    punti.mittente = RANA;
    punti.pid = getpid();
    punti.punteggio = 0;

    while (1) {
        punti.punteggio = 0;
        messaggio.posVecchia = pos;

        kCode = getch();

        switch(kCode) {
            case KEY_UP:
                    punti.punteggio = 10;
                    pos.y = -SALTO_RANA;
            break;

            case KEY_DOWN:
                    punti.punteggio = 3;
                    pos.y = SALTO_RANA;
            break;
            
            case KEY_RIGHT:
                    punti.punteggio = 5;
                    pos.x = SALTO_RANA + W_RANA;
            break;
            
            case KEY_LEFT:
                    punti.punteggio = 5;
                    pos.x = - SALTO_RANA - W_RANA;
            break;

            case KEY_BARRA_SPAZIATRICE:
                if (!sparato || ora - start >= TEMPO_RICARICA_GRANATA) {
                    sparato = true;
                    time(&start);
                    pos.x = CODICE_GRANATA_SPARATA;
                    pos.y = CODICE_GRANATA_SPARATA;
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

        if (messaggio.posVecchia.x == pos.x && messaggio.posVecchia.y == pos.y) {
            pos.x = 0; pos.y = 0;
        }
        messaggio.posAttuale = pos;
        write(fdScrittura, &messaggio, sizeof(Messaggio));
        write(fdScrittura, &punti, sizeof(Messaggio));

        usleep(1000);

        time(&ora);
    }
}
