#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include "struttureDati.h"

void coccodrillo(int fdScrittura, Flusso flussoAttuale) {
    Posizione pos; 
    Messaggio messaggio;
    time_t start = 0, ora = 0;
    _Bool sparato = false;
    pos = flussoAttuale.posIniziale;

    // scrittura primo messaggio
    messaggio.tipo = POSIZIONE;
    messaggio.mittente = COCCO;
    messaggio.pid = getpid();
    if (messaggio.pid < 0) {perror("Errore getpid()"); _exit(2);}
    messaggio.posAttuale = pos;

    messaggio.posVecchia = pos;

    write(fdScrittura, &messaggio, sizeof(Messaggio));

    while (1) {
        // TODO: ok ho provato a fargli sparare una granata ad ogni passo e funziona, aggiungere i proiettili sarà facile
        messaggio.posVecchia = pos;

        pos.x += flussoAttuale.velocità * flussoAttuale.verso;
        // aggiornamento coordinate
        messaggio.posAttuale = pos;
        write(fdScrittura, &messaggio, sizeof(Messaggio));

        usleep(400000);
    }
}
