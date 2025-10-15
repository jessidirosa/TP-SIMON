#include "sdl.h"
#include "juego.h"
#include "sonidos.h"
#include "dibujos.h"
#include "menus.h"

void menuInicial(tJuego* juego, tPartida* partida)
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
    SDL_PollEvent(&evento);

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, jugar.destino.x, jugar.destino.y, jugar.destino.w, jugar.destino.h))
        juego->instancia = JUGANDO;

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, config.destino.x, config.destino.y, config.destino.w, config.destino.h))
        juego->instancia = CONFIG;

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, ranking.destino.x, ranking.destino.y, ranking.destino.w, ranking.destino.h))
        juego->instancia = RANK_ORDENADO;

    if(evento.type == SDL_QUIT || (evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, salir.destino.x, salir.destino.y, salir.destino.w, salir.destino.h)))
        juego->instancia = SALIR;


    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, jugar.destino.x, jugar.destino.y, jugar.destino.w, jugar.destino.h))
        {
            SDL_SetRenderDrawBlendMode(juego->render, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(juego->render, 0, 0, 0, 180); // negro con transparencia
            SDL_Rect overlay = {0,0, ANCHO_VENTANA, ALTO_VENTANA};
            SDL_RenderFillRect(juego->render, &overlay);
            SDL_RenderPresent(juego->render);


            TTF_Font* fuente = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf",20);
            if (!fuente)
            {
                printf("Error al cargar fuente: %s\n", TTF_GetError());
            }
            else
            {

                pedirNombreJugador(juego, fuente, partida->ranking.jugador, sizeof(partida->ranking.jugador));
                TTF_CloseFont(fuente);
            }

            juego->instancia = JUGANDO;
        }

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
    SDL_PollEvent(&evento);
    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, modoSchonberg.destino.x, modoSchonberg.destino.y, modoSchonberg.destino.w, modoSchonberg.destino.h))
    {
        partida->modoJuego = SCHONBERG;
        juego->instancia = CONFIG;
    }
    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, modoMozart.destino.x, modoMozart.destino.y, modoMozart.destino.w, modoMozart.destino.h))
    {
        partida->modoJuego = MOZART;
        juego->instancia = CONFIG;
    }

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, modoDesafio.destino.x, modoDesafio.destino.y, modoDesafio.destino.w, modoDesafio.destino.h))
    {
        partida->modoJuego = DESAFIO;
        juego->instancia = BOTONES_DESAFIO;
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

void menuCantBotonesDesafio(tJuego* juego, tPartida* partida)
{
    tBotones b3, b4, b5, b6, b7, b8;
    tBotones atras;
    SDL_Event evento;

    SDL_RenderClear(juego->render);
    SDL_RenderCopy(juego->render, juego->fondo, NULL, NULL);
    dibujarMenuBotones(juego, &b3, &b4, &b5, &b6, &b7, &b8, &atras);
    SDL_PollEvent(&evento);
    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, b3.destino.x, b3.destino.y, b3.destino.w, b3.destino.h))
    {
        juego->botones = 3;
        juego->instancia = MODO_DESAFIO;
    }

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, b4.destino.x, b4.destino.y, b4.destino.w, b4.destino.h))
    {
        juego->botones = 4;
        juego->instancia = MODO_DESAFIO;
    }

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, b5.destino.x, b5.destino.y, b5.destino.w, b5.destino.h))
    {
        juego->botones = 5;
        juego->instancia = MODO_DESAFIO;
    }

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, b6.destino.x, b6.destino.y, b6.destino.w, b6.destino.h))
    {
        juego->botones = 6;
        juego->instancia = MODO_DESAFIO;
    }

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, b7.destino.x, b7.destino.y, b7.destino.w, b7.destino.h))
    {
        juego->botones = 7;
        juego->instancia = MODO_DESAFIO;
    }

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, b8.destino.x, b8.destino.y, b8.destino.w, b8.destino.h))
    {
        juego->botones = 8;
        juego->instancia = MODO_DESAFIO;
    }

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, atras.destino.x, atras.destino.y, atras.destino.w, atras.destino.h))
        juego->instancia = CONFIG;


}


void pedirNombreJugador(tJuego* juego, TTF_Font* fuente, char* destino, int maxLen)
{
    SDL_Event evento;
    SDL_StartTextInput();
    destino[0] = '\0';
    SDL_Color blanco = {255,255,255,255};

    int escribiendo = 1;
    Uint32 ultimoBlink = SDL_GetTicks();
    int mostrarCursor = 1;

    while (escribiendo)
    {
        // cursor parpadeante cada 500 ms
        if (SDL_GetTicks() - ultimoBlink > 500) {
            mostrarCursor = !mostrarCursor;
            ultimoBlink = SDL_GetTicks();
        }

        while (SDL_PollEvent(&evento))
        {
            // DEBUG: imprime eventos para que veas si llegan
            // printf("Evento: %d\n", evento.type);

            if (evento.type == SDL_QUIT) {
                escribiendo = 0;
                break;
            }

            if (evento.type == SDL_WINDOWEVENT) {
                if (evento.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
                    printf("Ventana: foco ganado\n");
                if (evento.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
                    printf("Ventana: foco perdido\n");
            }

            if (evento.type == SDL_KEYDOWN)
            {
                if (evento.key.keysym.sym == SDLK_RETURN) {
                    escribiendo = 0;
                    break;
                }
                if (evento.key.keysym.sym == SDLK_ESCAPE) {
                    destino[0] = '\0';
                    escribiendo = 0;
                    break;
                }
                if (evento.key.keysym.sym == SDLK_BACKSPACE && strlen(destino) > 0)
                    destino[strlen(destino) - 1] = '\0';
            }

            if (evento.type == SDL_TEXTINPUT)
            {
                // DEBUG: ves en consola lo que SDL entrega
                // printf("SDL_TEXTINPUT: '%s'\n", evento.text.text);

                if ((int)strlen(destino) + (int)strlen(evento.text.text) < maxLen) {
                    strcat(destino, evento.text.text);
                }
            }
        }

        // ---------- DIBUJADO ----------
        // Limpio pantalla (si querés overlay en lugar de limpiar toda la pantalla,
        // comentar RenderClear y usar SDL_RenderFillRect con blendmode)
        SDL_SetRenderDrawColor(juego->render, 0, 0, 0, 255);
        SDL_RenderClear(juego->render);
        SDL_RenderCopy(juego->render, juego->fondo, NULL, NULL);
        //SDL_RenderFillRect(juego->render, &overlay);

        // Prompt
        SDL_Surface* sPrompt = TTF_RenderText_Solid(fuente, "Ingrese su nombre:", blanco);
        if (sPrompt) {
            SDL_Texture* tPrompt = SDL_CreateTextureFromSurface(juego->render, sPrompt);
            SDL_Rect rPrompt = {10,75, sPrompt->w, sPrompt->h};
            SDL_RenderCopy(juego->render, tPrompt, NULL, &rPrompt);
            SDL_DestroyTexture(tPrompt);
            SDL_FreeSurface(sPrompt);
        } else {
            printf("Error render prompt: %s\n", TTF_GetError());
        }

        // Texto usuario (o cursor si vacío)
        char buf[256];
        if (strlen(destino) == 0) {
            // mostrar cursor si no escribió nada
            if (mostrarCursor)
                snprintf(buf, sizeof(buf), "_");
            else
                snprintf(buf, sizeof(buf), " ");
        } else {
            if (mostrarCursor)
                snprintf(buf, sizeof(buf), "%s_", destino);
            else
                snprintf(buf, sizeof(buf), "%s", destino);
        }

        SDL_Surface* sText = TTF_RenderText_Solid(fuente, buf, blanco);
        if (sText) {
            SDL_Texture* tText = SDL_CreateTextureFromSurface(juego->render, sText);
            SDL_Rect rText = {10,110, sText->w, sText->h};
            SDL_RenderCopy(juego->render, tText, NULL, &rText);
            SDL_DestroyTexture(tText);
            SDL_FreeSurface(sText);
        }
        SDL_RenderPresent(juego->render);

        //pequeña espera para no saturar CPU
        SDL_Delay(10);
    }

    SDL_StopTextInput();
}

bool mostrarRanking(tJuego* juego, tPartida* partida, char* nombreArch, tRanking* vecRanking, int* ce, int* maxTam)
{
    int y = 70;
    int i;
    tBotones atras;
    SDL_Color color = {255,255,255,255};
    SDL_Event evento;
    char bufPuntaje[5];
    int limite;

    (*ce) = 0;
    SDL_RenderClear(juego->render);
    SDL_RenderCopy(juego->render, juego->fondo, NULL, NULL);
    dibujarTextos("Estadisticas: TOP 5", juego, 15, 40, "fnt/aero_2/Aero.ttf", 18, color);
    dibujarBotones(&atras, "Atras", juego, 35, 160, "fnt/vcr_osd_mono/VCR_OSD_MONO_1.001.ttf", 16, color);

    if(ordenarArchivo(nombreArch, &vecRanking, ce, maxTam, juego) == VACIO)
        dibujarTextos("No hay datos aun", juego, 25,  70, "fnt/aero_2/Aero.ttf", 12, color);

    limite = ((*ce) < TOP) ? (*ce) : TOP;
    for (i = 0; i < limite; ++i)
    {
        snprintf(bufPuntaje, sizeof(bufPuntaje), "%d", vecRanking->score);
        dibujarTextos(vecRanking->jugador, juego, 50,  y, "fnt/aero_2/Aero.ttf", 12, color);
        dibujarTextos(bufPuntaje, juego, 130, y, "fnt/aero_2/Aero.ttf", 12, color);

        y += 14;
        vecRanking++;
    }

    SDL_RenderPresent(juego->render);
    SDL_DestroyTexture(atras.textura);
    TTF_CloseFont(atras.fuente);

    SDL_PollEvent(&evento);
    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, atras.destino.x, atras.destino.y, atras.destino.w, atras.destino.h))
        juego->instancia = MENU;

    return true;
}

void menuDuracionInicial(tJuego* juego, tPartida* partida)
{
    tBotones d1000, d1500, d2000;
    tBotones atras;
    SDL_Event evento;

    SDL_RenderClear(juego->render);
    SDL_RenderCopy(juego->render, juego->fondo, NULL, NULL);
    dibujarMenuDuracion(juego, &d1000, &d1500, &d2000, &atras);
    SDL_PollEvent(&evento);
    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, d2000.destino.x, d2000.destino.y, d2000.destino.w, d2000.destino.h))
    {
        partida->tiempoNota = TIEMPO_FACIL;
        juego->instancia = CONFIG;
    }

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, d1500.destino.x, d1500.destino.y, d1500.destino.w, d1500.destino.h))
    {
        partida->tiempoNota = TIEMPO_MEDIO;
        juego->instancia = CONFIG;
    }

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, d1000.destino.x, d1000.destino.y, d1000.destino.w, d1000.destino.h))
    {
        partida->tiempoNota = TIEMPO_PRO;
        juego->instancia = CONFIG;
    }

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, atras.destino.x, atras.destino.y, atras.destino.w, atras.destino.h))
        juego->instancia = CONFIG;

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
