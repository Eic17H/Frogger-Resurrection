#include "thread.h"
#include <ncurses.h>

bool bufferVuoto(TuttoBuffer* buffer) {
    pthread_mutex_lock(&buffer->mutex);
    bool ret = (buffer->head) == (buffer->tail);
    pthread_mutex_unlock(&buffer->mutex);
    return ret;
    }
bool bufferPieno(TuttoBuffer* buffer) {
    pthread_mutex_lock(&buffer->mutex);
    bool ret = ((buffer->head) + 1) % BUFFER_SIZE == (buffer->tail);
    pthread_mutex_unlock(&buffer->mutex);
    return ret;
    }

// TODO: fa segmentation fault e non capisco perché
void invia(TuttoBuffer* buffer, Messaggio msg) {
    while (bufferPieno(buffer));
    pthread_mutex_lock(&buffer->mutex);
    (buffer->buffer)[(buffer->head)] = msg;
    (buffer->head) = ((buffer->head) + 1) % BUFFER_SIZE;
    pthread_mutex_unlock(&buffer->mutex);
}

Messaggio ricevi(TuttoBuffer* buffer) {
    Messaggio msg;
    msg.mittente = -1;
    if (bufferVuoto(buffer)) return msg;
    pthread_mutex_lock(&buffer->mutex);
    msg = (buffer->buffer)[(buffer->tail)];
    buffer->tail = (buffer->tail + 1)  % BUFFER_SIZE;
    pthread_mutex_unlock(&buffer->mutex);
    return msg;
}