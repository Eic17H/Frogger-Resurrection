#include "regole.h"

bool tempoScaduto(time_t ora, time_t start){
    return ora-start<DURATA_MANCHE_S;
}

bool cadutoInAcqua(Posizione pos){
    // TODO
    return false;
}