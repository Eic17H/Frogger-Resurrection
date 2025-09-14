#include <pthread.h>
#include <stdbool.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "inizializzazione.h"
#include "struttureDati.h"
#include "costanti.h"
#include "listaCoccodrillo.h"
#include "manche.h"
#include "visualizzazione.h"
#include "thread.h"
#include "listaThreadDaTerminare.h"

int main() {
    srand(time(0));
    
    ListaThreadTerminabili* listaThreadTerminabili = (ListaThreadTerminabili*) malloc(sizeof(ListaThreadTerminabili));
    listaThreadTerminabili->testa = listaThreadTerminabili->coda = NULL;
    // ======== ==== ==== ========
    // ======== VARIABILI ========
    // ======== ==== ==== ========
    int x_rana = X_PARTENZA_RANA, y_rana = Y_PARTENZA_RANA, x_granata = NON_SU_SCHERMO, y_granata = NON_SU_SCHERMO;
    int round, vite, nTaneOccupate;
    int punteggioTotale;
    bool tanaOccupata; 
    Flusso flussi[N_FLUSSI];
    Tana tane[N_TANE];
    ListaCoccodrillo* listaCoccodrilli[N_FLUSSI];
    ListaGranata* listaGranate;

    pthread_t idRana;
    Messaggio* buffer = NULL;
    sem_t semLiberi, semOccupati;
    pthread_mutex_t mutex;
    int iScrittura = 0, iLettura = 0;

    TuttoBuffer tb = {0};
    // ======== ==== = == = ==== ========
    // ======== INIZIALIZZAZIONE ========
    // ======== ==== = == = ==== ========
    srand(time(NULL));
    inizializzaNcurses();
    messaggioBenvenuto();
    adattaFinestra();
    inizializzaColori();
    creaTane(N_TANE, tane);

    do {
        round = 1, vite = N_VITE, nTaneOccupate = 0;
        punteggioTotale = 0;
        while (round <= N_MANCHE && vite > 0 && nTaneOccupate < N_TANE) {
            tanaOccupata = false;

            coloraAmbienteGioco();
            visualizzaVite(vite);
            visualizzaRoundRimasti(N_MANCHE - round);

            inizializzaTuttoBuffer(&tb);
            if (pthread_create(&idRana, NULL, &rana, &tb) != 0) {
                endwin();
                perror("Errore creazione rana");
                return -1;
            }

            punteggioTotale += manche(flussi, listaCoccodrilli, &listaGranate, idRana, tane, 0, &tanaOccupata, &tb, listaThreadTerminabili);
            ripulisciTuttoBuffer(&tb);
            if (!tanaOccupata) vite--;
            else nTaneOccupate++;

            round++;
        }
        
        messaggioFinePartita(&tb, nTaneOccupate, punteggioTotale);
    } while(ricominciaPartita());

    clear(); refresh();
    endwin();
    return 0;
}
