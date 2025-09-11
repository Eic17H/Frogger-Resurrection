#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "inizializzazione.h"
#include "processi.h"
#include "struttureDati.h"
#include "costanti.h"
#include "listaCoccodrillo.h"
#include "manche.h"
#include "visualizzazione.h"

int main() {
    // ======== ==== ==== ========
    // ======== VARIABILI ========
    // ======== ==== ==== ========
    int fd[2];
    int x_rana = X_PARTENZA_RANA, y_rana = Y_PARTENZA_RANA, x_granata = NON_SU_SCHERMO, y_granata = NON_SU_SCHERMO;
    int round, vite, nTaneOccupate;
    int punteggioTotale;
    bool tanaOccupata; 
    Flusso flussi[N_FLUSSI];
    pthread_t idRana;
    Tana tane[N_TANE];
    ListaCoccodrillo* listaCoccodrilli[N_FLUSSI];
    ListaGranata* listaGranate;
    
    Messaggio* buff = NULL;
    sem_t semLiberi;
    sem_t semOccupati;
    pthread_mutex_t mutex;
    int indiceScrittura = 0, indiceLettura = 0;

    inizializzaBufferSemafori(&semLiberi,&semOccupati, &buff);
    ParamRana paramRana = {.buff = buff, .semLiberi = &semLiberi, .semOccupati = &semOccupati, .mutex = &mutex, .indiceScrittura = &indiceScrittura};

    // ======== ==== = == = ==== ========
    // ======== INIZIALIZZAZIONE ========
    // ======== ==== = == = ==== ========
    srand(time(NULL));
    inizializzaNcurses();
    if (pthread_create(&idRana, NULL, &rana, &paramRana) != 0) {
        endwin();
        perror("Errore creazione rana\n");
        return -1;
    }

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

            punteggioTotale += manche(buff, &indiceLettura, &semLiberi, &semOccupati, fd, flussi, listaCoccodrilli, &listaGranate, idRana, tane, 0, &tanaOccupata);
            if (!tanaOccupata) vite--;
            else nTaneOccupate++;

            round++;
        }
        chiudiPipe(fd);
        messaggioFinePartita(nTaneOccupate, punteggioTotale);
    } while(ricominciaPartita());

    clear(); refresh();
    endwin();
    return 0;
}
