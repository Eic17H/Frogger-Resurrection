#include <stdbool.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "visualizzazione.h"
#include "inizializzazione.h"
#include "processi.h"
#include "struttureDati.h"
#include "costanti.h"
#include "regole.h"
#include "listaCoccodrillo.h"

int main(){
    // Creazione variabili necessarie
    // Le solite cose per ncurses
    // Messaggio di benvenuto
    // Dimensione della finestra
    // Inizializzazione dei colori
    // Inizializzazione tane
    // Inizializzazione coccodrilli
    // Fork per creare processo main e processo rana

    // Processo main:
    // Creazione variabili
    // Inizializzazione timer
    // Loop:
        // Lettura messaggio
        // Gestione messaggio della rana
            // Colore sfondo
            // Stampa per cancellare
            // Colore sprite
            // Aggiornamento variabile locale delle coordinate
            // Stampa dello sprite
        // Gestione messaggio della granata
            // Colore sfondo (la y rimane uguale a prescindere)
            // Stampa per cancellare
            // Se è ancora nello schermo stampa
            // Se è uscita dallo schermo kill
        // Gestione messaggio del coccodrillo
            // Colore coccodrillo
            // Cancella vecchio coccodrillo
            // Se è nello schermo
                // Disegna il coccodrillo
                // Capisce il flusso
                // Gestisce una lista vuota
            // Altrimenti, uccidi
        // Stampa timer
    // Alla fine, uscito dal loop, uccidi la rana

    // Nuova struttura invece:

    // Creazione variabili
    int fd[2], myPipe = pipe(fd), x_rana = X_PARTENZA_RANA, y_rana = Y_PARTENZA_RANA, x_granata = NON_SU_SCHERMO, y_granata = NON_SU_SCHERMO, indiceFlussoCoccodrilloPrimo; 
    Flusso flussi[N_FLUSSI];
    pid_t coccodrilliCreatiPerPrimi[N_FLUSSI];
    char* spriteCoccodrilloGiu = NULL, *spriteCoccodrillosu = NULL;
    Tana tane[N_TANE];
    srand(time(NULL));

    // Inizializzazione
    inizializzaNcurses();
    messaggioBenvenuto();
    void inizializzaPartita();


    pid_t pidRana = fork();

    if (forkFallita(pidRana)){
        endwin();
        perror("chiamata fork() rana");
        _exit(2);
    }

    if(processoFiglio(pidRana)){
        close(fd[0]);
        rana(fd[1]);
    }else if(processoPadre(pidRana)){
        // Inizializzazione variabili e timer
        bool vivo;
        time_t start, ora=0;
        Messaggio messaggio;
        time(&start);

        // Loop della manche
        while(!tempoScaduto(ora, start) && vivo){
            read(fd[0], &messaggio, sizeof(Messaggio));
            spostaSprite(messaggio.mittente, messaggio.posVecchia, messaggio.posAttuale, sprite(messaggio.mittente));
            if(messaggio.mittente = RANA) vivo = !cadutoInAcqua(messaggio.posAttuale);
            VISUALIZZA_TIMER;
            VISUALIZZA_PUNTEGGIO;
        }
        kill(pidRana, SIGTERM);

        /**
         * Come gestire i coccodrilli
         * DISTMAX è la distanza tra due coccodrilli diciamo
         * Se la testa è fuori dallo schermo, pop e dealloca
         * Se la coda è lontana >DISTMAX dal bordo dell'area di gioco, crea coccodrillo e push
         * Questo è consistente anche col fatto di creare un solo coccodrillo per riga all'inizio e far creare tutti gli altri in automatico
         */
    }
    endwin();
    return 0;
}