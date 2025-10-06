#ifndef DIBUJOS_H_INCLUDED
#define DIBUJOS_H_INCLUDED

#include <math.h>
#include "sdl.h"

#define AM 0
#define V 1
#define R 2
#define AZ 3
#define VI 4
#define RO 5
#define NA 6
#define CE 7
#define BL 8
#define T 9

#define MAT_COL 200
#define MAT_FILA 200
#define R_INT 15
#define R_EXT 80
#define TAM_PIXEL 1

void dibujar(tJuego* juego, int** mat);
int** crearMatriz(int cf, int cc, size_t tam);
void dibujarBordes(tJuego* juego);
void iluminarBoton(int boton, tJuego* juego, int** mat);

#endif // DIBUJOS_H_INCLUDED
