#include "manche.h"
#include "costanti.h"

void stampaPosPosPosPosPos(Posizione posDaStampare, int xInCuiStampare, int yInCuiStampare) {
    char stringa[9] = {posDaStampare.x%10000/1000+'0', posDaStampare.x%1000/100+'0', posDaStampare.x%100/10+'0', posDaStampare.x%10+'0', ';', posDaStampare.y%10000/1000+'0', posDaStampare.y%1000/100+'0', posDaStampare.y%100/10+'0', posDaStampare.y%10+'0'};
    mvaddstr(yInCuiStampare, xInCuiStampare, stringa);
}

bool fatto = false;

void aggiornaPosizioneRana(Posizione *posMain, Posizione posInviata, Flusso flussi[N_FLUSSI], ListaCoccodrillo* lista[N_FLUSSI]) {
    Posizione posVecchia = *posMain;
    stampaPosPosPosPosPos(*posMain, 10, 0);
    posMain->x += posInviata.x;
    posMain->y += posInviata.y;
    Posizione posAttuale = *posMain;
    // TODO: A INIZIO PARTITA È 700 PER UN CICLO DI LOGICA????????????'''
    if(!fatto)
    stampaPosPosPosPosPos(posInviata, 20, 0);
    fatto = true;

    NodoCoccodrillo *coccodrilloAttuale=NULL, *coccodrilloPrecedente=NULL;
    int px;

    if (posAttuale.y < DIM_LINES - H_MARCIAPIEDE && posAttuale.y > H_SPONDA) {
        // Se si è mossa
        if (1) {    
            int i = trovaIndiceFlusso(N_FLUSSI, flussi, posAttuale.y);
            
            // PROVVISORIO: Perché a questo punto la si perde la partita
            if (i != -1) {;
                coccodrilloAttuale = trovaCoccodrilloSottoRana(posAttuale, coccodrilloAttuale, lista, i);
            } else {fineRound(); return ;}
        }
        if(coccodrilloAttuale != NULL )mvaddch(coccodrilloAttuale->dato.posAttuale.y, coccodrilloAttuale->dato.posAttuale.x, '!');
        else beep();
        if (coccodrilloAttuale != NULL) {        

            if ((coccodrilloPrecedente == NULL || laRanaESuUnCoccodrilloDiverso(coccodrilloPrecedente, coccodrilloAttuale) ||
                laRanaSpostataSuStessoCoccodrillo(coccodrilloPrecedente, coccodrilloAttuale, posVecchia, posAttuale))) {

                coccodrilloPrecedente = coccodrilloAttuale;
            }
                px = trovaPosRanaSuCoccodrillo(coccodrilloAttuale->dato.posAttuale.x, posAttuale.x);

            // TODO:  per ora snappa al centro del coccodrillo, non so perché px non funzioni boh
            posMain->x = coccodrilloAttuale->dato.posAttuale.x + W_RANA;
        } else { return ;}}
}

/**
 * LA STRUTTURA DI UNA MANCHE
 */
int manche(int fd[2], Flusso flussi[N_FLUSSI], ListaCoccodrillo* listaCoccodrilli[N_FLUSSI], pid_t pidRana, Tana tane[N_TANE]) {
    inizializzaManche(N_TANE, N_FLUSSI, tane, flussi, listaCoccodrilli, fd);

    // Inizializzazione variabili e timer
        bool vivo = true;
        time_t start, ora = 0;
        Posizione predefinita = {0,0};
        Messaggio messaggio = {predefinita, predefinita, -1, -1};
        time(&start);
        int punteggio;

    // Variabili relative alla singola manche
        Posizione posRana = {X_PARTENZA_RANA, Y_PARTENZA_RANA};

    // Loop principale
    while(!tempoScaduto(time(&ora), start) && vivo){
        read(fd[0], &messaggio, sizeof(Messaggio));
        spostaSprite(messaggio, N_FLUSSI, flussi, listaCoccodrilli);
        if (messaggio.mittente == COCCO) aggiornaPosInListaCoccodrilli(messaggio, N_FLUSSI, flussi, listaCoccodrilli); 
        if(messaggio.mittente == RANA){
            vivo = !cadutoInAcqua(messaggio.posAttuale);
            Messaggio msg;
            msg.mittente = RANA;
            msg.pid = messaggio.pid;
            msg.posVecchia = posRana;
            aggiornaPosizioneRana(&posRana, messaggio.posAttuale, flussi, listaCoccodrilli);
            msg.posAttuale = posRana;
            spostaSprite(msg, N_FLUSSI, flussi, listaCoccodrilli);
        }
        
        controllaSpawnCoccodrilli(N_FLUSSI, listaCoccodrilli, flussi, fd);

        time(&ora);
        visualizzaTimer(DURATA_MANCHE_S - (ora-start));
        visualizzaPunteggio(punteggio);
        refresh();
    }
    kill(pidRana, SIGTERM);
}