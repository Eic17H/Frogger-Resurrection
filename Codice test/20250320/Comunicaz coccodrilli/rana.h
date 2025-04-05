#ifndef RANA_H
#define RANA_H

#include <unistd.h>
#include "main.h"

void rana(int fdScrittura);
void creaProcessoGranata(int fdScrittura, Posizione posPartenza, int direzione);

#endif