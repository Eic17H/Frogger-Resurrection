#include "thread.h"
#include "stdlib.h"
#include <ncurses.h>
#include <pthread.h>
#include <semaphore.h>
#include "costanti.h"

void inizializzaTuttoBuffer(TuttoBuffer* buffer) {
    buffer->buffer = (Messaggio*)malloc(sizeof(Messaggio) * BUFFER_SIZE);
    if (buffer->buffer == NULL) {endwin(); perror("errore creazione buffer"); return ;}

    sem_init(&buffer->semLiberi, 0, BUFFER_SIZE);
    sem_init(&buffer->semOccupati, 0, 0);

    pthread_mutex_init(&buffer->mutex, NULL);

    buffer->iLettura = 0;
    buffer->iScrittura = 0;
}

void ripulisciTuttoBuffer(TuttoBuffer* buffer) {
    // distruzione vecchi semafori
    sem_destroy(&buffer->semLiberi);
    sem_destroy(&buffer->semOccupati);

    pthread_mutex_destroy(&buffer->mutex);
    
    free(buffer->buffer);
    buffer->buffer = NULL;
}

void invia(TuttoBuffer* buffer, Messaggio msg) {
    sem_wait(&buffer->semLiberi);
    pthread_mutex_lock(&buffer->mutex);
    buffer->buffer[buffer->iScrittura] = msg;
    buffer->iScrittura = (buffer->iScrittura + 1) % BUFFER_SIZE;
    pthread_mutex_unlock(&buffer->mutex);
    sem_post(&buffer->semOccupati);
}

Messaggio ricevi(TuttoBuffer* buffer) {
    Messaggio msg;
    msg.mittente = -1;

    sem_wait(&buffer->semOccupati);
    msg = buffer->buffer[buffer->iLettura];
    sem_post(&buffer->semLiberi);

    // non c'è bisogno di mutex perché solo il main thread può leggere
    buffer->iLettura = (buffer->iLettura + 1)  % BUFFER_SIZE;

    return msg;
}