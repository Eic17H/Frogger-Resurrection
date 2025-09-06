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

int main() {
    // ======== ==== ==== ========
    // ======== VARIABILI ========
    // ======== ==== ==== ========
    int fd[2];
    int x_rana = X_PARTENZA_RANA, y_rana = Y_PARTENZA_RANA, x_granata = NON_SU_SCHERMO, y_granata = NON_SU_SCHERMO;
    Flusso flussi[N_FLUSSI];
    pid_t pidRana;
    Tana tane[N_TANE];
    ListaCoccodrillo* listaCoccodrilli[N_FLUSSI];

    // ======== ==== = == = ==== ========
    // ======== INIZIALIZZAZIONE ========
    // ======== ==== = == = ==== ========
    srand(time(NULL));
    inizializzaNcurses();
    messaggioBenvenuto();
    adattaFinestra();
    inizializzaColori();
    coloraAmbienteGioco();

    creaPipe(fd);
    pidRana = creaRana(2, fd);
    manche(fd, flussi, listaCoccodrilli, pidRana, tane);
    
    coloraAmbienteGioco();
    
    chiudiPipe(fd);
    creaPipe(fd);

    pidRana = creaRana(2, fd);
    manche(fd, flussi, listaCoccodrilli, pidRana, tane);

    clear(); refresh();
    endwin();
    return 0;
}
