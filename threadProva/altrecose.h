#ifndef ALTRECOSE_H
#define ALTRECOSE_H

#include "listaCoccodrillo.h"
#include "listaGranate.h"
#include "struttureDati.h"
#include "listaThreadDaTerminare.h"

#define RAND_TRA(x,y) x + rand() % (y - x + 1)

bool laRanaESuUnCoccodrilloPuntoInterrogativo(Posizione rana, Posizione coccodrillo, int difficolta);
bool laRanaESuTanaPuntoInterrogativo(Posizione rana, Tana tana, int difficolta);
bool laRanaConquistatoTanaChiusa(Posizione rana, Tana tane[], int difficolta, bool* tanaSbagliata);
bool ranaSuTanaSbagliata(Posizione rana, bool );
NodoCoccodrillo* trovaCoccodrilloSottoRana(Posizione posAttualeRana, NodoCoccodrillo* coccodrilloAttuale, ListaCoccodrillo** lista, int indiceFlusso);
int trovaPosRanaSuCoccodrillo(int xCoccodrillo, int xRana);
bool gestisciCollisioneConGranate(Messaggio messaggioProiettile, ListaGranata* listaGranate, ListaThreadTerminabili listaThreadTerminabili);
void gestisciCollisioneConRana(Messaggio messaggioProiettile, Posizione posRana, bool* colpito);
bool laRanaESuUnCoccodrilloDiverso(NodoCoccodrillo* precedente, NodoCoccodrillo* attuale);
bool laRanaSpostataSuStessoCoccodrillo(NodoCoccodrillo* precedente, NodoCoccodrillo* attuale, Posizione posVecchiaRana, Posizione posNuovaRana);
Posizione sommaPosizioni(Posizione pos1, Posizione pos2);
bool posizioniUguali(Posizione pos1, Posizione pos2);
int trovaIndiceFlusso(int n, Flusso flussi[n], int yDesiderata);
int daSecondiAMinutiSecondi(int tempoInSecondi);
bool ricominciaPartita();
#endif /* ALTRECOSE_H */
