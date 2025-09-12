#include "altrecose.h"
#include <signal.h>
#include <stdbool.h>
#include "struttureDati.h"
#include "costanti.h"
#include "listaCoccodrillo.h"
#include "listaGranate.h"
#include <ncurses.h>
#include "regole.h"
#include "visualizzazione.h"

bool laRanaESuUnCoccodrilloPuntoInterrogativo(Posizione rana, Posizione coccodrillo, int difficolta){
    
    // la seconda condizione è perché la sprite del coccodrillo occupa due y diverse (è composta da
    // due piani). La rana è sopra al coccodrillo anche se si trova 'al piano di sotto' 
    if(rana.y!=coccodrillo.y && rana.y-1!=coccodrillo.y) return false;
    int limDestro, limSinistro; 
    if(difficolta == 0) { //bassa: basta una casella in comune
        limSinistro = coccodrillo.x-W_RANA+1;
        limDestro = coccodrillo.x+W_COCCODRILLO;
    } else { // alta: deve stare tutta sul coccodrillo
        limSinistro = coccodrillo.x;
        limDestro = coccodrillo.x+W_COCCODRILLO-W_RANA+1;
    }
    if(rana.x < limSinistro) return false;
    if(rana.x > limDestro) return false;
    return true;
}

NodoCoccodrillo* trovaCoccodrilloSottoRana(Posizione posAttualeRana, NodoCoccodrillo* coccodrilloAttuale, ListaCoccodrillo** lista, int indiceFlusso) {
    if(!NELL_AREA_DI_GIOCO(posAttualeRana)) return NULL;
    coccodrilloAttuale = lista[indiceFlusso]->testa;
    while (coccodrilloAttuale != NULL && !laRanaESuUnCoccodrilloPuntoInterrogativo(posAttualeRana, coccodrilloAttuale->dato.posAttuale, 0)) {
        coccodrilloAttuale = coccodrilloAttuale->successivo;
    }
    //coccodrilloAttuale = lista[i]->testa;
                        //while (coccodrilloAttuale != NULL && !laRanaESuUnCoccodrilloPuntoInterrogativo(posAttualeRana, coccodrilloAttuale->dato.posAttuale, 0)) {
                        //    coccodrilloAttuale = coccodrilloAttuale->successivo;
                        //}
    return coccodrilloAttuale;
}

// restituisce la differenza fra la x della rana e quella del coccodrillo su cui si trova
int trovaPosRanaSuCoccodrillo(int xCoccodrillo, int xRana) {
    //if (xRana - xCoccodrillo < 0) return -(xRana - xCoccodrillo);
    return xRana - xCoccodrillo;
}

void gestisciCollisioneConRana(Messaggio messaggioProiettile, Posizione posRana, bool* colpito) {
    if(posizioniUguali(messaggioProiettile.posAttuale, posRana)) {
        *colpito = true;
    }
}

/**
 * messaggioProiettile: il messaggio di un solo proiettile
 * listaGranate: tutte le granate
 * return:  true - se c'è stata una collisione
 *          false - se non c'è stata una collisione
 */
bool gestisciCollisioneConGranate(Messaggio messaggioProiettile, ListaGranata* listaGranate) {
    NodoGranata* granata = listaGranate->testa, *granataColpita = NULL;

    while (granata != NULL && granataColpita == NULL) {
        if (posizioniUguali(granata->dato.posAttuale, messaggioProiettile.posAttuale)) {
            granataColpita = granata;
            beep();
        }
        granata = granata->successivo;
    }
    if (granataColpita != NULL) {
        //kill(granataColpita->dato.pid, SIGKILL);
        //kill(messaggioProiettile.pid, SIGKILL);
        return true;
    }
    return false;
}

bool laRanaESuTanaPuntoInterrogativo(Posizione rana, Tana tana, int difficolta) {
    int limDestro, limSinistro;

    if(difficolta == 0) { //bassa: basta una casella in comune
        limSinistro = tana.xInizio-W_RANA+1;
        limDestro = tana.xFine;
    }
    else { // alta: deve stare tutta sul coccodrillo
        limSinistro = tana.xInizio;
        limDestro = tana.xFine-W_RANA+1;
    }
    if(rana.x < limSinistro) return false;
    if(rana.x > limDestro) return false;
    return true;
}

bool laRanaConquistatoTanaChiusa(Posizione rana, Tana tane[], int difficolta, bool* tanaSbagliata) {
    if (rana.y > H_TANA+1) return false;

    int i = 0;
    bool inTana = false;

    while (i < N_TANE && !inTana) {
        inTana = laRanaESuTanaPuntoInterrogativo(rana, tane[i], 0);
        i++;
    }

    if (inTana) {
        if (!tane[i-1].chiusa) {
            tane[i-1].chiusa = true;
            return true;
        }
        else *tanaSbagliata = true;
    }
    
    return false;
}

bool laRanaESuUnCoccodrilloDiverso(NodoCoccodrillo* precedente, NodoCoccodrillo* attuale) {
    return (precedente != NULL && attuale != NULL && precedente->dato.id != attuale->dato.id);
}

bool laRanaSpostataSuStessoCoccodrillo(NodoCoccodrillo* precedente, NodoCoccodrillo* attuale, Posizione posVecchiaRana, Posizione posNuovaRana) {
    return !laRanaESuUnCoccodrilloDiverso(precedente, attuale) && posNuovaRana.x != posVecchiaRana.x; 
}

int trovaIndiceFlusso(int n, Flusso flussi[n], int yDesiderata) {
   int i = 0;

   // la seconda condizione è perché la sprite del coccodrillo occupa due y diverse (è composta da
   // due piani). La rana è sopra al coccodrillo anche se si trova 'al piano di sotto' 
    while (flussi[i].posIniziale.y != yDesiderata && flussi[i].posIniziale.y != yDesiderata-1) {
        i++;
        if (i >= n) return -1;
    }
    return i;
}

// 136 -> 216 (perché è 2:16)
int daSecondiAMinutiSecondi(int tempoInSecondi){
    int secondi = tempoInSecondi%60;
    int minuti = tempoInSecondi/60;
    return minuti*100+secondi;
}

Posizione sommaPosizioni(Posizione pos1, Posizione pos2) {
    pos1.x += pos2.x;
    pos1.y += pos2.y;
    return pos1;
}

bool posizioniUguali(Posizione pos1, Posizione pos2) {
    return pos1.x == pos2.x && pos1.y == pos2.y;
}

bool ricominciaPartita() {
    int ch;
    // per ripulire il buffer di input
    flushinp();

    clear();
    TESTO_CENTRATO("NUOVA PARTITA? [s/n]")
    refresh();
    
    do {
        ch = getch();
    } while (ch != 's' && ch != 'n');
    
    clear();
    
    if (ch == 's') return true;
    return false;
}