#include "manche.h"
#include "altrecose.h"
#include "costanti.h"
#include "inizializzazione.h"
#include "struttureDati.h"
#include <ncurses.h>


void stampaPosPosPosPosPos(Posizione posDaStampare, int xInCuiStampare, int yInCuiStampare) {
    char stringa[8] = {posDaStampare.x%1000/100+'0', posDaStampare.x%100/10+'0', posDaStampare.x%10+'0', ';', posDaStampare.y%1000/100+'0', posDaStampare.y%100/10+'0', posDaStampare.y%10+'0', '\0'};
    mvaddstr(yInCuiStampare, xInCuiStampare, stringa);
}

/**
 * Aggiorna la posizione della rana in posMain
 * Restituisce falso se la posizione risulta nella sconfitta
 */
bool aggiornaPosizioneRana(Posizione *posMain, Posizione posInviata, Flusso flussi[N_FLUSSI], ListaCoccodrillo* lista[N_FLUSSI]) {
    Posizione posVecchia = *posMain;
    //stampaPosPosPosPosPos(*posMain, 10, 0);
    posMain->x += posInviata.x;
    posMain->y += posInviata.y;
    Posizione posAttuale = *posMain;
    
    static NodoCoccodrillo *coccodrilloAttuale=NULL, *coccodrilloPrecedente=NULL;
    static int offsetSuCoccodrillo = 0;

    if (posAttuale.x < 0 || posAttuale.x + W_RANA > DIM_COLS) return false;
    
    if (posAttuale.y < DIM_LINES - H_MARCIAPIEDE && posAttuale.y > H_SPONDA) {
        // Se si è mossa
        if (!posizioniUguali(posVecchia, posAttuale)) {    
            int i = trovaIndiceFlusso(N_FLUSSI, flussi, posAttuale.y);
            
            if (i == -1) { return false; }
            coccodrilloAttuale = trovaCoccodrilloSottoRana(posAttuale, coccodrilloAttuale, lista, i);
        }
        if(coccodrilloAttuale != NULL )mvaddch(coccodrilloAttuale->dato.posAttuale.y, coccodrilloAttuale->dato.posAttuale.x, '!');
        if (coccodrilloAttuale != NULL) {        

            if ((coccodrilloPrecedente == NULL || laRanaESuUnCoccodrilloDiverso(coccodrilloPrecedente, coccodrilloAttuale) ||
                laRanaSpostataSuStessoCoccodrillo(coccodrilloPrecedente, coccodrilloAttuale, posVecchia, posAttuale))) {

                coccodrilloPrecedente = coccodrilloAttuale;
                offsetSuCoccodrillo = trovaPosRanaSuCoccodrillo(coccodrilloAttuale->dato.posAttuale.x, posAttuale.x);
            }
            // TODO: Ancora non funziona :(. Ora snappa o al lato destro o a quello sinistro. Boh.
            // TODO: La rana viene renderizzata due volte, lasciando un buco sul coccodrillo a seconda della direzione.
            posMain->x = coccodrilloAttuale->dato.posAttuale.x + offsetSuCoccodrillo;
        } else { return false;}}
        return true;
}

/**
 * LA STRUTTURA DI UNA MANCHE
 */
void manche(int fd[2], Flusso flussi[N_FLUSSI], ListaCoccodrillo* listaCoccodrilli[N_FLUSSI], pid_t pidRana, Tana tane[N_TANE]) {
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
        if (messaggio.mittente != RANA) {
            spostaSprite(messaggio, N_FLUSSI, flussi, listaCoccodrilli);
            if (messaggio.mittente == COCCO) aggiornaPosInListaCoccodrilli(messaggio, N_FLUSSI, flussi, listaCoccodrilli); 
        }
        else {
            if (messaggio.posAttuale.x != CODICE_GRANATA_SPARATA && messaggio.posAttuale.y != CODICE_GRANATA_SPARATA) {
                Messaggio msg;
                msg.mittente = RANA;
                msg.pid = messaggio.pid;
                msg.posVecchia = posRana;
                vivo = aggiornaPosizioneRana(&posRana, messaggio.posAttuale, flussi, listaCoccodrilli);
                msg.posAttuale = posRana;
                spostaSprite(msg, N_FLUSSI, flussi, listaCoccodrilli);
            } else {
                Posizione posPartenzaGranata;
                posPartenzaGranata.x = posRana.x + W_RANA;
                posPartenzaGranata.y = posRana.y;

                creaProcessoGranata(fd[1], posPartenzaGranata, AVANZAMENTO_DX);    
                // posizione granata sinistra
                posPartenzaGranata.x = posRana.x - 1;
                creaProcessoGranata(fd[1], posPartenzaGranata, AVANZAMENTO_SX);
            }
        }
        
        controllaSpawnCoccodrilli(N_FLUSSI, listaCoccodrilli, flussi, fd);

        time(&ora);
        visualizzaTimer(DURATA_MANCHE_S - (ora-start));
        visualizzaPunteggio(punteggio);
        refresh();
    }

    kill(pidRana, SIGKILL);

    for(int i = 0; i<N_FLUSSI; i++) {
        NodoCoccodrillo* listaCoccodrilliDiQuestoFlusso = listaCoccodrilli[i]->testa;
        NodoCoccodrillo* successivo = listaCoccodrilliDiQuestoFlusso->successivo;
        for(NodoCoccodrillo* coccodrillo = listaCoccodrilliDiQuestoFlusso; coccodrillo != NULL; coccodrillo = successivo){
            kill(coccodrillo->dato.pid, SIGKILL);
            successivo = coccodrillo->successivo;
            free(coccodrillo);
        }
    }

    messaggioAltroRound(vivo);
}

// TODO: cambiare il parametro per includere i proiettili
void messaggioAltroRound(bool vivo) {
    if (vivo) {TESTO_CENTRATO("TEMPO SCADUTO!");}
    else {
        TESTO_CENTRATO("CADUTO IN ACQUA!");
    } 
    refresh();
    for(int i=0; i<4; i++){
        beep();
        usleep(150000);
    }
    sleep(1);
    clear();
}