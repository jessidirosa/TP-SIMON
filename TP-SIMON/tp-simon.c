#include "tp-simon.h"

bool sdl_inicializar(tJuego* juego)
{
    if (SDL_Init(SDL_INIT_EVERYTHING)) //siempre se inicializa, como TDA
    {
        printf("Error inicializando SDL: %s\n", SDL_GetError());
        return false;
    }

    ///ventana: puntero a la ventana que creamos
    juego->ventana = SDL_CreateWindow("SIMON GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ALTO_VENTANA, ANCHO_VENTANA, 0);
    if(!juego->ventana)
    {
        printf("Error creando ventana: %s\n", SDL_GetError());
        return false;
    }

    ///"aceleracion por hardware":
//    Que las operaciones de dibujo no las haga la CPU en software,
//    sino que se las delegue a la GPU / placa de video, que está especializada para eso
//    un rendered para dibujar se puede crear en sw (mas lento) o hw (mas rapido), por eso es este flag


     //SDL trabaja con renderers para dibujar.
    //Un renderer está ligado a una ventana y se encarga de usar la GPU (si hay) o software para pintar
    ///rend: lienzo para dibujar
    juego->render = SDL_CreateRenderer(juego->ventana, -1, SDL_RENDERER_ACCELERATED);
    if(!juego->render)
    {
        printf("Error creando render: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void sdl_limpiar(tJuego* juego)
{
    //destruimos render
    SDL_DestroyRenderer(juego->render);
    //destruimos la ventana creada, pues mem dinamica
    SDL_DestroyWindow(juego->ventana);
    //cierra todos los subsistemas que SDL abra y use
    SDL_Quit();
    return;
}
