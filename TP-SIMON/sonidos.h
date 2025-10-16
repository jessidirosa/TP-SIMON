#ifndef SONIDOS_H_INCLUDED
#define SONIDOS_H_INCLUDED

#define FREC_MUESTREO 44100
#define AMPLITUD_TONO 32766
#define DURACION_SEG 1

#include <math.h>
#include "sdl.h"

Mix_Chunk* cargarSonido(const char *ruta);
Mix_Chunk* crearTono(float frecuencia);
void destruirTono(Mix_Chunk *tono);
void asignarSonidos(tJuego* juego);
void reproducirNota(int boton, tJuego* juego);
void reproducirError(tJuego* juego);

Mix_Chunk* crearTonoSeno(float frecuencia);
Mix_Chunk* crearTonoCuadrada(float frecuencia);
Mix_Chunk* crearTonoSierra(float frecuencia);
Mix_Chunk* crearTonoTriang(float frecuencia);
Mix_Chunk* crearTonoPorTimbre(float frecuencia, eTimbre timbre);


#endif // SONIDOS_H_INCLUDED
