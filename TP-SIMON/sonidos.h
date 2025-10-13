#ifndef SONIDOS_H_INCLUDED
#define SONIDOS_H_INCLUDED

#define FREC_MUESTREO 44100
#define AMPLITUD_TONO 32766

#include <math.h>
#include "sdl.h"

Mix_Chunk* cargarSonido(const char *ruta);
Mix_Chunk* crearTono(float frecuencia);
void destruirTono(Mix_Chunk *tono);
void asignarSonidos(tJuego* juego);
void reproducirNota(int boton, tJuego* juego);

#endif // SONIDOS_H_INCLUDED
