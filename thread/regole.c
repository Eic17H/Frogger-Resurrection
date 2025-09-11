#include "regole.h"
#include "costanti.h"
#include "struttureDati.h"

bool tempoScaduto(time_t ora, time_t start){
    return ora-start>=DURATA_MANCHE_S;
}

bool fuoriSchermo(Posizione pos, Mittente mittente, int verso) {
    switch(mittente) {
        case PROIETTILE:
        case GRANATA:
            if (verso >= AVANZAMENTO_DX && pos.x > DIM_COLS - 1 ||
                verso <= AVANZAMENTO_DX && pos.x < 0) 
                return true;
        
        break;
        case COCCO:
            if (verso >= AVANZAMENTO_DX && pos.x > DIM_COLS - 1 ||
                verso <= AVANZAMENTO_SX && pos.x < -W_COCCODRILLO + 1)
                return true;

        break;
        case RANA:
            // la rana non può avere y = 0 perché la riga 0 è dedicata a punteggio e timer
            if (pos.x + W_RANA > DIM_COLS -1 || pos.x < 0 || pos.y > DIM_LINES -1 || pos.y <= 0)
            return true;

        break;
        default:
        break;
    }

    return false;
}

bool ancoraViva(bool inAcqua, bool colpito, bool tanaSbagliata) {
    return !(inAcqua || colpito || tanaSbagliata);
}