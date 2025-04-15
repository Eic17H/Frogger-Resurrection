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
    inizializzaNcurses();
    void inizializzaPartita();
    fork();
    if(RANA){
        rana()
    }else if(MAIN){
        INIZIALIZZA_VARIABILI;
        INIZIALIZZA_TIMER;
        while(PARTITA_IN_CORSO){
            messaggio = LEGGI_MESSAGGIO;
            spostaSprite(messaggio.mittente, messaggio.posVecchia, messaggio.posAttuale, sprite(messaggio.mittente))
            VISUALIZZA_TIMER;
            VISUALIZZA_PUNTEGGIO;
        }
        kill(RANA)
    }
    endwin();
    return 0;
}