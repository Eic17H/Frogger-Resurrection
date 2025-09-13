#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
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

void creaPipe(int fd[]) {
    if (pipe(fd) == -1) {
        endwin();
        perror("errore creazione pipe");
        exit(1);
    }
}

void chiudiPipe(int fd[]) {
    close(fd[0]);
    close(fd[1]);
}