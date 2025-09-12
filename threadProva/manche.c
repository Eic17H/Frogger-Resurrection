#include "manche.h"
#include "altrecose.h"
#include "costanti.h"
#include "inizializzazione.h"
#include "listaGranate.h"
#include "regole.h"
#include "struttureDati.h"
#include "visualizzazione.h"
#include <ncurses.h>
#include "thread.h"

/**
 * Aggiorna la posizione della rana in posMain
 * Restituisce falso se la posizione risulta nella sconfitta
 */
bool aggiornaPosizioneRana(Posizione *posMain, Posizione posInviata, Flusso flussi[N_FLUSSI], ListaCoccodrillo* lista[N_FLUSSI]) {
    Posizione posVecchia = *posMain;
    if (!fuoriSchermo(sommaPosizioni(*posMain, posInviata), RANA, 0)) {
        posMain->x += posInviata.x;
        posMain->y += posInviata.y;
    }
    Posizione posAttuale = *posMain;
    
    static NodoCoccodrillo *coccodrilloAttuale=NULL, *coccodrilloPrecedente=NULL;
    static int offsetSuCoccodrillo = 0;

    if(!NELL_AREA_DI_GIOCO(posAttuale)) coccodrilloAttuale = coccodrilloPrecedente = NULL;

    if (posAttuale.x < 0 || posAttuale.x + W_RANA > DIM_COLS - 1) return true;
    
    if (NELL_AREA_DI_GIOCO(posAttuale)) {
        // Se si è mossa
        if (!posizioniUguali(posVecchia, posAttuale)) {    
            int i = trovaIndiceFlusso(N_FLUSSI, flussi, posAttuale.y);
            
            if (i == -1) { return true; }
            coccodrilloAttuale = trovaCoccodrilloSottoRana(posAttuale, coccodrilloAttuale, lista, i);
        }
        if(coccodrilloAttuale != NULL )mvaddch(coccodrilloAttuale->dato.posAttuale.y, coccodrilloAttuale->dato.posAttuale.x, '!');
        if (coccodrilloAttuale != NULL) {        

            if ((coccodrilloPrecedente == NULL || laRanaESuUnCoccodrilloDiverso(coccodrilloPrecedente, coccodrilloAttuale) ||
                laRanaSpostataSuStessoCoccodrillo(coccodrilloPrecedente, coccodrilloAttuale, posVecchia, posAttuale) || coccodrilloAttuale == NULL)) {

                coccodrilloPrecedente = coccodrilloAttuale;
                offsetSuCoccodrillo = trovaPosRanaSuCoccodrillo(coccodrilloAttuale->dato.posAttuale.x, posAttuale.x);
            }
            posMain->x = coccodrilloAttuale->dato.posAttuale.x + offsetSuCoccodrillo;
        } else { return true;}}
        return false;
}

/**
 * LA STRUTTURA DI UNA MANCHE
 * return:  il punteggio della manche
 */
int manche(int fd[2], Flusso flussi[N_FLUSSI], ListaCoccodrillo* listaCoccodrilli[N_FLUSSI], ListaGranata** listaGranate, pid_t pidRana, Tana tane[N_TANE], int difficolta, bool* tanaOccupata, TuttoBuffer* buffer) {
    inizializzaManche(N_TANE, N_FLUSSI, tane, flussi, listaCoccodrilli, listaGranate, fd, buffer);
    
    // Inizializzazione variabili e timer
    bool vivo = true, inAcqua = false, tanaSbagliata = false, colpito = false, collisioneGranata = false;
    time_t start, ora = 0;
    Posizione predefinita = {0,0};
    Messaggio messaggio = {predefinita, predefinita, -1, -1};
    time(&start);
    int punteggioManche = 0;

    // Variabili relative alla singola manche
    Posizione posRana = {X_PARTENZA_RANA, Y_PARTENZA_RANA};

    // Loop principale
    while(!tempoScaduto(time(&ora), start) && vivo && !*tanaOccupata){
        messaggio = ricevi(buffer);
        mvaddch(0,0,messaggio.mittente+'0');
        switch(messaggio.mittente) {
            case COCCO:
                aggiornaPosInListaCoccodrilli(messaggio, N_FLUSSI, flussi, listaCoccodrilli);
                spostaSprite(messaggio);
                break;
            case RANA:
            beep();
                if (messaggio.posAttuale.x != CODICE_GRANATA_SPARATA && messaggio.posAttuale.y != CODICE_GRANATA_SPARATA) {

                    // La rana manda lo spostamento, non la posizione
                    // Inoltriamo a spostaSprite() una versione modificata del messaggio che contiene la posizione assoluta
                    Messaggio msg;
                    msg.mittente = RANA;
                    msg.id = messaggio.id;
                    msg.posVecchia = posRana;

                    // Assegnazione punteggio sulla base del movimento
                    if(NELL_AREA_DI_GIOCO(posRana)){
                        if(messaggio.posAttuale.x != 0) punteggioManche += 5;
                        if(messaggio.posAttuale.y > 0) punteggioManche += 3;
                        if(messaggio.posAttuale.y < 0) punteggioManche += 10;
                    }
                    
                    // Aggiorniamo la posizione della rana e nel mentre controlliamo se è caduta in acqua
                    inAcqua = aggiornaPosizioneRana(&posRana, messaggio.posAttuale, flussi, listaCoccodrilli);
                    
                    *tanaOccupata = laRanaConquistatoTanaChiusa(posRana, tane, difficolta, &tanaSbagliata);

                    // Trovata la posizione, possiamo metterla nel messaggio modificato e inviarla
                    msg.posAttuale = posRana;
                    spostaSprite(msg);
                } else {
                    Posizione posPartenzaGranata;
                    posPartenzaGranata.x = posRana.x + W_RANA;
                    posPartenzaGranata.y = posRana.y;

            beep();
                    creaProcessoGranata(GRANATA, fd[1], posPartenzaGranata, AVANZAMENTO_DX, *listaGranate, buffer);    
                    
                    // posizione granata sinistra
                    posPartenzaGranata.x = posRana.x - 1;
                    creaProcessoGranata(GRANATA, fd[1], posPartenzaGranata, AVANZAMENTO_SX, *listaGranate, buffer);
                }
                break;
            case GRANATA:
                spostaSprite(messaggio);
                aggiornaPosInListaGranate(messaggio,*listaGranate);
                break;
            case PROIETTILE:
                spostaSprite(messaggio);
                collisioneGranata = gestisciCollisioneConGranate(messaggio, *listaGranate);
                if(collisioneGranata) punteggioManche += 20;
                gestisciCollisioneConRana(messaggio, posRana, &colpito);
                break;
        }
        vivo = ancoraViva(inAcqua, colpito, tanaSbagliata);
        controllaSpawnCoccodrilli(N_FLUSSI, listaCoccodrilli, flussi, fd, buffer);

        time(&ora);
        visualizzaTimer(DURATA_MANCHE_S - (ora-start));
        visualizzaPunteggio(punteggioManche);
        refresh();
    }

    //kill(pidRana, SIGKILL);

    for(int i = 0; i<N_FLUSSI; i++) {
        NodoCoccodrillo* listaCoccodrilliDiQuestoFlusso = listaCoccodrilli[i]->testa;
        NodoCoccodrillo* successivo = listaCoccodrilliDiQuestoFlusso->successivo;
        for(NodoCoccodrillo* coccodrillo = listaCoccodrilliDiQuestoFlusso; coccodrillo != NULL; coccodrillo = successivo){
            //kill(coccodrillo->dato.pid, SIGKILL);
            successivo = coccodrillo->successivo;
            free(coccodrillo);
        }
        free(listaCoccodrilli[i]);
    }

    NodoGranata* granata = (*listaGranate)->testa, *temp = NULL;
    while (granata != NULL) {
        temp = granata;
        granata = granata->successivo;
        free(temp);
    }
    free(*listaGranate);

    punteggioManche += (ora-start)*5; // 5 punti per secondo rimasto
    messaggioAltroRound(inAcqua, colpito, tanaSbagliata, *tanaOccupata);
    return punteggioManche;
}

void messaggioAltroRound(bool inAcqua, bool colpito, bool tanaSbagliata, bool tanaOccupata) {
    if (tanaOccupata) {
        TESTO_CENTRATO("TANA CONQUISTATA!"); 
    }
    else if (inAcqua) {
        TESTO_CENTRATO("CADUTO IN ACQUA!");
    }
    else if (colpito) {
        TESTO_CENTRATO("COLPITO!"); 
    } 
    else if (tanaSbagliata) {
        TESTO_CENTRATO("TANA NON VALIDA!");
    }
    else {
        TESTO_CENTRATO("TEMPO SCADUTO!"); 
    }

    refresh();
    for(int i=0; i<4; i++){
        beep();
        usleep(150000);
    }
    sleep(1);
    clear();
}