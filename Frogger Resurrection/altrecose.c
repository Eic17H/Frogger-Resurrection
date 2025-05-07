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