#include "sdl.h"
#include "juego.h"
#include "sonidos.h"
#include "dibujos.h"
#include "menus.h"

void menuInicial(tJuego* juego, tPartida* partida) ///faltaria agregar el tema de que ponga su nombre por el ranking
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

    if(evento.type == SDL_QUIT || puntoEnRectangulo(evento.button.x, evento.button.y, config.destino.x, config.destino.y, ANCHO_BOTON, ALTO_BOTON))
        juego->instancia = SALIR;


    SDL_DestroyTexture(jugar.textura);
    SDL_DestroyTexture(config.textura);

    TTF_CloseFont(jugar.fuente);
    TTF_CloseFont(config.fuente);
}

void menuConfig(tJuego* juego, tPartida* partido)
{
    tBotones modos;
    tBotones duracionInicial;
    tBotones cantBotones;
    tBotones atras;
    SDL_Event evento;

    //limpia el lienzo
    SDL_RenderClear(juego->render);
    //pega fondo en el render
    SDL_RenderCopy(juego->render, juego->fondo, NULL, NULL);
    dibujarMenuConfig(juego, &modos, &duracionInicial, &cantBotones, &atras);
    SDL_PollEvent(&evento);

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, modos.destino.x, modos.destino.y, ANCHO_BOTON, ALTO_BOTON))
    {
        ///3 botones con los modos, se guarda en partida->modoJuego
    }

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, duracionInicial.destino.x, duracionInicial.destino.y, ANCHO_BOTON, ALTO_BOTON))
    {
        ///carga por teclado?
    }

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, cantBotones.destino.x, cantBotones.destino.y, ANCHO_BOTON, ALTO_BOTON))
    {
        ///6 botones con los num del 3 al 8 para que elija, y eso se guarda en juego->botones
    }

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, atras.destino.x, atras.destino.y, ANCHO_BOTON, ALTO_BOTON))
        juego->instancia = MENU;

}

///si llegamos se hace, sino no
//void menuPausa(tJuego* juego)
//{
//    tBotones reanudar;
//    tBotones volverAInicio;
//    SDL_Event evento;
//    dibujarMenuPausa(juego, &reanudar, &volverAInicio);
//
//    SDL_PollEvent(&evento);
//
//    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, reanudar.destino.x, reanudar.destino.y, ANCHO_BOTON, ALTO_BOTON))
//    {
//        juego->instancia = JUGANDO;
//        SDL_RenderClear(juego->render);
//    }
//
//
//}
