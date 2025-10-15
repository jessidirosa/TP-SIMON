#ifndef MENUS_H_INCLUDED
#define MENUS_H_INCLUDED

#include "sdl.h"

#define SCHONBERG 1
#define MOZART 2
#define DESAFIO 3

#define QUIT_BOTON_X 100
#define QUIT_BOTON_Y 100

#define MIN_DURACION 2000

#define TOP 5
#define TIEMPO_PRO 1000
#define TIEMPO_MEDIO 1500
#define TIEMPO_FACIL 2000

void menuInicial(tJuego* juego, tPartida* partida);
void menuConfig(tJuego* juego, tPartida* partida);
void menuModos(tJuego* juego, tPartida* partida);
void menuCantBotones(tJuego* juego, tPartida* partida);
void menuDuracionInicial(tJuego* juego, tPartida* partida);
void pedirNombreJugador(tJuego* juego, TTF_Font* fuente, char* destino, int maxLen);
bool mostrarRanking(tJuego* juego, tPartida* partida, char* nombreArch, tRanking* vecRanking, int* ce, int* max);
void menuDuracionInicial(tJuego* juego, tPartida* partida);
void menuCantBotonesDesafio(tJuego* juego, tPartida* partida);

#endif // MENUS_H_INCLUDED
