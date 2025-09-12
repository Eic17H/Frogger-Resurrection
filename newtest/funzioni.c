#ifndef x
#define x

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define BUFFER_SIZE 255

typedef struct {
    int num;
    int mittente;
} Messaggio;

typedef struct {
    Messaggio* buffer;
    int* head;
    int* tail;
    pthread_mutex_t* mutex;
} TuttoBuffer;

bool bufferVuoto(TuttoBuffer buffer) { return *(buffer.head) == *(buffer.tail); }
bool bufferPieno(TuttoBuffer buffer) { return (*(buffer.head) + 1) % BUFFER_SIZE == *(buffer.tail); }

void invia(TuttoBuffer* buffer, Messaggio msg) {
    while (bufferPieno(*buffer));
    pthread_mutex_lock(buffer->mutex);
    (buffer->buffer)[(*buffer->head)] = msg;
    *(buffer->head) = (*(buffer->head) + 1) % BUFFER_SIZE;
    pthread_mutex_unlock(buffer->mutex);
}

Messaggio ricevi(TuttoBuffer* buffer) {
    Messaggio msg;
    msg.mittente = -1;
    if (bufferVuoto(*buffer))
        return msg;
    pthread_mutex_lock(buffer->mutex);
    msg = (buffer->buffer)[(*buffer->tail)];
    *buffer->tail = (*buffer->tail + 1)  % BUFFER_SIZE;
    pthread_mutex_unlock(buffer->mutex);
    return msg;
}



void* thread1(void* dati) {
    printf("thread1()\n");
    TuttoBuffer *buffer = (TuttoBuffer*) dati;
    Messaggio m;
    m.mittente = 0;
    m.num = 0;
    while(m.num <= 1000) {invia(buffer, m); m.num++;}
    return NULL;
}

void* thread2(void* dati) {
    printf("thread2()\n");
    TuttoBuffer *buffer = (TuttoBuffer*) dati;
    Messaggio m = {0, 1};
    while(1) {
        m = ricevi(buffer);
        if(m.mittente != -1){
            pthread_mutex_lock(buffer->mutex);
            printf("%d: %d\n", *buffer->tail, m.num);
            pthread_mutex_unlock(buffer->mutex);
        }
    }
    return NULL;
}

#endif