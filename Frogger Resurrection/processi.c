#include "processi.h"

bool forkFallita(pid_t pid){
    return pid<0;
}

bool processoFiglio(pid_t pid){
    return pid==0;
}

bool processoPadre(pid_t pid){
    return pid>0;
}