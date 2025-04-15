#ifndef COSTANTI_H
#define COSTANTI_H

#define NON_SU_SCHERMO -1
//#define DIM_LINES 24
//#define DIM_COLS 80
#define DIM_LINES 38
#define DIM_COLS 143

#define SPRITE_RANA "<00>"
#define SPRITE_GRANATA '*'

#define COCCODRILLO_SU "______^^"
#define COCCODRILLO_GIU "uu__uu_<"
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

#endif /* COSTANTI_H */
