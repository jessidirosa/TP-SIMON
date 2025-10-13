#include "sdl.h"
#include "juego.h"
#include "sonidos.h"
#include "dibujos.h"
#include "menus.h"

bool sdl_inicializar(tJuego* juego)
{
    if (SDL_Init(SDL_INIT_EVERYTHING)) //siempre se inicializa, como TDA
    {
        printf("Error inicializando SDL: %s\n", SDL_GetError());
        return false;
    }

    if(TTF_Init())
    {
        printf("Error inicializando TTF: %s\n", TTF_GetError());
        return false;
    }

    ///ventana: puntero a la ventana que creamos
    juego->ventana = SDL_CreateWindow("SIMON GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ANCHO_VENTANA, ALTO_VENTANA, 0);
    if(!juego->ventana)
    {
        printf("Error creando ventana: %s\n", SDL_GetError());
        return false;
    }

    ///"aceleracion por hardware":
//    Que las operaciones de dibujo no las haga la CPU en software,
//    sino que se las delegue a la GPU / placa de video, que está especializada para eso
//    un rendered para dibujar se puede crear en sw (mas lento) o hw (mas rapido), por eso es este flag

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

     //SDL trabaja con renderers para dibujar.
    //Un renderer está ligado a una ventana y se encarga de usar la GPU (si hay) o software para pintar
    ///render: lienzo para dibujar
    juego->render = SDL_CreateRenderer(juego->ventana, -1, SDL_RENDERER_ACCELERATED);
    if(!juego->render)
    {
        printf("Error creando render: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawBlendMode(juego->render, SDL_BLENDMODE_BLEND);

    int ancho_log = MAT_COL * TAM_PIXEL;
    int alto_log = MAT_FILA * TAM_PIXEL;

    SDL_RenderSetLogicalSize(juego->render, ancho_log, alto_log);

    //crea surface, carga a ram el archivo bmp
    juego->surface = SDL_LoadBMP("fondo.bmp");

    if (!juego->surface)
        SDL_Log("No se pudo cargar BMP: %s", SDL_GetError());


    juego->fondo = SDL_CreateTextureFromSurface(juego->render, juego->surface);
    if (!juego->fondo)
        SDL_Log("No se pudo crear fondo: %s", SDL_GetError());


    SDL_FreeSurface(juego->surface);  // ya no se necesita (queda la Texture)

    // Inicializa SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        printf("[ERROR] No se pudo inicializar el audio: %s\n", Mix_GetError());

    juego->instancia = MENU;

    return true;
}



void sdl_limpiar(tJuego* juego)
{
    //destruimos render
    SDL_DestroyRenderer(juego->render);
    //destruimos la ventana creada, pues mem dinamica
    SDL_DestroyWindow(juego->ventana);
    TTF_Quit();
    //cierra todos los subsistemas que SDL abra y use
    SDL_Quit();
    return;
}


