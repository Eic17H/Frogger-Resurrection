#ifndef PROCESSI_H
#define PROCESSI_H

#include <stdbool.h>
#include <unistd.h>

bool forkFallita(pid_t pid);
bool processoFiglio(pid_t pid);
bool processoPadre(pid_t pid);

#endif /* PROCESSI_H */
