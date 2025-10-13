#include "sdl.h"
#include "juego.h"
#include "sonidos.h"
#include "dibujos.h"
#include "menus.h"

void menuInicial(tJuego* juego, tPartida* partida)
{
    tBotones jugar;
    tBotones config;
    SDL_Event evento;
    //limpia el lienzo
    SDL_RenderClear(juego->render);
    //pega fondo en el render
    SDL_RenderCopy(juego->render, juego->fondo, NULL, NULL);

    dibujarMenu(juego, &jugar, &config);
    SDL_PollEvent(&evento);

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, jugar.destino.x, jugar.destino.y, ANCHO_BOTON, ALTO_BOTON))
        juego->instancia = JUGANDO;

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, config.destino.x, config.destino.y, ANCHO_BOTON, ALTO_BOTON))
        juego->instancia = CONFIG;

    if(evento.type == SDL_QUIT && puntoEnRectangulo(evento.button.x, evento.button.y, config.destino.x, config.destino.y, ANCHO_BOTON, ALTO_BOTON))
        juego->instancia = SALIR;


    SDL_DestroyTexture(jugar.textura);
    SDL_DestroyTexture(config.textura);

    TTF_CloseFont(jugar.fuente);
    TTF_CloseFont(config.fuente);
}

void menuPausa(tJuego* juego)
{
    tBotones reanudar;
    tBotones volverAInicio;
    dibujarMenuPausa(juego, reanudar, volverAInicio);

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, reanudar.destino.x, reanudar.destino.y, ANCHO_BOTON, ALTO_BOTON))
    {
        juego->instancia = JUGANDO;
        SDL_RenderClear(juego->render);
    }


}
