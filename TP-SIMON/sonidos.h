#ifndef SONIDOS_H_INCLUDED
#define SONIDOS_H_INCLUDED

#define FREC_MUESTREO 44100
#define AMPLITUD_TONO 32766

#include <math.h>

Mix_Chunk* cargarSonido(const char *ruta);
Mix_Chunk* crearTono(float frecuencia);
void destruirTono(Mix_Chunk *tono);

#endif // SONIDOS_H_INCLUDED
