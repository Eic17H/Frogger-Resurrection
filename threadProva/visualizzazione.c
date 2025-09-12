#include "visualizzazione.h"
#include "costanti.h"
#include "listaCoccodrillo.h"
#include "ncurses.h"
#include "regole.h"
#include "sprite.h"
#include "struttureDati.h"
#include "altrecose.h"
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>

void spostaSprite(TuttoBuffer* buffer, Messaggio messaggio){
    Mittente mittente = messaggio.mittente;
    Posizione posVecchia = messaggio.posVecchia, posAttuale = messaggio.posAttuale;
    pid_t pid = messaggio.id;
    static int px = 0; 
    char sprite[20], sprite2[20], daStampare[20], daStampare2[20], stringaVuota[20];
    int daTagliareL = 0, daTagliareR = 0, vecchioDaTagliareL, vecchioDaTagliareR, lenDaStampare = 0;
    static NodoCoccodrillo *coccodrilloPrecedente = NULL, *coccodrilloAttuale = NULL;
    switch(mittente) {
        case RANA: {

            strcpy(sprite, SPR_RANA_R0);
            strcpy(sprite2, SPR_RANA_R1);
            // puntatore+offset per spostare l'inizio, il terzo è la lunghezza che quindi è totale-tagliati
            // anziché definire W_SPR_RANA sprite diversi
            strncpy(daStampare, sprite+daTagliareL, W_SPR_RANA-daTagliareR-daTagliareL);
            /* stampa(posVecchia, tagliaStringa(posVecchia, stringaVuota)) */
            /* stampa(posAttuale, tagliaStringa(posAttuale, SPR_RANA)) */
            creaStringaVuota(W_SPR_RANA, stringaVuota);

            Posizione posVecchiaRana = messaggio.posVecchia;
            Posizione posAttualeRana = messaggio.posAttuale;

            // Se, verticalmente, si trova dentro l'area di gioco
        
            inizializzaColoreSprite(posVecchiaRana.y);

            sem_wait(buffer->semLiberi);
            pthread_mutex_lock(buffer->mutex);
            
            mvprintw(messaggio.posVecchia.y-1, messaggio.posVecchia.x, "%s", stringaVuota);
            mvprintw(messaggio.posVecchia.y, messaggio.posVecchia.x, "%s", stringaVuota);

            inizializzaColoreSprite(posAttualeRana.y);
            mvprintw(messaggio.posAttuale.y-1, messaggio.posAttuale.x, "%s", sprite);
            mvprintw(messaggio.posAttuale.y, messaggio.posAttuale.x, "%s", sprite2);
            refresh();
            pthread_mutex_unlock(buffer->mutex);
            sem_post(buffer->semLiberi);

            break;
        }
        case PROIETTILE:
        case GRANATA:
            strcpy(sprite, SPR_GRANATA);
            creaStringaVuota(strlen(SPR_GRANATA), stringaVuota);
            
            inizializzaColoreSprite(posVecchia.y);
            mvprintw(posVecchia.y, posVecchia.x, "%s", stringaVuota);
                    
            if (fuoriSchermo(posAttuale, mittente, posAttuale.x - posVecchia.x)) {
                //kill(pid, SIGKILL);
                return ;
            }
            inizializzaColoreSprite(posAttuale.y);
            if(mittente == PROIETTILE) attron(COLOR_PAIR(COCCODRILLO_ROSSO));
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

            selezionaColoreCoccodrillo(posAttuale.x - posVecchia.x, messaggio.staPerSparare);
            cancellaCoccodrillo(buffer, stringaVuota, posVecchia, vecchioDaTagliareL);
            
            if (fuoriSchermo(posAttuale, COCCO, posAttuale.x - posVecchia.x)) return ;
            stampaCoccodrillo(buffer, daStampare, daStampare2, posAttuale, daTagliareL);
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

void selezionaColoreCoccodrillo(int versoCoccodrillo, bool staPerSparare) {
    if (staPerSparare) {
        attron(COLOR_PAIR(COCCODRILLO_ROSSO));
        return;
    }
    
    // se il coccodrillo va da sx a dx
    if (versoCoccodrillo > 0) {
        attron(COLOR_PAIR(COCCODRILLO_VERDE));
        return;
    }
    // altrimenti
    attron(COLOR_PAIR(COCCODRILLO_GIALLO));
}

void cancellaCoccodrillo(TuttoBuffer* buffer, char stringaVuota[], Posizione posVecchia, int vecchioDaTagliareL) {
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

void stampaCoccodrillo(TuttoBuffer* buffer, char spriteSu[], char spriteGiu[], Posizione posAttuale, int daTagliareL) {
    int xDaStampare;

    // se il coccodrillo non è uscito completamente, si stampa a partire da x = 0 (perché le coordinate originali sono negative)
    if (daTagliareL != 0) {
        xDaStampare = 0;
    }
    // si segue la x originale
    else {
        xDaStampare = posAttuale.x;
    }

    sem_wait(buffer->semLiberi);
    pthread_mutex_lock(buffer->mutex);
    
    mvprintw(posAttuale.y, xDaStampare, "%s", spriteSu);
    mvprintw(posAttuale.y + 1, xDaStampare, "%s", spriteGiu);
    
    pthread_mutex_unlock(buffer->mutex);
    sem_post(buffer->semLiberi);
}

void visualizzaTimer(TuttoBuffer* buffer, int secondi){
    int tempoInt = daSecondiAMinutiSecondi(secondi);
    // Prende ciascuna cifra, +'0' per avere il codice ASCII, e in mezzo mette i due punti
    char tempo[6] = {tempoInt/1000+'0', tempoInt/100%10+'0', ':', tempoInt/10%10+'0', tempoInt/1%10+'0', '\0'};
    /* stampa, magari usando macro per la posizione */
    
    attron(COLOR_PAIR(NERO));
    sem_wait(buffer->semLiberi);
    pthread_mutex_lock(buffer->mutex);
    
    mvprintw(Y_TIMER_MANCHE, X_TIMER_MANCHE, "%s", tempo);
    
    pthread_mutex_unlock(buffer->mutex);
    sem_post(buffer->semLiberi);
}

void visualizzaPunteggio(TuttoBuffer* buffer, int punteggio){
    char punteg[5] = {punteggio/1000+'0', punteggio/100%10+'0', punteggio/10%10+'0', punteggio%10+'0', '\0'};
    attron(COLOR_PAIR(NERO));

    sem_wait(buffer->semLiberi);
    pthread_mutex_lock(buffer->mutex);

    mvprintw(Y_TIMER_MANCHE, X_TIMER_MANCHE+10, "%s", punteg);
    
    pthread_mutex_unlock(buffer->mutex);
    sem_post(buffer->semLiberi);
}

void visualizzaVite(TuttoBuffer* buffer, int vite) {
    if (vite < 1) return;
    int x = X_PARTENZA_STAMPA_VITE;

    // eliminazione vecchie icone (fino a max vite * 2 perché ogni icona è separata da uno spazio)
    char stringaVuota[N_VITE*2];
    creaStringaVuota(strlen(stringaVuota), stringaVuota);
    
    attron(COLOR_PAIR(NERO));

    sem_wait(buffer->semLiberi);
    pthread_mutex_lock(buffer->mutex);

    mvprintw(Y_STAMPA_VITE, X_PARTENZA_STAMPA_VITE, "%s", stringaVuota);
    
    for (int i = 0; i < vite; i++) {
        mvprintw(Y_STAMPA_VITE, x, "*");
        // ogni icona è seguita da uno spazio
        x+=2;
    }
    refresh();
    
    pthread_mutex_unlock(buffer->mutex);
    sem_post(buffer->semLiberi);
}

void visualizzaRoundRimasti(TuttoBuffer* buffer, int roundRimasti) {
    char messaggioDaStampare[] = "Round rimasti: ";
    int lunghezzaMsgDaStampare = strlen(messaggioDaStampare);
    char stringaVuota[lunghezzaMsgDaStampare + 2];
    // eliminazione messaggio + vecchio numero (fino a max 2 cifre: difficile che i round arrivino alle centinaia)
    creaStringaVuota(strlen(stringaVuota), stringaVuota);
    
    attron(COLOR_PAIR(NERO));
    
    sem_wait(buffer->semLiberi);
    pthread_mutex_lock(buffer->mutex);

    mvprintw(Y_STAMPA_ROUND_RIMASTI, X_STAMPA_ROUND_RIMASTI, "%s", stringaVuota);
    
    mvprintw(Y_STAMPA_ROUND_RIMASTI, X_STAMPA_ROUND_RIMASTI, "%s%d", messaggioDaStampare, roundRimasti);

    refresh();
    
    pthread_mutex_unlock(buffer->mutex);
    sem_post(buffer->semLiberi);

}

void messaggioFinePartita(TuttoBuffer* buffer, int nTaneOccupate, int punti) {
    sem_wait(buffer->semLiberi);
    pthread_mutex_lock(buffer->mutex);
    if (nTaneOccupate != N_TANE) {
        TESTO_CENTRATO("HAI PERSO :(");
    }
    else {
        TESTO_CENTRATO("HAI VINTO!!");
        char punteggio[15] = "Punteggio: 0000";
        punteggio[11] = punti/1000%10+'0';
        punteggio[12] = punti/100%10+'0';
        punteggio[13] = punti/10%10+'0';
        punteggio[14] = punti/1%10+'0';
        // Il testo compare gradualmente
        for(int i=0; i<15; i++) {
            mvaddch(LINES/2+1, COLS/2-7+i, punteggio[i]);
            usleep(100000);
            refresh();
        }
    }
    
    refresh();
    pthread_mutex_unlock(buffer->mutex);
    sem_post(buffer->semLiberi);

    sleep(2);
        
}