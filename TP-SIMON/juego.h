#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX 1024
#define MAX_NOMBRE 20
#define GAMEOVER 0
#define ERROR -1
#define CONTINUA 1
#define TIEMPO_INICIAL 1000
#define TIEMPO_ACOTADO_POR_NOTA 0.03
#define MAX_BOTON 8
#define CENTRO_PLAY_X 99
#define CENTRO_PLAY_Y 99

typedef struct
{
    char jugador[MAX_NOMBRE];
    int score;
} tRanking;

typedef struct
{
    int* sec;
    int* psec;
    tRanking* ranking;
    bool estado;
    float tiempoNota;
    float acoteDuracion;

} tPartida;

typedef struct
{
    int num; //1, 2, 3, 4, 5, 6, 7, 8
    double x;
    double y;
    double distClick;
    bool encendido;
    int duracion_flash; //tiempo que dura encendido
    int sonido;
} tBoton;

void inicializarPartida(tPartida* juego);
void iniciarJuego(tPartida* partida, tJuego* juego, int** mat);
bool insertarArchivoRankingSinDup(tPartida* juego);
int botonSeleccionar(tJuego* juego);
bool esMenor(double actual, double nuevo);
void crearSecuenciaAleatoria(tPartida* partida);
void secuenciaJuego(tPartida* partida, tJuego* juego, int** mat);
int respuesta(tPartida* partida, tJuego* juego, int** mat);
bool puntoDentroCirculo(int x, int y, int cx, int cy, int r);
int finalizarJuego(tPartida* partida);
void liberarMemoria(tJuego* juego, int** mat, tPartida* partida);
float duracionNota(tPartida* partida);



#endif // JUEGO_H_INCLUDED
