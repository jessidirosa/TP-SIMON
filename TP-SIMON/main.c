#include "dibujos.h"
#include "sdl.h"
#include "juego.h"
#include "sonidos.h"

int main(int argc, char* argv[])
{
    tJuego juego;
    SDL_Event evento;
    bool corriendo = true;
    Mix_Chunk* sonido;
    Mix_Chunk* tono;
    tPartida partida;

    int **mat = crearMatriz(MAT_FILA, MAT_COL, sizeof(int));
    if(!mat)
        return ERROR;

    if(!sdl_inicializar(&juego))
    {
        sdl_limpiar(&juego);
        return ERROR_INICIALIZACION;
    }

    // Inicializa SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        printf("[ERROR] No se pudo inicializar el audio: %s\n", Mix_GetError());

    sonido = cargarSonido("sonidos/sonido.wav");
    tono = crearTono(440.0f);

        ///esto va despues dentro de iniciarJuego
    inicializarPartida(&partida);

    while(corriendo)
    {

        //limpia el lienzo
        SDL_RenderClear(juego.render);

        //pega fondo en el render
        SDL_RenderCopy(juego.render, juego.fondo, NULL, NULL);
        ///esto entra del menu
        juego.botones = 8;
        dibujar(&juego, mat);
        dibujarBordes(&juego);
        //presenta lo hecho en el render
        SDL_RenderPresent(juego.render);
        SDL_Delay(16);

        ///esto va despues dentro de iniciarJuego
        secuenciaJuego(&partida, &juego, mat);

        //precisamos captar cada evento
        //para decidir que hacer con el
        while(SDL_PollEvent(&evento)) //se usa para sacar todos los eventos que van ocurriendo y ver que hacemos uno por uno
        {
            switch (evento.type)
            {
            case SDL_QUIT: //sale cuando se toca la X para cerrar la pestaña
                corriendo = false;
                break;
            default:
                break;

            case SDL_KEYDOWN: //en el caso de tocar una letra
                switch(evento.key.keysym.scancode) //puntualmanete una letra especifica, esta en todo ese enjambre de structs
                {
                case SDL_SCANCODE_ESCAPE: //la tecla ESC
                    corriendo = false;
                    break;
                default:
                    break;
                }

            case SDL_MOUSEBUTTONDOWN://guardamos coordenadas de a donde toco para iluminar boton y ver si esta bien o no
                //toca Play (x,y)

                juego.mx = evento.button.x;
                juego.my = evento.button.y;
                printf("Click detectado en: %d %d\n", juego.mx, juego.my);

                if(botonSeleccionar(&juego) > 0)
                {
                    iluminarBoton(botonSeleccionar(&juego), &juego, mat);

                }

                printf("Boton seleccionado: %d\n\n", botonSeleccionar(&juego));
                break;
            }
        }
    }

    sdl_limpiar(&juego);
    free(mat); //destruirla bien

    return 0;
}
