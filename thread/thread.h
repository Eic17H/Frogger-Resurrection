#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include "struttureDati.h"

#define BUFFER_SIZE 255

typedef struct {
    Messaggio* buffer;
    int head;
    int tail;
    pthread_mutex_t mutex;
} TuttoBuffer;

bool bufferVuoto(TuttoBuffer* buffer);
bool bufferPieno(TuttoBuffer* buffer);
void invia(TuttoBuffer* buffer, Messaggio msg);
Messaggio ricevi(TuttoBuffer* buffer);

#endif // THREAD_H