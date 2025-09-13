#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <semaphore.h>
#include "struttureDati.h"

typedef struct {
    Messaggio* buffer;
    sem_t semLiberi;
    sem_t semOccupati;
    int iScrittura;
    int iLettura;
    pthread_mutex_t mutex;
} TuttoBuffer;

typedef struct 
{
    Flusso flussoAttuale;
    TuttoBuffer* buffer;
}ArgsThreadCoccodrillo;

typedef struct 
{
    Mittente mittente;
    Posizione posPartenza;
    int direzione;
    TuttoBuffer* buffer;
}ArgsThreadSparo;

void inizializzaTuttoBuffer(TuttoBuffer* buffer);
void ripulisciTuttoBuffer(TuttoBuffer* buffer);

void invia(TuttoBuffer* buffer, Messaggio msg);
Messaggio ricevi(TuttoBuffer* buffer);

#endif // THREAD_H