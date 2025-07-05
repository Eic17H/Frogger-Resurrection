#include "visualizzazione.h"
#include "costanti.h"
#include "listaCoccodrillo.h"
#include "ncurses.h"
#include "regole.h"
#include "sprite.h"
#include "struttureDati.h"
#include "altrecose.h"
#include <signal.h>
#include <string.h>
#include <sys/types.h>

void spostaSprite(Messaggio messaggio, int n, Flusso flussi[n], ListaCoccodrillo* lista[n]){
    Mittente mittente = messaggio.mittente;
    Posizione posVecchia = messaggio.posVecchia, posAttuale = messaggio.posAttuale;
    pid_t pid = messaggio.pid;

    char sprite[20], sprite2[20], daStampare[20], daStampare2[20], stringaVuota[20];
    int daTagliareL = 0, daTagliareR = 0, vecchioDaTagliareL, vecchioDaTagliareR, lenDaStampare = 0;

    switch(mittente) {
        case RANA: { // TODO: aggiungere sprite di sotto?
            static Posizione posAttualeRana = {X_PARTENZA_RANA, Y_PARTENZA_RANA}, posVecchiaRana = {X_PARTENZA_RANA, Y_PARTENZA_RANA};
            
            strcpy(sprite, SPR_RANA_R0);
            // puntatore+offset per spostare l'inizio, il terzo è la lunghezza che quindi è totale-tagliati
            // anziché definire W_SPR_RANA sprite diversi
            strncpy(daStampare, sprite+daTagliareL, W_SPR_RANA-daTagliareR-daTagliareL);
            /* stampa(posVecchia, tagliaStringa(posVecchia, stringaVuota)) */
            /* stampa(posAttuale, tagliaStringa(posAttuale, SPR_RANA)) */
            creaStringaVuota(W_SPR_RANA, stringaVuota);
            
            if (!fuoriSchermo(sommaPosizioni(posVecchiaRana, posVecchia), RANA, 0)) {
                posVecchiaRana = sommaPosizioni(posVecchiaRana, posVecchia);
                inizializzaColoreSprite(posVecchiaRana.y);
                mvprintw(posVecchiaRana.y, posVecchiaRana.x, "%s", stringaVuota);
            }
            if (!fuoriSchermo(sommaPosizioni(posAttualeRana, posAttuale), RANA, 0)) {
                posAttualeRana = sommaPosizioni(posAttualeRana, posAttuale);
                inizializzaColoreSprite(posAttualeRana.y);
                mvprintw(posAttualeRana.y, posAttualeRana.x, "%s", sprite);
            }
    
            if (messaggio.posAttuale.y > H_MARCIAPIEDE && messaggio.posAttuale.y < DIM_LINES - H_SPONDA) {
                int i = trovaIndiceFlusso(N_FLUSSI, flussi, messaggio.posAttuale.y);
                if (i == -1) break;
                NodoCoccodrillo* n = lista[i]->testa;
                while (n != NULL && !laRanaESuUnCoccodrilloPuntoInterrogativo(messaggio.posAttuale, n->dato.posAttuale, 0)) {
                    n = n->successivo;
                }
                if (n != NULL) {mvprintw(0, 16, "s"); refresh();}
            }
        
            break;
        }
        case GRANATA:
            strcpy(sprite, SPR_GRANATA);
            creaStringaVuota(strlen(SPR_GRANATA), stringaVuota);
            
            inizializzaColoreSprite(posVecchia.y);
            mvprintw(posVecchia.y, posVecchia.x, "%s", stringaVuota);
                    
            if (fuoriSchermo(posAttuale, GRANATA, posAttuale.x - posVecchia.x)) {
                kill(pid, SIGKILL);
                return ;
            }
            inizializzaColoreSprite(posAttuale.y);
            mvprintw(posAttuale.y, posAttuale.x, "%s", sprite);

            break;
        
        case COCCO:
            daTagliareR = calcolaDaTagliareR(posAttuale); daTagliareL = calcolaDaTagliareL(posAttuale); 
            vecchioDaTagliareR = calcolaDaTagliareR(posVecchia); vecchioDaTagliareL = calcolaDaTagliareL(posVecchia);
            // per assegnare sprite e sprite2 a seconda del verso  
            assegnaSpriteCoccodrillo(posAttuale, posVecchia, sprite, sprite2);

            lenDaStampare = strlen(sprite) - daTagliareL - daTagliareR;
            // puntatore+offset per spostare l'inizio, il terzo è la lunghezza che quindi è totale-tagliati
            // anziché definire W_SPR_RANA sprite diversi
            strncpy(daStampare, sprite+daTagliareL,lenDaStampare);
            strncpy(daStampare2, sprite2+daTagliareL,lenDaStampare);
            
            daStampare[lenDaStampare] = '\0';
            daStampare2[lenDaStampare] = '\0';

            creaStringaVuota(strlen(sprite) - vecchioDaTagliareL - vecchioDaTagliareR, stringaVuota);

            attron(COLOR_PAIR(COCCODRILLO));
            cancellaCoccodrillo(stringaVuota, posVecchia, vecchioDaTagliareL);
            stampaCoccodrillo(daStampare, daStampare2, posAttuale, daTagliareL);
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

void assegnaSpriteCoccodrillo(Posizione posAttuale, Posizione posVecchia, char* sprite1, char* sprite2) {
    strcpy(sprite1, SPR_COCCODRILLO_R0);
    strcpy(sprite2, SPR_COCCODRILLO_R1);
        
    if (posAttuale.x - posVecchia.x <= AVANZAMENTO_SX) {
        // se il coccodrillo va a sinistra, si inverte la sprite
        invertiStringa(sprite1);
        invertiStringa(sprite2);
        
        // nella sprite di sotto, '<' diventa '>'
        sprite2[0] = '>';
    }
}

void invertiStringa(char* stringa) {
    int i = 0;
    int j = strlen(stringa) - 1;
    while (i < j) {
        char tmp = stringa[i];
        stringa[i] = stringa[j];
        stringa[j] = tmp;
        i++;
        j--;
    }
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

void cancellaCoccodrillo(char stringaVuota[], Posizione posVecchia, int vecchioDaTagliareL) {
    int xDaStampare;
    // se il coccodrillo non è uscito completamente, si cancella a partire da x = 0 (perché le coordinate originali sono negative)
    if(vecchioDaTagliareL != 0) {
        xDaStampare = 0;
    }
    // si segue la x originale
    else {
        xDaStampare = posVecchia.x;
    }

    mvprintw(posVecchia.y, xDaStampare, "%s", stringaVuota);
    mvprintw(posVecchia.y + 1, xDaStampare, "%s", stringaVuota);
}

void stampaCoccodrillo(char spriteSu[], char spriteGiu[], Posizione posAttuale, int daTagliareL) {
    int xDaStampare;

    // se il coccodrillo non è uscito completamente, si stampa a partire da x = 0 (perché le coordinate originali sono negative)
    if (daTagliareL != 0) {
        xDaStampare = 0;
    }
    // si segue la x originale
    else {
        xDaStampare = posAttuale.x;
    }

    mvprintw(posAttuale.y, xDaStampare, "%s", spriteSu);
    mvprintw(posAttuale.y + 1, xDaStampare, "%s", spriteGiu);
}

void visualizzaTimer(int secondi){
    int tempoInt = daSecondiAMinutiSecondi(secondi);
    // Prende ciascuna cifra, +'0' per avere il codice ASCII, e in mezzo mette i due punti
    char tempo[6] = {tempoInt/1000+'0', tempoInt/100%10+'0', ':', tempoInt/10%10+'0', tempoInt/1%10+'0', '\0'};
    /* stampa, magari usando macro per la posizione */
}

void visualizzaPunteggio(int punteggio){
    
}