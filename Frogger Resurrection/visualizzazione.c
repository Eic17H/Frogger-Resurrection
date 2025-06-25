#include "visualizzazione.h"
#include "ncurses.h"
#include "sprite.h"

void spostaSprite(Mittente mittente, Posizione posVecchia, Posizione posAttuale){
    char sprite[20], daStampare[20], stringaVuota[20];
    int daTagliareL = 0, daTagliareR = 0;
    creaStringaVuota(20, stringaVuota);

    switch(mittente) {
        case RANA:
            // assegna daTagliareL e daTagliareR
            strcpy(sprite, SPR_RANA_R0);
            // puntatore+offset per spostare l'inizio, il terzo è la lunghezza che quindi è totale-tagliati
            // anziché definire W_SPR_RANA sprite diversi
            strncpy(daStampare, sprite+daTagliareL, W_SPR_RANA-daTagliareR-daTagliareL);
            /* stampa(posVecchia, tagliaStringa(posVecchia, stringaVuota)) */
            /* stampa(posAttuale, tagliaStringa(posAttuale, SPR_RANA)) */
            tagliaStringa(stringaVuota, 20 - W_SPR_RANA);
            mvprintw(posAttuale.y, posAttuale.x, "%s", sprite);
            mvprintw(posVecchia.y, posVecchia.x, "%s", stringaVuota);
            break;
        // gli altri sono analoghi
        case GRANATA:
            break;
        default:
            break;
    }
    refresh();
}

void creaStringaVuota(int n, char stringa[n]) {
    for (int i = 0; i < n; i++) {
        stringa[i] = ' ';
    }
}

void tagliaStringa(char stringa[], int daTagliare) {
    int n = strlen(stringa) - daTagliare;

    strncpy(stringa, stringa, n);
}

void visualizzaTimer(int secondi){
    int tempoInt = daSecondiAMinutiSecondi(secondi);
    // Prende ciascuna cifra, +'0' per avere il codice ASCII, e in mezzo mette i due punti
    char tempo[6] = {tempoInt/1000+'0', tempoInt/100%10+'0', ':', tempoInt/10%10+'0', tempoInt/1%10+'0', '\0'};
    /* stampa, magari usando macro per la posizione */
}


void visualizzaPunteggio(int punteggio){
    
}