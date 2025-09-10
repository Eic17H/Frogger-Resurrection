#ifndef STRUTTUREDATI_H
#define STRUTTUREDATI_H

#include <unistd.h>
#include <stdbool.h>

typedef enum{ BASE, MARCIAPIEDE, ACQUA, SPONDA, COCCODRILLO_VERDE, COCCODRILLO_GIALLO, RANA_SU_COCCODRILLO, NERO } colori; // Probabilmente da rifare in vista delle skin
typedef enum { RANA, GRANATA, COCCO } Mittente;
typedef enum { POSIZIONE, PUNTEGGIO } TipoMessaggio;

typedef struct {
    int x;
    int y;
} Posizione;

typedef struct {
    TipoMessaggio tipo;
    pid_t pid;
    Mittente mittente;
    Posizione posVecchia;
    Posizione posAttuale;
    int punteggio;
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