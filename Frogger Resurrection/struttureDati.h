#ifndef STRUTTUREDATI_H
#define STRUTTUREDATI_H

#include <unistd.h>

typedef enum{ BASE, MARCIAPIEDE, ACQUA, SPONDA, COCCODRILLO, RANA_SU_COCCODRILLO, NERO } colori; // Probabilmente da rifare in vista delle skin
typedef enum { RANA, GRANATA } Mittente;

typedef struct {
    int x;
    int y;
} Posizione;

typedef struct {
    Posizione posVecchia;
    Posizione posAttuale;
    pid_t pid;
    Mittente mittente;
} Messaggio;

typedef struct {
    pid_t pid;
} Coccodrillo;

#endif /* STRUTTUREDATI_H */