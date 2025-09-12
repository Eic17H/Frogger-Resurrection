#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <semaphore.h>
#include "struttureDati.h"

typedef struct {
    Messaggio* buffer;
    sem_t* semLiberi;
    sem_t* semOccupati;
    int* iScrittura;
    pthread_mutex_t* mutex;
} TuttoBuffer;

typedef struct 
{
    Flusso flussoAttuale;
    TuttoBuffer* buffer;
}ArgsThreadCoccodrillo;

void inizializzaBufferSemaforiMutex(Messaggio** buffer, sem_t* semLiberi, sem_t* semOccupati, pthread_mutex_t* mutex);

bool bufferVuoto(TuttoBuffer* buffer);
bool bufferPieno(TuttoBuffer* buffer);
void invia(TuttoBuffer* buffer, Messaggio msg);
Messaggio ricevi(TuttoBuffer* buffer);

#endif // THREAD_H