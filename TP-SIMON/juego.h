#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX 1024
#define MAX_NOMBRE 20
#define GAMEOVER 0
#define ERROR -1
#define CONTINUA 1
#define TIEMPO_INICIAL 2000
#define TIEMPO_ACOTADO_POR_NOTA 0.03

typedef struct
{
    char jugador[MAX_NOMBRE];
    int score;
} tRanking;

typedef struct
{
    int sec[MAX];
    int* psec;
    int res[MAX];
    tRanking* ranking;
    bool estado;
    float tiempoNota;
    float acoteDuracion;

} tPartida;

void inicializarPartida(tPartida* juego);
void iniciarJuego(tPartida* juego);
bool insertarArchivoRankingSinDup(tPartida* juego);


#endif // JUEGO_H_INCLUDED
