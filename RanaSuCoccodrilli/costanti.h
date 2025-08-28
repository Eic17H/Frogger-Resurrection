#ifndef COSTANTI_H
#define COSTANTI_H

// coordinate
#define NON_SU_SCHERMO -1
#define DIM_LINES 38
#define DIM_COLS 143

#define X_PARTENZA_RANA (DIM_COLS/2 - W_RANA/2)
#define Y_PARTENZA_RANA (DIM_LINES - 1)

// dimensioni
#define SALTO_RANA 2
#define W_RANA 4
#define W_COCCODRILLO 8
#define W_TANA 9
#define H_TANA 4

#define H_SPONDA 6
#define H_MARCIAPIEDE 6

#define N_FLUSSI 11
#define DISTANZA_FLUSSI 1
#define N_TANE 5 
#define N_SEPARATORI_TANE 6

#define DURATA_MANCHE_S 300
#define TEMPO_RICARICA_GRANATA 2

#define KEY_BARRA_SPAZIATRICE ' '

// coccodrilli
#define MIN_VELOCITA_COCCO 2
#define MAX_VELOCITA_COCCO 5
#define MIN_SPAZIO_FRA_COCCO (W_COCCODRILLO )
#define MAX_SPAZIO_FRA_COCCO (W_COCCODRILLO + 1)

#define AVANZAMENTO_DX 1
#define AVANZAMENTO_SX -1

#endif /* COSTANTI_H */
