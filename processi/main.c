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
    int round = 1, vite = N_VITE, nTaneOccupate = 0;
    int punteggioTotale = 0;
    bool tanaOccupata; 
    Flusso flussi[N_FLUSSI];
    pid_t pidRana;
    Tana tane[N_TANE];
    ListaCoccodrillo* listaCoccodrilli[N_FLUSSI];
    ListaGranata* listaGranate;
    // ======== ==== = == = ==== ========
    // ======== INIZIALIZZAZIONE ========
    // ======== ==== = == = ==== ========
    srand(time(NULL));
    inizializzaNcurses();
    messaggioBenvenuto();
    adattaFinestra();
    inizializzaColori();
    creaTane(N_TANE, tane);
    
    while (round <= N_MANCHE && vite > 0 && nTaneOccupate < N_TANE) {
        tanaOccupata = false;

        coloraAmbienteGioco();
        visualizzaVite(vite);
        visualizzaRoundRimasti(N_MANCHE - round);
        
        if (round > 1) chiudiPipe(fd);
        
        creaPipe(fd);
        pidRana = creaRana(2, fd);
        
        punteggioTotale += manche(fd, flussi, listaCoccodrilli, &listaGranate, pidRana, tane, 0, &tanaOccupata);
        if (!tanaOccupata) vite--;
        else nTaneOccupate++;

        round++;
    }
    messaggioFinePartita(nTaneOccupate, punteggioTotale);

    clear(); refresh();
    endwin();
    return 0;
}
