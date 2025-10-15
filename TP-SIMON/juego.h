#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "sdl.h"

#define POS_VIDAS 140

#define MAX 1024
#define MAX_RANK 100
#define MAX_NOMBRE 20
#define GAMEOVER 0
#define ERROR -1
#define CONTINUA 1
#define TIEMPO_INICIAL 1000
#define TIEMPO_ACOTADO_POR_NOTA 0.1
#define MAX_BOTON 8
#define CENTRO_PLAY_X 99
#define CENTRO_PLAY_Y 99
#define VACIO -1

#define CANT_VIDAS 3

typedef struct
{
    char jugador[MAX_NOMBRE];
    int score;
} tRanking;

typedef struct
{
    int* sec;
    int* psec;
    tRanking ranking;
    int estado;
    float tiempoNota;
    float acoteDuracion;
    int modoJuego;

} tPartida;

typedef struct
{
    int num; //0, 1, 2, 3, 4, 5, 6, 7
    double x;
    double y;
    double distClick;
    int sonido;
} tBoton;

void inicializarPartida(tPartida* partida, tJuego* juego);
void iniciarJuego(tPartida* partida, tJuego* juego, int** mat);
bool insertarArchivoRankingSinDup(tPartida* juego);
int botonSeleccionar(tJuego* juego);
bool esMenor(double actual, double nuevo);
void crearSecuenciaAleatoria(tPartida* partida, tJuego* juego);
void secuenciaJuego(tPartida* partida, tJuego* juego, int** mat);
int respuesta(tPartida* partida, tJuego* juego, int** mat, tBotones* pausa);
bool puntoDentroCirculo(int x, int y, int cx, int cy, int r);
void liberarMemoria(tJuego* juego, int** mat, tPartida* partida, tRanking* ranking);
float duracionNota(tPartida* partida);
bool puntoEnRectangulo(int x, int y, int xRect, int yRect, int anchoRect, int altoRect);
void configPorDefecto(tJuego* juego, tPartida* partida);
bool mostrarArchivo(char* nombre);
void* crearVector(size_t tam, int ce);
bool redimensionar(void** v, int ce, size_t tam, int* maxTam);
int ordenarArchivo(char* nombre, tRanking* vRank, int* ce, int* maxTam);
int compararRankings(const void* r1, const void* r2);

#endif // JUEGO_H_INCLUDED
