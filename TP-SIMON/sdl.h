#ifndef TP_SIMON_H_INCLUDED
#define TP_SIMON_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define ALTO_VENTANA 680
#define ANCHO_VENTANA 680
#define ERROR_INICIALIZACION -1;


typedef struct
{
    SDL_Window* ventana;
    SDL_Renderer* render;
    SDL_Surface* surface;
    SDL_Texture* fondo;
    unsigned botones; //3 a 8
    int* tonosBotones;
    int mx; //coordenada x del mouse
    int my; //coordenada y del mouse

} tJuego;

bool sdl_inicializar(tJuego* juego);
void sdl_limpiar(tJuego* juego);

#endif // TP_SIMON_H_INCLUDED
