#ifndef RANA_H
#define RANA_H

#include <unistd.h>
#include "struttureDati.h"

void rana(int fdScrittura);
void creaProcessoGranata(int fdScrittura, Posizione posPartenza, int direzione);

#endif