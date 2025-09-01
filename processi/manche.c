#include "manche.h"

/**
 * LA STRUTTURA DI UNA MANCHE
 */
int manche(int fd[2], Flusso flussi[N_FLUSSI], ListaCoccodrillo* listaCoccodrilli[N_FLUSSI], pid_t pidRana, Tana tane[N_TANE]) {
    inizializzaManche(N_TANE, N_FLUSSI, tane, flussi, listaCoccodrilli, fd);

    // Inizializzazione variabili e timer
    bool vivo = true;
    time_t start, ora = 0;
    Messaggio messaggio;
    time(&start);
    int punteggio;

    // Loop principale
    while(tempoScaduto(time(&ora), start) && vivo){
        read(fd[0], &messaggio, sizeof(Messaggio));
        spostaSprite(messaggio, N_FLUSSI, flussi, listaCoccodrilli);
        if (messaggio.mittente == COCCO) aggiornaPosInListaCoccodrilli(messaggio, N_FLUSSI, flussi, listaCoccodrilli); 
        if(messaggio.mittente == RANA){
            vivo = !cadutoInAcqua(messaggio.posAttuale);
        } 
        
        controllaSpawnCoccodrilli(N_FLUSSI, listaCoccodrilli, flussi, fd);

        time(&ora);
        visualizzaTimer(ora-start);
        visualizzaPunteggio(punteggio);
        refresh();
    }
    kill(pidRana, SIGTERM);
}