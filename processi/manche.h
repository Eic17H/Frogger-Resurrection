#ifndef MANCHE_H
#define MANCHE_H

#include <stdbool.h>
#include <time.h>
#include <sys/wait.h>
#include "listaGranate.h"
#include "struttureDati.h"
#include "costanti.h"
#include "listaCoccodrillo.h"
#include "regole.h"
#include "visualizzazione.h"
#include "inizializzazione.h"

bool aggiornaPosizioneRana(Posizione *posMain, Posizione posInviata, Flusso flussi[N_FLUSSI], ListaCoccodrillo* lista[N_FLUSSI]);
int manche(int fd[2], Flusso flussi[N_FLUSSI], ListaCoccodrillo* listaCoccodrilli[N_FLUSSI], ListaGranata** listaGranate, pid_t pidRana, Tana tane[N_TANE], int difficolta, bool* tanaOccupata);
void messaggioAltroRound(bool inAcqua, bool colpito, bool tanaSbagliata, bool tanaOccupata);
void stampaPosPosPosPosPos(Posizione posDaStampare, int xInCuiStampare, int yInCuiStampare); // TODO: rimuovere

#endif /* MANCHE_H */