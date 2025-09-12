#ifndef COCCODRILLO_H
#define COCCODRILLO_H

#include <unistd.h>
#include "struttureDati.h"
#include "thread.h"

void coccodrillo(int fdScrittura, Flusso flussoAttuale, TuttoBuffer* buffer);

#endif