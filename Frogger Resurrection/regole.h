#ifndef REGOLE_H
#define REGOLE_H

#include "costanti.h"
#include "struttureDati.h"
#include <stdbool.h>
#include <time.h>

bool tempoScaduto(time_t ora, time_t start);
bool cadutoInAcqua(Posizione pos);

#endif /* REGOLE_H */
