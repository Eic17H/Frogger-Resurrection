#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "struttureDati.h"
#include "costanti.h"
#include "rana.h"

void* rana(void* args) {
    ParamRana* argomenti = (ParamRana*)args; 

    int kCode, spazioNuovaTestaRana = SALTO_RANA + W_RANA, wRanaSenzaTesta = (W_RANA - 1), *i = NULL;
    Posizione pos = {0, 0}, posPartenzaGranata;
    time_t start = 0, ora = 0;
    Messaggio messaggio;

    // scrittura primo messaggio
    messaggio.mittente = RANA;
    messaggio.pid = pthread_self();
    messaggio.posAttuale = pos;
    // all'inizio la vecchia posizione è la stessa di quella attuale
    messaggio.posVecchia = pos;

    // aspetto per spazio libero e poi semLiberi--
    sem_wait(argomenti->semLiberi); 
    // mutex per accedere a indice di scrittura
    pthread_mutex_lock(argomenti->mutex); 
    
    // per leggibilità
    i = argomenti->indiceScrittura; 
    // writing coordintes into buffer (index = read_index)
    argomenti->buff[*i] = messaggio;
    *argomenti->indiceScrittura = (*argomenti->indiceScrittura + 1) % DIM_BUFFER; // incrementing circular buffer
    
    // rilascio l'accesso all'indice di scrittura
    pthread_mutex_unlock(argomenti->mutex);
    // segnalo nuovo spazio occupato
    sem_post(argomenti->semOccupati); 

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

        sem_wait(argomenti->semLiberi); 
        pthread_mutex_lock(argomenti->mutex); 
        
        i = argomenti->indiceScrittura; 
        argomenti->buff[*i] = messaggio;
        *argomenti->indiceScrittura = (*argomenti->indiceScrittura + 1) % DIM_BUFFER; // incrementing circular buffer
        
        pthread_mutex_unlock(argomenti->mutex);
        sem_post(argomenti->semOccupati); 

        usleep(4000);

        time(&ora);
    }
}
