#ifndef COSTANTI_H
#define COSTANTI_H

// coordinate
#define NON_SU_SCHERMO -1
#define DIM_LINES 38
#define DIM_COLS 143

#define Y_PARTENZA_RANA (DIM_LINES - 2)
#define X_PARTENZA_RANA (DIM_COLS/2 - W_RANA/2)

#define Y_TIMER_MANCHE 0
#define X_TIMER_MANCHE 16

#define Y_STAMPA_VITE 0
#define X_PARTENZA_STAMPA_VITE 4

#define Y_STAMPA_ROUND_RIMASTI 0
#define X_STAMPA_ROUND_RIMASTI (DIM_COLS - 20)

#define CODICE_GRANATA_SPARATA 255

// dimensioni
#define SALTO_RANA 2
#define W_RANA 3
#define W_COCCODRILLO 8
#define W_TANA 9
#define H_TANA 4

#define H_SPONDA 10
#define H_MARCIAPIEDE 7

#define N_FLUSSI 10
#define DISTANZA_FLUSSI 1
#define N_TANE 5 
#define N_SEPARATORI_TANE 6
#define N_VITE 4
#define N_MANCHE 8

#define DURATA_MANCHE_S 40
#define TEMPO_RICARICA_GRANATA 0

#define KEY_BARRA_SPAZIATRICE ' '

// coccodrilli
#define MIN_VELOCITA_COCCO 1
#define MAX_VELOCITA_COCCO 2
#define MIN_SPAZIO_FRA_COCCO (W_COCCODRILLO + 1)
#define MAX_SPAZIO_FRA_COCCO (W_COCCODRILLO + SALTO_RANA - 1)

#define AVANZAMENTO_DX 1
#define AVANZAMENTO_SX -1

#define ATTESA_MIN_PROIETTILE_S 6
#define ATTESA_MAX_PROIETTILE_S 30

#define PREAVVISO_SPARO_S 1

#endif /* COSTANTI_H */
