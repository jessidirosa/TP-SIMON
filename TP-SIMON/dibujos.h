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
#define N 10

#define MAT_COL 200
#define MAT_FILA 200
#define R_INT 15
#define R_EXT 80
#define TAM_PIXEL 1

void dibujar(tJuego* juego, int** mat);
int** crearMatriz(int cf, int cc, size_t tam);
void dibujarBordes(tJuego* juego);
void iluminarBoton(int boton, tJuego* juego, int** mat);
void apagarBoton(int boton, tJuego* juego, int** mat);
void dibujarMenu(tJuego* juego, tBotones* jugar, tBotones* config, tBotones* salir, tBotones* ranking);
void dibujarBotonCentro(tJuego* juego, tBotones* boton, char* nombre);
void dibujarMenuConfig(tJuego* juego, tBotones* modos, tBotones* duracionInicial, tBotones* cantBotones, tBotones* atras);
void dibujarBotones(tBotones* boton, char* nombre, tJuego* juego, int x, int y, char* fuente, int tam, SDL_Color color);
void dibujarMenuModos(tJuego* juego, tBotones* modoSchonberg, tBotones* modoMozart, tBotones* modoDesafio, tBotones* atras);
void dibujarMenuBotones(tJuego* juego, tBotones* b3, tBotones* b4, tBotones* b5, tBotones* b6, tBotones* b7, tBotones* b8, tBotones* atras);
void dibujarTextos(char* nombre, tJuego* juego, int x, int y, char* fuente, int tam, SDL_Color color);


//void dibujarMenuPausa(tJuego* juego, tBotones* reanudar, tBotones* volverAInicio);

#endif // DIBUJOS_H_INCLUDED
