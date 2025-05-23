#ifndef MAIN_H
#define MAIN_H

#include <unistd.h>

typedef enum{ BASE, MARCIAPIEDE, ACQUA, SPONDA, COCCODRILLO, RANA_SU_COCCODRILLO, NERO } colori;
typedef enum { RANA, GRANATA, COCCOD } mittente;

struct posizione {
        int x;
        int y;
}; typedef struct posizione Posizione;

struct messaggio {
        Posizione posVecchia;
        Posizione posAttuale;
        pid_t pid;
        mittente mittente;
}; typedef struct messaggio Messaggio;

struct flusso {
        int verso;
        int velocità;
        int distanzaCoccodrilli;
        Posizione posIniziale;
}; typedef struct flusso Flusso;

struct tana {
        bool chiusa;
        int xInizio;
        int xFine;
        int y;
}; typedef struct tana Tana;

#define N_TANE 5
#define N_SEPARATORI_TANE 6
#define SPRITE_TANA_APERTA1 "###***###"
#define SPRITE_TANA_APERTA2 "##*   *##"
#define SPRITE_TANA_APERTA3 "#*     *#"
#define SPRITE_TANA_APERTA4 "#*.....*#"

#define SPRITE_TANA_CHIUSA1 "###@@@###"
#define SPRITE_TANA_CHIUSA2 "##@I@I@##"
#define SPRITE_TANA_CHIUSA3 "#@I@I@I@#"
#define SPRITE_TANA_CHIUSA4 "#@*****@#"

#define NON_IN_LISTA -1
#define NON_SU_SCHERMO -1
#define DIM_LINES 38
#define DIM_COLS 141

#define SPRITE_RANA "<00>"
#define SPRITE_GRANATA '*'

#define N_FLUSSI 8
#define DISTANZA_FLUSSI 2

#define MIN_VELOCITA_COCCO 1
#define MAX_VELOCITA_COCCO 4
#define MIN_SPAZIO_FRA_COCCO 10
#define MAX_SPAZIO_FRA_COCCO 20

#define COCCODRILLO_DX_SU "______^^"
#define COCCODRILLO_DX_GIU "uu__uu_<"

#define COCCODRILLO_SX_SU "^^______"
#define COCCODRILLO_SX_GIU ">_uu__uu"
#define COCCODRILLO_NO "        "

#define SPRITE_TANA_APERTA1 "###***###"
#define SPRITE_TANA_APERTA2 "##*   *##"
#define SPRITE_TANA_APERTA3 "#*     *#"
#define SPRITE_TANA_APERTA4 "#*.....*#"

#define SALTO_RANA 2
#define W_RANA 4
#define W_COCCODRILLO 8
#define W_TANA 9
#define H_TANA 4

#define X_PARTENZA_RANA (COLS/2 - W_RANA/2)
#define Y_PARTENZA_RANA (LINES - 1)

#define DURATA_MANCHE_S 30
#define TEMPO_RICARICA_GRANATA 2

#define KEY_BARRA_SPAZIATRICE ' '

#define AVANZAMENTO_DX 1
#define AVANZAMENTO_SX -1

int altezzaSponda();
int altezzaMarciapiede();

void adattaFinestra();

void inizializzaColori();

void coloraAmbienteGioco();

void inizializzaColoreSprite(int ySprite);

void creaTane(int nTane, Tana tane[nTane]);

void disegnaTane(int nTane, Tana tane[nTane]);

void inizializzaArrayFlussi(int nFlussi, Flusso flussi[nFlussi]);

void creaCoccodrilliIniziali(int n, int fd[n], int nFlussi, Flusso flussi[nFlussi], pid_t coccodrilliCreatiPerPrimi[nFlussi]);

bool coccodrilloFuoriSchermo(int xAttuale, int xVecchia);

int trovaPosCoccodrilloListaPrimi(pid_t pidCoccodrillo, int nFlussi, pid_t coccodrilliCreatiPerPrimi[nFlussi]);

bool creareNuovoCoccodrillo(Flusso flussoCoccodrillo, Messaggio messaggioCoccodrillo);

void creaNuovoPrimoCoccodrillo(int nFlussi, pid_t coccodrilliCreatiPerPrimi[nFlussi], Flusso FlussoCoccodrillo, int indiceFlussoCoccodrillo, int n, int fd[n]);

bool coccodrilloStaComparendo(Flusso flussoCoccodrillo, int xAttuale);

void cancellaVecchioCoccodrillo(Posizione posVecchia);

void disegnaCoccodrillo(Posizione posAttuale, int versoCoccodrillo);

#endif