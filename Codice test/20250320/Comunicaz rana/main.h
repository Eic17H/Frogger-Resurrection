#ifndef MAIN_H
#define MAIN_H

#include <unistd.h>

typedef enum{ BASE, MARCIAPIEDE, ACQUA, SPONDA, COCCODRILLO, RANA_SU_COCCODRILLO, NERO } colori;
typedef enum { RANA, GRANATA } mittente;

struct posizione {
        int x;
        int y;
}; typedef struct posizione Posizione;

struct messaggio {
        Posizione pos;
        pid_t pid;
        mittente mittente;
}; typedef struct messaggio Messaggio;

#define NON_SU_SCHERMO -1
#define DIM_LINES 24
#define DIM_COLS 80

#define SPRITE_RANA "<00>"
#define SPRITE_GRANATA '*'

#define COCCODRILLO_SU "______^^"
#define COCCODRILLO_GIU "uu__uu_<"
#define COCCODRILLO_NO "        "

#define SALTO_RANA 2
#define W_RANA 4
#define W_COCCODRILLO 8

#define X_PARTENZA_RANA (COLS/2 - W_RANA/2)
#define Y_PARTENZA_RANA (LINES - 1)

#define DURATA_MANCHE_S 30

#define KEY_BARRA_SPAZIATRICE ' '

int altezzaSponda();
int altezzaMarciapiede();

void adattaFinestra();

void inizializzaColori();

void coloraAmbienteGioco();

void inizializzaColoreSprite(int ySprite);

#endif