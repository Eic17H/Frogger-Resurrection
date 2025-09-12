#ifndef REGOLE_H
#define REGOLE_H

#include "costanti.h"
#include "struttureDati.h"
#include <stdbool.h>
#include <time.h>

#define NELL_AREA_DI_GIOCO(x) (x.y < DIM_LINES - H_MARCIAPIEDE && x.y > H_SPONDA)

bool tempoScaduto(time_t ora, time_t start);
bool fuoriSchermo(Posizione pos, Mittente mittente, int verso);
bool ancoraViva(bool inAcqua, bool colpito, bool tanaSbagliata);
#endif /* REGOLE_H */
