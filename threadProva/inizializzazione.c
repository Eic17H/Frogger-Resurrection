#include <stdlib.h>
#include <ncurses.h>

#include "inizializzazione.h"
#include "listaCoccodrillo.h"
#include "processi.h"
#include "struttureDati.h"
#include "thread.h"
#include "visualizzazione.h"
#include "costanti.h"
#include "coccodrillo.h"
#include "sparo.h"
#include "listaCoccodrillo.h"
#include "listaGranate.h"

void inizializzaNcurses() {
    initscr();
    keypad(stdscr, true);
    cbreak();
    nodelay(stdscr, true);
    noecho();
    curs_set(0);
}

void messaggioBenvenuto() {
    TESTO_CENTRATO("BENVENUTO!");
    refresh();
    sleep(2);
    clear();
}

void inizializzaColori() {
    start_color();
    init_pair(MARCIAPIEDE, COLOR_BLACK, COLOR_WHITE);
    init_pair(ACQUA, COLOR_WHITE, COLOR_BLUE);
    init_pair(SPONDA, COLOR_BLACK, COLOR_GREEN);
    init_pair(COCCODRILLO_VERDE, COLOR_GREEN, COLOR_BLUE);
    init_pair(COCCODRILLO_GIALLO, COLOR_YELLOW, COLOR_BLUE);
    init_pair(COCCODRILLO_ROSSO, COLOR_RED, COLOR_BLUE);
    init_pair(RANA_SU_COCCODRILLO, COLOR_WHITE, COLOR_GREEN);
    init_pair(NERO, COLOR_WHITE, COLOR_BLACK);
    // sfondo blu
    bkgd(COLOR_PAIR(ACQUA));
}

void coloraAmbienteGioco(TuttoBuffer* buffer) {

    // barra superiore        
    attron(COLOR_PAIR(NERO));
    for(int i=0; i<DIM_COLS+1; i++){
        sem_wait(buffer->semLiberi);
        pthread_mutex_lock(buffer->mutex);

        mvaddch(0, i, ' ');
        
        pthread_mutex_unlock(buffer->mutex);
        sem_post(buffer->semLiberi);
    	
    }
    // sponda verde
    attron(COLOR_PAIR(SPONDA));
    for(int i=0; i< DIM_COLS+1; i++){
    	for(int j=0; j< H_SPONDA; j++){
            sem_wait(buffer->semLiberi);
            pthread_mutex_lock(buffer->mutex);

            mvaddch(j+1, i, ' ');
            pthread_mutex_unlock(buffer->mutex);
            sem_post(buffer->semLiberi);
    		
    	}
    }
    // marciapiede bianco        
    attron(COLOR_PAIR(MARCIAPIEDE));
    for(int i=0; i<DIM_COLS+1; i++){
    	for(int j=0; j< H_MARCIAPIEDE; j++){
            sem_wait(buffer->semLiberi);
            pthread_mutex_lock(buffer->mutex);
            mvaddch(LINES-1 -j, i, ' ');
    
            pthread_mutex_unlock(buffer->mutex);
            sem_post(buffer->semLiberi);
    		
    	}
    }
    sem_wait(buffer->semLiberi);
    pthread_mutex_lock(buffer->mutex);

    refresh();
    pthread_mutex_unlock(buffer->mutex);
    sem_post(buffer->semLiberi);
}

void adattaFinestra() {
    if (LINES == DIM_LINES && COLS == DIM_COLS) return ;

    int dim1 = strlen("TU: LINES = xx COLS = yy")/2;  
    int dim2 = strlen("DIM: LINES = xx COLS = yy")/2;

    TESTO_CENTRATO("Ridimensiona lo schermo per un'esperienza ottimale");
    refresh();
    sleep(2);

    clear();
    refresh();
    
    while (LINES != DIM_LINES || COLS != DIM_COLS) {
        WINDOW* indicaz_win = newwin(DIM_LINES, DIM_COLS, 0, 0);
        box(indicaz_win, ACS_VLINE, ACS_HLINE);
    
        TESTO_COORD_CENTRATO(indicaz_win, "TU: LINES = %d COLS = %d", LINES, COLS);
        TESTO_COORD_CENTRATO2(indicaz_win, "DIM: LINES = %d COLS = %d", DIM_LINES, DIM_COLS);

        wrefresh(indicaz_win);
        
        delwin(indicaz_win);
    }
    usleep(10000);
    clear();
}

void creaTane(int nTane, Tana tane[nTane]) {
    int wTotTane = W_TANA*nTane, wSpondaLibera = DIM_COLS - wTotTane, spazioFraTane = wSpondaLibera / N_SEPARATORI_TANE - 0.5; 
    int indiceTane = 0;
    // si sposta il cursore nel punto giusto per disegnare la nuova tana
    for (int j = spazioFraTane; j < DIM_COLS; j += W_TANA + spazioFraTane) { 
        // inizialmente tutte le tane sono aperte
        tane[indiceTane].chiusa = false;
        tane[indiceTane].y = H_TANA;
        
        tane[indiceTane].xInizio = j;
        tane[indiceTane].xFine = j + W_TANA;
        indiceTane++;
    }
}

void disegnaTane(TuttoBuffer* buffer, int nTane, Tana tane[nTane]) {
    // W_TANA+1 per includere il terminatore di stringa /0
    char spriteTanaAperta[H_TANA][W_TANA+1] = {SPR_TANA_APERTA1, SPR_TANA_APERTA2, SPR_TANA_APERTA3, SPR_TANA_APERTA4};
    char spriteTanaChiusa[H_TANA][W_TANA+1] = {SPR_TANA_CHIUSA1, SPR_TANA_CHIUSA2, SPR_TANA_CHIUSA3, SPR_TANA_CHIUSA4};
    int indiceTana;
    for (int i = 0; i < nTane; i++) {
        indiceTana = 0;
        // si disegna la tana partendo dall'alto fino ad arrivare alla riva
        for (int j = 1; j <= H_TANA; j++) {
            attron(COLOR_PAIR(SPONDA));
            
            sem_wait(buffer->semLiberi);
            pthread_mutex_lock(buffer->mutex);

    
            if (!tane[i].chiusa) {
                mvaddstr(j, tane[i].xInizio, spriteTanaAperta[indiceTana]);
            }
            else {
                mvaddstr(j, tane[i].xInizio, spriteTanaChiusa[indiceTana]);
            }

            pthread_mutex_unlock(buffer->mutex);
            sem_post(buffer->semLiberi);
            indiceTana++;
        }
    }
}

int generaVersoFlussoIniziale() {
    // generato numero casuale: 0 o 1
    if (rand() % (AVANZAMENTO_DX + 1) == AVANZAMENTO_DX) return AVANZAMENTO_DX;
    return AVANZAMENTO_SX;  
}

void inizializzaArrayFlussi(int nFlussi, Flusso flussi[nFlussi]) {
    int yPartenza = DIM_LINES - H_MARCIAPIEDE - 1 - 1;
    int versoFlusso = generaVersoFlussoIniziale();

    //il primo -1 è perché il coccodrillo ha anche una sprite inferiore, il secondo è perché non deve partire a un pixel di distanza rispetto al marciapiede 
    for (int i = 0; i < nFlussi; i++) {
        flussi[i].verso = versoFlusso;

        if (versoFlusso == AVANZAMENTO_DX) {
            // per far si che il coccodrillo si mostri grdualmente (senza essere disegnato fuori schermo)
            flussi[i].posIniziale.x = -W_COCCODRILLO+1;  
        }
        else {
            flussi[i].posIniziale.x = DIM_COLS - 1;
        }
        flussi[i].posIniziale.y = yPartenza;
        
        // velocità e distanza fra coccodrilli del flusso sono estratti casualmente
        flussi[i].velocità = MIN_VELOCITA_COCCO + rand() % (MAX_VELOCITA_COCCO - MIN_VELOCITA_COCCO + 1);
        flussi[i].distanzaCoccodrilli = MIN_SPAZIO_FRA_COCCO + rand() % (MAX_SPAZIO_FRA_COCCO - MIN_SPAZIO_FRA_COCCO + 1);
        yPartenza -= DISTANZA_FLUSSI + 1; // +1 per la sprite inferiore del coccodrillo 
    
        // i versi si alternano
        versoFlusso *= -1;
    }
}

void inizializzaListaCoccodrilli(int nFlussi, ListaCoccodrillo* lista[nFlussi]) {
    for (int i = 0; i < nFlussi; i++) {
        lista[i] = creaListaVuotaCoccodrillo();
    }
}

void creaCoccodrilliIniziali(int n, int fd[n], int nFlussi, Flusso flussi[nFlussi], ListaCoccodrillo* lista[nFlussi], TuttoBuffer* buffer) {
    for (int i = 0; i < nFlussi; i++) {
        creaCoccodrillo(lista[i], fd, flussi[i], buffer);
    }
}

void* thread_coccodrillo(void* arg) {
    ArgsThreadCoccodrillo args = *(ArgsThreadCoccodrillo*) arg;
    free(arg);
    coccodrillo(args.flussoAttuale, args.buffer);

    return NULL;
}

void creaCoccodrillo(ListaCoccodrillo* lista, int fd[], Flusso flusso, TuttoBuffer* buffer) {
    pid_t pidCoccodrillo;
    Coccodrillo nuovoCoccodrillo;
    NodoCoccodrillo* nuovoNodo = NULL;

    ArgsThreadCoccodrillo* argsThreadCoccodrillo = malloc(sizeof(ArgsThreadCoccodrillo));
    if (argsThreadCoccodrillo == NULL) {endwin(); perror("Errore allocazione ArgsThreadCoccodrillo"); return ;}
    argsThreadCoccodrillo->buffer = buffer;
    argsThreadCoccodrillo->flussoAttuale = flusso;
    pthread_t tid;

    if(pthread_create(&tid, NULL, &thread_coccodrillo, argsThreadCoccodrillo) != 0) { endwin(); perror("fallito thread coccodrillo"); _exit(2); };
    nuovoCoccodrillo = assegnaDatiCoccodrillo(tid, flusso.posIniziale, flusso);
    nuovoNodo = creaNodoCoccodrillo(nuovoCoccodrillo);
    pushCoccodrillo(lista, nuovoNodo);

    //free(argsThreadCoccodrillo);
}

//pid_t creaRana(int n, int fd[n], TuttoBuffer* buffer) {
//    pid_t pidRana = fork();
//
//    // ERRORE
//    if (forkFallita(pidRana)){
//        endwin();
//        perror("chiamata fork() rana");
//        _exit(2);
//    }
//    // RANA
//    else if(processoFiglio(pidRana)){
//        close(fd[0]);
//        rana(fd[1], buffer);
//    }
//    // MAIN
//    return pidRana;
//}

typedef struct 
{
    Mittente mittente;
    int fdScrittura;
    Posizione posPartenza;
    int direzione;
    TuttoBuffer* buffer;
}ArgsThreadSparo;

void* thread_sparo(void* arg) {
    ArgsThreadSparo args = *(ArgsThreadSparo*) arg;
    free(arg);
    sparo(args.mittente, args.posPartenza, args.direzione, args.buffer);
    return NULL;
}

void creaProcessoGranata(Mittente mittente, int fdScrittura, Posizione posPartenza, int direzione, ListaGranata* listaGranate, TuttoBuffer* buffer) {
    Granata nuovaGranata;
    NodoGranata* nuovoNodo = NULL;
    
    ArgsThreadSparo* argsThreadSparo = malloc(sizeof(ArgsThreadSparo));
    if (argsThreadSparo == NULL) {endwin(); perror("Errore allocazione ArgsThreadSparo"); return ;}
    argsThreadSparo->buffer = buffer;
    argsThreadSparo->direzione = direzione;
    argsThreadSparo->fdScrittura = fdScrittura;
    argsThreadSparo->mittente = mittente;
    argsThreadSparo->posPartenza = posPartenza;
    pthread_t tid;

    if(pthread_create(&tid, NULL, thread_sparo, argsThreadSparo) != 0) {endwin(); perror("fallito thread sparo"); _exit(2);}
    nuovaGranata = assegnaDatiGranata(tid, posPartenza);
    nuovoNodo = creaNodoGranata(nuovaGranata);
    pushGranata(listaGranate, nuovoNodo);
}

void creaProcessoProiettile(Mittente mittente, Posizione posPartenza, int direzione, TuttoBuffer* buffer) {
    Granata nuovaGranata;
    NodoGranata* nuovoNodo = NULL;
    

    ArgsThreadSparo* argsThreadSparo = malloc(sizeof(ArgsThreadSparo));
    if (argsThreadSparo == NULL) {endwin(); perror("Errore allocazione ArgsThreadSparo"); return ;}
    argsThreadSparo->buffer = buffer;
    argsThreadSparo->direzione = direzione;
    argsThreadSparo->mittente = mittente;
    argsThreadSparo->posPartenza = posPartenza;
    pthread_t tid;
    if(pthread_create(&tid, NULL, thread_sparo, argsThreadSparo) != 0) {endwin(); perror("fallito thread sparo"); _exit(2);}
    // processo padre continua l'esecuzione
}

void inizializzaManche(int nTane, int nFlussi, Tana tane[nTane], Flusso flussi[nFlussi], ListaCoccodrillo* listaCocco[nFlussi], ListaGranata** listaGranate, int fd[2], TuttoBuffer* buffer) {
    disegnaTane(buffer, nTane, tane);
    inizializzaArrayFlussi(nFlussi, flussi);
    inizializzaListaCoccodrilli(nFlussi, listaCocco);
    *listaGranate = creaListaVuotaGranata();
    creaCoccodrilliIniziali(2, fd, nFlussi, flussi, listaCocco, buffer);
    
    sem_wait(buffer->semLiberi);
    pthread_mutex_lock(buffer->mutex);
    refresh();
    pthread_mutex_unlock(buffer->mutex);
    sem_post(buffer->semLiberi);
}