#ifndef ALTRECOSE_H
#define ALTRECOSE_H

#include "listaCoccodrillo.h"
#include "struttureDati.h"

bool laRanaESuUnCoccodrilloPuntoInterrogativo(Posizione rana, Posizione coccodrillo, int difficolta);
NodoCoccodrillo* trovaCoccodrilloSottoRana(Posizione posAttualeRana, NodoCoccodrillo* coccodrilloAttuale, ListaCoccodrillo** lista, int indiceFlusso);
int trovaPosRanaSuCoccodrillo(int xCoccodrillo, int xRana);
bool laRanaESuUnCoccodrilloDiverso(NodoCoccodrillo* precedente, NodoCoccodrillo* attuale);
bool laRanaSpostataSuStessoCoccodrillo(NodoCoccodrillo* precedente, NodoCoccodrillo* attuale, Posizione posVecchiaRana, Posizione posNuovaRana);
Posizione sommaPosizioni(Posizione pos1, Posizione pos2);
bool posizioniUguali(Posizione pos1, Posizione pos2);
int trovaIndiceFlusso(int n, Flusso flussi[n], int yDesiderata);
int daSecondiAMinutiSecondi(int tempoInSecondi);

#endif /* ALTRECOSE_H */
