#include "thread.h"
#include "stdlib.h"
#include <ncurses.h>
#include <semaphore.h>
#include "costanti.h"

void inizializzaBufferSemaforiMutex(Messaggio** buffer, sem_t* semLiberi, sem_t* semOccupati, pthread_mutex_t* mutex) {
    *buffer = (Messaggio*)malloc(sizeof(Messaggio) * BUFFER_SIZE);
    if (*buffer == NULL) {endwin(); perror("errore creazione buffer"); return ;}

    sem_init(semLiberi, 0, BUFFER_SIZE);
    sem_init(semOccupati, 0, 0);

    pthread_mutex_init(mutex, NULL);
}

//bool bufferVuoto(TuttoBuffer* buffer) {
//    pthread_mutex_lock(buffer->mutex);
//    bool ret = *buffer->head == *buffer->tail;
//    pthread_mutex_unlock(buffer->mutex);
//    return ret;
//    }
//bool bufferPieno(TuttoBuffer* buffer) {
//    pthread_mutex_lock(buffer->mutex);
//    bool ret = (*buffer->head + 1) % BUFFER_SIZE == *buffer->tail;
//    pthread_mutex_unlock(buffer->mutex);
//    return ret;
//    }
//

void invia(TuttoBuffer* buffer, Messaggio msg) {
    sem_wait(buffer->semLiberi);
    pthread_mutex_lock(buffer->mutex);
    buffer->buffer[*buffer->iScrittura] = msg;
    *buffer->iScrittura = (*buffer->iScrittura + 1) % BUFFER_SIZE;
    pthread_mutex_unlock(buffer->mutex);
    sem_post(buffer->semOccupati);
}

Messaggio ricevi(TuttoBuffer* buffer) {
    Messaggio msg;
    msg.mittente = -1;
    static int iLettura = 0;

    sem_wait(buffer->semOccupati);
    msg = buffer->buffer[iLettura];
    sem_post(buffer->semLiberi);

    iLettura = (iLettura + 1)  % BUFFER_SIZE;

    return msg;
}