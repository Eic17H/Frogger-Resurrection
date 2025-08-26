#ifndef ALTRECOSE_H
#define ALTRECOSE_H

#include "struttureDati.h"

bool laRanaESuUnCoccodrilloPuntoInterrogativo(Posizione rana, Posizione coccodrillo, int difficolta);
Posizione sommaPosizioni(Posizione pos1, Posizione pos2);
int trovaIndiceFlusso(int n, Flusso flussi[n], int yDesiderata);
int daSecondiAMinutiSecondi(int tempoInSecondi);

#endif /* ALTRECOSE_H */
