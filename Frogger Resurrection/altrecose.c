#include <stdbool.h>
#include "struttureDati.h"
#include "costanti.h"

bool laRanaESuUnCoccodrilloPuntoInterrogativo(Posizione rana, Posizione coccodrillo, int difficolta){
    if(rana.y!=coccodrillo.y) return false;
    int limDestro, limSinistro;
    if(ledgeDifficolta(difficolta)) { //bassa: basta una casella in comune
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

bool laRanaESuUnaTanaPuntoInterrogativo(Posizione rana, Tana tana, int difficolta) {
    int limDestro, limSinistro;

    if(ledgeDifficolta(difficolta)) { //bassa: basta una casella in comune
        limSinistro = tana.xInizio-W_RANA+1;
        limDestro = tana.xFine+W_COCCODRILLO;
    } else { // alta: deve stare tutta sul coccodrillo
        limSinistro = tana.xInizio;
        limDestro = tana.xFine+W_COCCODRILLO-W_RANA+1;
    }
    if(rana.x < limSinistro) return false;
    if(rana.x > limDestro) return false;
    return true;

    /* chiamante potrebbe funzionare così:
    if (rana.y <= altezzaSponda()) {
        int i = 0;
        bool inTana = false;
        while (i < N_TANE && !inTana) {
                inTana = laRanaESuUnaTanaPuntoInterrogativo(messaggio.posAttuale, tane[i], 0);
                i++;
        }
        if (inTana) tane[i-1].chiusa = true;
        else hai perso la manche
    }*/
}

// 136 -> 216 (perché è 2:16)
int daSecondiAMinutiSecondi(int tempoInSecondi){
    int secondi = tempoInSecondi%60;
    int minuti = tempoInSecondi/60;
    return minuti*100+secondi;
}