#include "sdl.h"
#include "juego.h"
#include "sonidos.h"
#include "dibujos.h"
#include "menus.h"

void menuInicial(tJuego* juego, tPartida* partida) ///faltaria agregar el tema de que ponga su nombre por el ranking
{
    tBotones jugar;
    tBotones config;
    tBotones ranking;
    tBotones salir;
    SDL_Event evento;
    //limpia el lienzo
    SDL_RenderClear(juego->render);
    //pega fondo en el render
    SDL_RenderCopy(juego->render, juego->fondo, NULL, NULL);
    dibujarMenu(juego, &jugar, &config, &salir, &ranking);
    SDL_RenderPresent(juego->render);
    SDL_PollEvent(&evento);

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, jugar.destino.x, jugar.destino.y, jugar.destino.w, jugar.destino.h))
        juego->instancia = JUGANDO;

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, config.destino.x, config.destino.y, config.destino.w, config.destino.h))
        juego->instancia = CONFIG;

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, ranking.destino.x, ranking.destino.y, ranking.destino.w, ranking.destino.h))
        juego->instancia = RANKING;

    if(evento.type == SDL_QUIT || (evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, salir.destino.x, salir.destino.y, salir.destino.w, salir.destino.h)))
        juego->instancia = SALIR;
}

void menuConfig(tJuego* juego, tPartida* partida)
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
    SDL_RenderPresent(juego->render);
    SDL_PollEvent(&evento);

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, modos.destino.x, modos.destino.y, modos.destino.w, modos.destino.h))
        juego->instancia = MODOS;

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, duracionInicial.destino.x, duracionInicial.destino.y, duracionInicial.destino.w, duracionInicial.destino.h))
        juego->instancia = DURACION_INICIAL;

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, cantBotones.destino.x, cantBotones.destino.y, cantBotones.destino.w, cantBotones.destino.h))
        juego->instancia = BOTONES;

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, atras.destino.x, atras.destino.y, atras.destino.w, atras.destino.h))
        juego->instancia = MENU;

    if(evento.type == SDL_QUIT)
        juego->instancia = SALIR;

}

void menuModos(tJuego* juego, tPartida* partida)
{
    tBotones modoSchonberg;
    tBotones modoMozart;
    tBotones modoDesafio;
    tBotones atras;
    SDL_Event evento;

    SDL_RenderClear(juego->render);
    SDL_RenderCopy(juego->render, juego->fondo, NULL, NULL);
    dibujarMenuModos(juego, &modoSchonberg, &modoMozart, &modoDesafio, &atras);
    SDL_RenderPresent(juego->render);

    SDL_PollEvent(&evento);
    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, modoSchonberg.destino.x, modoSchonberg.destino.y, modoSchonberg.destino.w, modoSchonberg.destino.h))
    {
        partida->modoJuego = SCHONBERG;
        juego->instancia = CONFIG;
    }
    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, modoMozart.destino.x, modoMozart.destino.y, modoMozart.destino.w, modoMozart.destino.h))
    {
        partida->modoJuego = MOZART; //de momento no existe
        juego->instancia = CONFIG;
    }

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, modoDesafio.destino.x, modoDesafio.destino.y, modoDesafio.destino.w, modoDesafio.destino.h))
    {
        partida->modoJuego = DESAFIO; //de momento no existe
        juego->instancia = CONFIG;
    }

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, atras.destino.x, atras.destino.y, atras.destino.w, atras.destino.h))
        juego->instancia = CONFIG;
}

void menuCantBotones(tJuego* juego, tPartida* partida)
{
    tBotones b3, b4, b5, b6, b7, b8;
    tBotones atras;
    SDL_Event evento;

    SDL_RenderClear(juego->render);
    SDL_RenderCopy(juego->render, juego->fondo, NULL, NULL);
    dibujarMenuBotones(juego, &b3, &b4, &b5, &b6, &b7, &b8, &atras);
    SDL_RenderPresent(juego->render);

    SDL_PollEvent(&evento);
    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, b3.destino.x, b3.destino.y, b3.destino.w, b3.destino.h))
    {
        juego->botones = 3;
        juego->instancia = CONFIG;
    }

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, b4.destino.x, b4.destino.y, b4.destino.w, b4.destino.h))
    {
        juego->botones = 4;
        juego->instancia = CONFIG;
    }

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, b5.destino.x, b5.destino.y, b5.destino.w, b5.destino.h))
    {
        juego->botones = 5;
        juego->instancia = CONFIG;
    }

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, b6.destino.x, b6.destino.y, b6.destino.w, b6.destino.h))
    {
        juego->botones = 6;
        juego->instancia = CONFIG;
    }

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, b7.destino.x, b7.destino.y, b7.destino.w, b7.destino.h))
    {
        juego->botones = 7;
        juego->instancia = CONFIG;
    }

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, b8.destino.x, b8.destino.y, b8.destino.w, b8.destino.h))
    {
        juego->botones = 8;
        juego->instancia = CONFIG;
    }

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, atras.destino.x, atras.destino.y, atras.destino.w, atras.destino.h))
        juego->instancia = CONFIG;

}

//void menuDuracionInicial(tJuego* juego, tPartida* partida)
//{
//    SDL_Event evento;
//    char* buffer = malloc(sizeof(MAX_DURACION));
//    SDL_StartTextInput();
//
//    if(evento.type == SDL_TEXTINPUT)
//    {
//
//    }
//    //en proceso
//
//
//    SDL_StopTextInput();
//}


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
