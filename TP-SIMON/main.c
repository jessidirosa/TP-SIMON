#include "dibujos.h"
#include "sdl.h"
#include "juego.h"
#include "sonidos.h"
#include "menus.h"

int main(int argc, char* argv[])
{
    tJuego juego;
    SDL_Event evento;
    bool corriendo = true;
    tPartida partida;
    tBotones iniciar;

    int **mat = crearMatriz(MAT_FILA, MAT_COL, sizeof(int));
    if(!mat)
        return ERROR;

    juego.tonosBotones = malloc(sizeof(int) * MAX_BOTON);
    if(!juego.tonosBotones)
        return ERROR;

    if(!sdl_inicializar(&juego))
    {
        sdl_limpiar(&juego);
        return ERROR_INICIALIZACION;
    }

    configPorDefecto(&juego, &partida);//por si el jugador no configura nada a la primera
    while(corriendo)
    {
        if(juego.instancia == MENU)
            menuInicial(&juego, &partida);

        if(juego.instancia == SALIR)
            corriendo = false;

        if(juego.instancia == CONFIG)
            menuConfig(&juego, &partida);

        if(juego.instancia == MODOS)
            menuModos(&juego, &partida);

        if(juego.instancia == BOTONES)
            menuCantBotones(&juego, &partida);

        if(juego.instancia == DURACION_INICIAL)
            menuDuracionInicial(&juego, &partida);

        if(juego.instancia == JUGANDO)
        {
            dibujar(&juego, mat);
            dibujarBordes(&juego);
            dibujarBotonCentro(&juego, &iniciar, "Iniciar");
            //presenta lo hecho en el render
            SDL_RenderPresent(juego.render);
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
                    if(puntoDentroCirculo(evento.button.x, evento.button.y, CENTRO_PLAY_X, CENTRO_PLAY_Y, R_INT))
                        iniciarJuego(&partida, &juego, mat);

                    break;
                }
            }
        }


    }

    sdl_limpiar(&juego);
    liberarMemoria(&juego, mat, &partida);

    return 0;
}
