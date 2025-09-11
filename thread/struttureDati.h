#ifndef STRUTTUREDATI_H
#define STRUTTUREDATI_H

#include <unistd.h>
#include <stdbool.h>
#include <bits/pthreadtypes.h>
#include <semaphore.h>

#include "costanti.h"

typedef enum{ BASE, MARCIAPIEDE, ACQUA, SPONDA, COCCODRILLO_VERDE, COCCODRILLO_GIALLO, COCCODRILLO_ROSSO, RANA_SU_COCCODRILLO, NERO } colori; // Probabilmente da rifare in vista delle skin
typedef enum { RANA, GRANATA, PROIETTILE, COCCO } Mittente;

typedef struct {
    int x;
    int y;
} Posizione;

typedef struct {
    Posizione posVecchia;
    Posizione posAttuale;
    pid_t pid;
    Mittente mittente;
    bool staPerSparare;
} Messaggio;

struct paramRana {
    Messaggio* buff;
    sem_t* semLiberi;
    sem_t* semOccupati;
    pthread_mutex_t* mutex;
    int* indiceScrittura;
}; typedef struct paramRana ParamRana;

struct flusso {
    int verso;
    int velocità;
    int distanzaCoccodrilli;
    Posizione posIniziale;
}; typedef struct flusso Flusso;

struct tana {
    bool chiusa;
    int xInizio;
    int xFine;
    int y;
}; typedef struct tana Tana;

#endif /* STRUTTUREDATI_H */