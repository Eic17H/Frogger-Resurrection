#include <stdbool.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

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
    INIZIALIZZA_NCURSES;
    MESSAGGIO_BENVENUTO;
    DIMENSIONE_FINESTRA;
    INIZIALIZZA_PARTITA;
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