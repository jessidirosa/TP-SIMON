#include "dibujos.h"
#include "sdl.h"
#include "juego.h"
#include "sonidos.h"

int main(int argc, char* argv[])
{
    tJuego juego;
    SDL_Event evento;

    if(!sdl_inicializar(&juego))
    {
        sdl_limpiar(&juego);
        return ERROR_INICIALIZACION;
    }

    // Inicializa SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        printf("[ERROR] No se pudo inicializar el audio: %s\n", Mix_GetError());

    Mix_Chunk* sonido = cargarSonido("sonidos/sonido.wav");
    Mix_Chunk* tono = crearTono(440.0f);

    while(true)
    {
        while(SDL_PollEvent(&evento)) //se usa para sacar todos los eventos que van ocurriendo y ver que hacemos uno por uno
        {
            switch (evento.type)
            {
            case SDL_QUIT: //sale cuando se toca la X para cerrar la pestaña
                sdl_limpiar(&juego);
                break;
            default:
                break;

            case SDL_KEYDOWN: //en el caso de tocar una letra
                switch(evento.key.keysym.scancode) //puntualmanete una letra especifica, esta en todo ese enjambre de structs
                {
                case SDL_SCANCODE_ESCAPE: //la tecla ESC
                    sdl_limpiar(&juego);
                    break;
                default:
                    break;
                }
            }
        }

        //limpia el lienzo
        SDL_RenderClear(juego.render);



        //presenta lo hecho en el (todavia nada)
        SDL_RenderPresent(juego.render);

        //sin chequear el evento de quit, la ventana pasados los 5 segundos se cierra. precisamos captar esos eventos
        //para decidir que hacer con ellos
    }

    sdl_limpiar(&juego);

    return 0;
}
