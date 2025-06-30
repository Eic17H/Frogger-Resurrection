#include "visualizzazione.h"
#include "costanti.h"
#include "ncurses.h"
#include "sprite.h"
#include "struttureDati.h"

void spostaSprite(Mittente mittente, Posizione posVecchia, Posizione posAttuale){
    char sprite[20], sprite2[20], daStampare[20], daStampare2[20], stringaVuota[20];
    int daTagliareL = 0, daTagliareR = 0, lenDaStampare = 0;

    switch(mittente) {
        case RANA:
            // assegna daTagliareL e daTagliareR
            strcpy(sprite, SPR_RANA_R0);
            // puntatore+offset per spostare l'inizio, il terzo è la lunghezza che quindi è totale-tagliati
            // anziché definire W_SPR_RANA sprite diversi
            strncpy(daStampare, sprite+daTagliareL, W_SPR_RANA-daTagliareR-daTagliareL);
            /* stampa(posVecchia, tagliaStringa(posVecchia, stringaVuota)) */
            /* stampa(posAttuale, tagliaStringa(posAttuale, SPR_RANA)) */
            creaStringaVuota(W_SPR_RANA, stringaVuota);
            
            inizializzaColoreSprite(posVecchia.y);
            mvprintw(posVecchia.y, posVecchia.x, "%s", stringaVuota);

            inizializzaColoreSprite(posAttuale.y);
            mvprintw(posAttuale.y, posAttuale.x, "%s", sprite);

            break;
        
        case GRANATA:
            strcpy(sprite, SPR_GRANATA);
            creaStringaVuota(strlen(SPR_GRANATA), stringaVuota);
            
            inizializzaColoreSprite(posVecchia.y);
            mvprintw(posVecchia.y, posVecchia.x, "%s", stringaVuota);
                    
            inizializzaColoreSprite(posAttuale.y);
            mvprintw(posAttuale.y, posAttuale.x, "%s", sprite);

            break;
        
        case COCCO:
            daTagliareR = calcolaDaTagliareR(posAttuale); daTagliareL = calcolaDaTagliareL(posAttuale); 
            strcpy(sprite, SPR_COCCODRILLO_R0);
            strcpy(sprite2, SPR_COCCODRILLO_R1);

            lenDaStampare = strlen(sprite) - daTagliareL - daTagliareR;
            // puntatore+offset per spostare l'inizio, il terzo è la lunghezza che quindi è totale-tagliati
            // anziché definire W_SPR_RANA sprite diversi
            strncpy(daStampare, sprite+daTagliareL,lenDaStampare);
            strncpy(daStampare2, sprite2+daTagliareL,lenDaStampare);
            
            daStampare[lenDaStampare] = '\0';
            daStampare2[lenDaStampare] = '\0';

            creaStringaVuota(strlen(sprite) - calcolaDaTagliareL(posVecchia) - calcolaDaTagliareR(posVecchia), stringaVuota);

            attron(COLOR_PAIR(COCCODRILLO));
            if (daTagliareL == 0) {
                mvprintw(posVecchia.y, posVecchia.x, "%s", stringaVuota);
                mvprintw(posVecchia.y + 1, posVecchia.x, "%s", stringaVuota);

                mvprintw(posAttuale.y, posAttuale.x, "%s", daStampare);
                mvprintw(posAttuale.y + 1, posAttuale.x, "%s", daStampare2);            
            }
            else {
                mvprintw(posVecchia.y, 0, "%s", stringaVuota);
                mvprintw(posVecchia.y + 1, 0, "%s", stringaVuota);

                mvprintw(posAttuale.y, 0, "%s", daStampare);
                mvprintw(posAttuale.y + 1, 0, "%s", daStampare2);
            }
            
            break;
        default:
            break;
    }
}

void creaStringaVuota(int n, char* stringa) {
    for (int i = 0; i < n; i++) {
        stringa[i] = ' ';
    }
    stringa[n] = '\0';
}

int calcolaDaTagliareR(Posizione pos) {
    // se il coccodrillo sta scomparendo gradualmente dal lato destro dello schermo 
    if (pos.x >= DIM_COLS-7 && pos.x <= DIM_COLS-1) {
        // se per esempio x = DIM_COLS - 1, facendo DIM_COLS - x si ottiene che al limite dello schermo c'è un solo pixel
        return 8 - (DIM_COLS - pos.x); 
    }
    // se il coccodrillo è intero, si stamperà la sprite di default (in posizione 0)
    return 0;
}

int calcolaDaTagliareL(Posizione pos) {
    // se il coccodrillo sta sbucando gradualmente dal lato sinistro dello schermo (non si è ancora mostrato interamente)
    if (pos.x >= -W_COCCODRILLO+1 && pos.x <= -W_COCCODRILLO+7) {
        // i pixel che sporgono sono dati dalla lunghezza del coccodrillo + x (che qui è sempre negativa). Se x = -W_COCCODRILLO + 1, sta sporgendo un solo pixel 
        return pos.x * -1;
    }
    // se il coccodrillo è intero, si stamperà la sprite di default (in posizione 0)
    return 0;
}

char* tagliaStringa(char stringa[], int lunghezza) {
    strncpy(stringa, stringa, lunghezza);
    return stringa;
}

void inizializzaColoreSprite(int ySprite) {
    if (ySprite <= H_SPONDA) {
        attron(COLOR_PAIR(SPONDA));
    }
    else if (ySprite >= DIM_LINES - H_MARCIAPIEDE) {
        attron(COLOR_PAIR(MARCIAPIEDE));
    }
    else {
        attron(COLOR_PAIR(ACQUA));
    }
}

void visualizzaTimer(int secondi){
    int tempoInt = daSecondiAMinutiSecondi(secondi);
    // Prende ciascuna cifra, +'0' per avere il codice ASCII, e in mezzo mette i due punti
    char tempo[6] = {tempoInt/1000+'0', tempoInt/100%10+'0', ':', tempoInt/10%10+'0', tempoInt/1%10+'0', '\0'};
    /* stampa, magari usando macro per la posizione */
}

void visualizzaPunteggio(int punteggio){
    
}