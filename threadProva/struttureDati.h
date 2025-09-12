#ifndef STRUTTUREDATI_H
#define STRUTTUREDATI_H

#include <bits/pthreadtypes.h>
#include <unistd.h>
#include <stdbool.h>

typedef enum{ BASE, MARCIAPIEDE, ACQUA, SPONDA, COCCODRILLO_VERDE, COCCODRILLO_GIALLO, COCCODRILLO_ROSSO, RANA_SU_COCCODRILLO, NERO } colori; // Probabilmente da rifare in vista delle skin
typedef enum { RANA, GRANATA, PROIETTILE, COCCO } Mittente;

typedef struct {
    int x;
    int y;
} Posizione;

typedef struct {
    Posizione posVecchia;
    Posizione posAttuale;
    pthread_t id;
    Mittente mittente;
    bool staPerSparare;
} Messaggio;

struct flusso {
    int verso;
    int velocità;
    int distanzaCoccodrilli;
    Posizione posIniziale;
}; typedef struct flusso Flusso;

struct tana {
    bool chiusa;
    int xInizio;
    int xFine;
    int y;
}; typedef struct tana Tana;

#endif /* STRUTTUREDATI_H */