#include "visualizzazione.h"

void spostaSprite(Mittente mittente, Posizione posVecchia, Posizione posAttuale){
    char sprite[20], daStampare[20];
    int daTagliareL, daTagliareR;
    switch(mittente) {
        case RANA:
            // assegna daTagliareL e daTagliareR
            strcpy(sprite, SPR_RANA_R0);
            // puntatore+offset per spostare l'inizio, il terzo è la lunghezza che quindi è totale-tagliati
            // anziché definire W_SPR_RANA sprite diversi
            strncpy(daStampare, sprite+daTagliareL, W_SPR_RANA-daTagliareR-daTagliareL);
            /* stampa(posVecchia, tagliaStringa(posVecchia, stringaVuota)) */
            /* stampa(posAttuale, tagliaStringa(posAttuale, SPR_RANA)) */
            break;
        // gli altri sono analoghi
        case GRANATA:
            break;
        default:
            break;
    }
}

void visualizzaTimer(int secondi){
    int tempoInt = daSecondiAMinutiSecondi(secondi);
    // Prende ciascuna cifra, +'0' per avere il codice ASCII, e in mezzo mette i due punti
    char tempo[6] = [tempoInt/1000+'0', tempoInt/100%10+'0', ':', tempoInt/10%10+'0', tempoInt/1%10+'0', '\0'];
    /* stampa, magari usando macro per la posizione */
}


void visualizzaPunteggio(int punteggio){
    
}