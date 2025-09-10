#ifndef ALTRECOSE_H
#define ALTRECOSE_H

#include "listaCoccodrillo.h"
#include "listaGranate.h"
#include "struttureDati.h"

bool laRanaESuUnCoccodrilloPuntoInterrogativo(Posizione rana, Posizione coccodrillo, int difficolta);
bool laRanaESuTanaPuntoInterrogativo(Posizione rana, Tana tana, int difficolta);
bool laRanaConquistatoTanaChiusa(Posizione rana, Tana tane[], int difficolta, bool* vivo);
bool ranaSuTanaSbagliata(Posizione rana, bool );
NodoCoccodrillo* trovaCoccodrilloSottoRana(Posizione posAttualeRana, NodoCoccodrillo* coccodrilloAttuale, ListaCoccodrillo** lista, int indiceFlusso);
int trovaPosRanaSuCoccodrillo(int xCoccodrillo, int xRana);
void gestisciCollisioneConGranate(Messaggio messaggioProiettile, ListaGranata* listaGranate);
bool laRanaESuUnCoccodrilloDiverso(NodoCoccodrillo* precedente, NodoCoccodrillo* attuale);
bool laRanaSpostataSuStessoCoccodrillo(NodoCoccodrillo* precedente, NodoCoccodrillo* attuale, Posizione posVecchiaRana, Posizione posNuovaRana);
Posizione sommaPosizioni(Posizione pos1, Posizione pos2);
bool posizioniUguali(Posizione pos1, Posizione pos2);
int trovaIndiceFlusso(int n, Flusso flussi[n], int yDesiderata);
int daSecondiAMinutiSecondi(int tempoInSecondi);
#endif /* ALTRECOSE_H */
