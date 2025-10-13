#ifndef TP_SIMON_H_INCLUDED
#define TP_SIMON_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL_ttf.h>

#define ALTO_VENTANA 680
#define ANCHO_VENTANA 680
#define ERROR_INICIALIZACION -1;

#define JUGANDO 1
#define CONFIG 2
#define MENU 3
#define SALIR 4
#define MODOS 5
#define BOTONES 6
#define DURACION_INICIAL 7


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
    int instancia;

} tJuego;

typedef struct
{
    SDL_Rect destino;
    TTF_Font* fuente;
    SDL_Texture* textura;
    SDL_Surface* surface;

} tBotones;

bool sdl_inicializar(tJuego* juego);
void sdl_limpiar(tJuego* juego);

#endif // TP_SIMON_H_INCLUDED
