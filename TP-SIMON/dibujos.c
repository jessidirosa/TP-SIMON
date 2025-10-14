#include "sdl.h"
#include "juego.h"
#include "sonidos.h"
#include "dibujos.h"
#include "menus.h"

SDL_Color colores[] =
{
    {255, 255, 0,   240}, // AM[0] - Amarillo
    {0,   150, 60,  240}, // V[1] - Verde
    {255,   0,  0,  240}, // R[2] - Rojo
    {0,   0,  255,  240}, // AZ[3] - Azul
    {128,   0,  255,  240}, // VI[4] - Violeta
    {255,   192,  203,  240}, // RO[5] - Rosa
    {255,   165,  0,  240}, // NA[6] - Naranja
    {178, 255, 255, 240}, // CE[7] - Celeste
    {255, 255, 255, 240}, // BL[8] - Blanco
    {0,   0,  0,  0}, // T[9] - Transparente
    {0, 0, 0, 255} // N[10] - Negro
};

void dibujar(tJuego* juego, int** mat)
{
    int i;
    int j;

    double cx = ((MAT_COL-1)/2);
    double cy = ((MAT_FILA-1)/2);

    double dx;
    double dy;
    double dist;
    double ang;
    int sector;

    SDL_RenderClear(juego->render);
    //pega fondo en el render
    SDL_RenderCopy(juego->render, juego->fondo, NULL, NULL);


    for(i=0;i<MAT_FILA;i++)
    {
        for(j=0;j<MAT_COL;j++)
        {
            dx = j - cx;
            dy = i - cy;
            dist = sqrt(dx * dx + dy * dy);
            if(dist > R_INT && dist < R_EXT)
            {
                ang = atan2(dy, dx);
                if(ang < 0)
                    ang += (2 * M_PI); //una vuelta mas para que quede positivo

                sector = (int)(ang / ((2*M_PI)/juego->botones));
                mat[i][j] = sector;
            }
            else
               mat[i][j] = T;
        }
    }

    //pintamos
    for(i=0;i<MAT_FILA;i++)
    {
        for(j=0;j<MAT_COL;j++)
        {
            if (mat[i][j] != T)
            {
                SDL_SetRenderDrawColor(juego->render,
                                   colores[mat[i][j]].r,
                                   colores[mat[i][j]].g,
                                   colores[mat[i][j]].b,
                                   colores[mat[i][j]].a);
                SDL_Rect pixel = {TAM_PIXEL*j, TAM_PIXEL*i, TAM_PIXEL, TAM_PIXEL};
                SDL_RenderFillRect(juego->render, &pixel);
            }
        }
    }
}

void iluminarBoton(int boton, tJuego* juego, int** mat)
{
    int i, j;

    for(i=0;i<MAT_FILA;i++)
    {
        for(j=0;j<MAT_COL;j++)
        {
            if(mat[i][j] == boton)
                {
                    SDL_SetRenderDrawColor(juego->render, 255, 255, 255, 150);
                    SDL_Rect pixel = {TAM_PIXEL*j, TAM_PIXEL*i, TAM_PIXEL, TAM_PIXEL};
                    SDL_RenderFillRect(juego->render, &pixel);
                }
        }
    }

    reproducirNota(boton, juego);
    SDL_RenderPresent(juego->render);
    SDL_Delay(100);
}

void apagarBoton(int boton, tJuego* juego, int** mat)
{
    int i, j;

    SDL_Color color = colores[boton]; // Tomamos el color original del botón

    for(i = 0; i < MAT_FILA; i++)
    {
        for(j = 0; j < MAT_COL; j++)
        {
            if(mat[i][j] == boton)
            {
                SDL_SetRenderDrawColor(juego->render, color.r, color.g, color.b, 255); // Forzamos alfa 255
                SDL_Rect pixel = {TAM_PIXEL * j, TAM_PIXEL * i, TAM_PIXEL, TAM_PIXEL};
                SDL_RenderFillRect(juego->render, &pixel);
            }
        }
    }
    dibujarBordes(juego);
    SDL_RenderPresent(juego->render);
}

void dibujarBordes(tJuego* juego) //ver cuando se tienen que redibujar que pasa (esta raro)
{
    double ang_boton = (2 * M_PI)/juego->botones;
    double x1, x2, y1, y2, ang;
    double cx = ((MAT_COL-1)/2);
    double cy = ((MAT_FILA-1)/2);
    int i;

    SDL_SetRenderDrawColor(juego->render, 205, 205, 205, 255);

    for(i=0; i<juego->botones; i++)
    {
        ang = i * ang_boton;
        x1 = cx + (R_INT * cos(ang));
        x2 = cx + (R_EXT * cos(ang));
        y1 = cy + (R_INT * sin(ang));
        y2 = cy + (R_EXT * sin(ang));
        SDL_RenderDrawLine(juego->render, x1, y1, x2, y2);
        SDL_RenderDrawLine(juego->render, x1, y1, x2, y2);
        SDL_RenderDrawLine(juego->render, x1+0.25, y1+0.25, x2+0.25, y2+0.25);
        SDL_RenderDrawLine(juego->render, x1+0.5, y1+0.5, x2+0.5, y2+0.5);
        SDL_RenderDrawLine(juego->render, x1+1, y1+1, x2+1, y2+1);

    }

}

int** crearMatriz(int cf, int cc, size_t tam)
{
    int **mat = malloc(sizeof(int*) * cf);
    if(!mat)
        return NULL;

    int **fila;
    int **col;
    int **fin = mat + cf;

    for(fila=mat;fila<fin;fila++)
    {
       *fila = malloc(tam * cc);
       if(!(*fila))
       {
            for(col=mat;col<fila;col++)
            {
                free((*col));
            }
            free(mat);
            return NULL;
       }
    }

    return mat;
}

//se podria hacer una funcion mas generica de los botones mandandoles las coord de donde se lo quiere
void dibujarBotonCentro(tJuego* juego, tBotones* boton, char* nombre)
{
    SDL_Color colorTexto = {255, 255, 255, 240}; //blanco

    boton->fuente = TTF_OpenFont("assets/porky_s/porkys_.ttf", 18);
    if (!boton->fuente)
        printf("OpenFont: %s\n", TTF_GetError());

    boton->destino.x = 88;
    boton->destino.y = 93;
    boton->destino.w = 25;
    boton->destino.h = 10;

    boton->surface = TTF_RenderUTF8_Blended(boton->fuente, nombre, colorTexto);
    boton->textura = SDL_CreateTextureFromSurface(juego->render, boton->surface);
    SDL_FreeSurface(boton->surface);
    SDL_RenderCopy(juego->render, boton->textura, NULL, &(boton->destino));
    SDL_RenderPresent(juego->render);

    SDL_DestroyTexture(boton->textura);
    TTF_CloseFont(boton->fuente);
}

void dibujarMenu(tJuego* juego, tBotones* jugar, tBotones* config, tBotones* salir, tBotones* ranking)
{
    TTF_Font* titulo;
    SDL_Texture* textTitulo;
    SDL_Surface* surfTitulo;
    SDL_Rect destTitulo;
    SDL_Color colorTexto = {255, 255, 255, 240}; //blanco
    int anchoTitulo, altoTitulo;

    titulo = TTF_OpenFont("assets/aero_2/Aero.ttf", 24);
    if (!titulo)
        printf("OpenFont: %s\n", TTF_GetError());

    TTF_SizeUTF8(titulo, "Simon Game", &anchoTitulo, &altoTitulo);

    destTitulo.x = 25;
    destTitulo.y = 50;
    destTitulo.w = anchoTitulo;
    destTitulo.h = altoTitulo;

    surfTitulo = TTF_RenderUTF8_Blended(titulo, "Simon Game", colorTexto);
    textTitulo = SDL_CreateTextureFromSurface(juego->render, surfTitulo);
    SDL_FreeSurface(surfTitulo);

    dibujarBotones(jugar, "Jugar", juego, 75, 90, "assets/vcr_osd_mono/VCR_OSD_MONO_1.001.ttf", 16);
    dibujarBotones(config, "Configuracion", juego, 40, 110, "assets/vcr_osd_mono/VCR_OSD_MONO_1.001.ttf", 16);
    dibujarBotones(ranking, "Ranking", juego, 65, 130, "assets/vcr_osd_mono/VCR_OSD_MONO_1.001.ttf", 16);
    dibujarBotones(salir, "Salir", juego, 75, 160, "assets/vcr_osd_mono/VCR_OSD_MONO_1.001.ttf", 16);

    SDL_RenderCopy(juego->render, textTitulo, NULL, &destTitulo);

    //funcion destruir Fuentes y Texturas
    SDL_DestroyTexture(textTitulo);
    SDL_DestroyTexture(jugar->textura);
    SDL_DestroyTexture(salir->textura);
    SDL_DestroyTexture(ranking->textura);

    TTF_CloseFont(titulo);
    TTF_CloseFont(jugar->fuente);
    TTF_CloseFont(config->fuente);
    TTF_CloseFont(salir->fuente);
    TTF_CloseFont(ranking->fuente);
}

void dibujarBotones(tBotones* boton, char* nombre, tJuego* juego, int x, int y, char* fuente, int tam)
{
    SDL_Color colorTexto = {255, 255, 255, 240}; //blanco

    boton->fuente = TTF_OpenFont(fuente, tam);
    if (!boton->fuente)
        printf("OpenFont: %s\n", TTF_GetError());

    TTF_SizeUTF8(boton->fuente, nombre, &(boton->destino.w), &(boton->destino.h));

    boton->destino.x = x;
    boton->destino.y = y;

    boton->surface = TTF_RenderUTF8_Blended(boton->fuente, nombre, colorTexto);
    boton->textura = SDL_CreateTextureFromSurface(juego->render, boton->surface);
    SDL_FreeSurface(boton->surface);

    SDL_RenderCopy(juego->render, boton->textura, NULL, &(boton->destino));
}

void dibujarMenuConfig(tJuego* juego, tBotones* modos, tBotones* duracionInicial, tBotones* cantBotones, tBotones* atras)
{
    dibujarBotones(modos, "Modos de juego", juego, 35, 70, "assets/vcr_osd_mono/VCR_OSD_MONO_1.001.ttf", 16);
    dibujarBotones(duracionInicial, "Duracion Inicial", juego, 35, 90, "assets/vcr_osd_mono/VCR_OSD_MONO_1.001.ttf", 16);
    dibujarBotones(cantBotones, "Cantidad de botones", juego, 35, 110, "assets/vcr_osd_mono/VCR_OSD_MONO_1.001.ttf", 12);
    dibujarBotones(atras, "Atras", juego, 35, 140, "assets/vcr_osd_mono/VCR_OSD_MONO_1.001.ttf", 16);

    SDL_DestroyTexture(modos->textura);
    SDL_DestroyTexture(duracionInicial->textura);
    SDL_DestroyTexture(cantBotones->textura);
    SDL_DestroyTexture(atras->textura);

    TTF_CloseFont(modos->fuente);
    TTF_CloseFont(duracionInicial->fuente);
    TTF_CloseFont(cantBotones->fuente);
    TTF_CloseFont(atras->fuente);

}

void dibujarMenuModos(tJuego* juego, tBotones* modoSchonberg, tBotones* modoMozart, tBotones* modoDesafio, tBotones* atras)
{
    dibujarBotones(modoSchonberg, "Modo Schonberg", juego, 35, 70, "assets/porky_s/porkys_.ttf", 16);
    dibujarBotones(modoMozart, "Modo Mozart", juego, 35, 90, "assets/porky_s/porkys_.ttf", 16);
    dibujarBotones(modoDesafio, "Modo Desafio", juego, 35, 110, "assets/porky_s/porkys_.ttf", 16);
    dibujarBotones(atras, "Atras", juego, 35, 140, "assets/porky_s/porkys_.ttf", 16);

    SDL_DestroyTexture(modoSchonberg->textura);
    SDL_DestroyTexture(modoMozart->textura);
    SDL_DestroyTexture(modoDesafio->textura);
    SDL_DestroyTexture(atras->textura);

    TTF_CloseFont(modoSchonberg->fuente);
    TTF_CloseFont(modoMozart->fuente);
    TTF_CloseFont(modoDesafio->fuente);
    TTF_CloseFont(atras->fuente);

}

void dibujarMenuBotones(tJuego* juego, tBotones* b3, tBotones* b4, tBotones* b5, tBotones* b6, tBotones* b7, tBotones* b8, tBotones* atras)
{
    dibujarBotones(b3, "3", juego, 70, 30, "assets/dimitri/dimitri_.ttf", 30);
    dibujarBotones(b4, "4", juego, 120, 30, "assets/dimitri/dimitri_.ttf", 30);
    dibujarBotones(b5, "5", juego, 70, 80, "assets/dimitri/dimitri_.ttf", 30);
    dibujarBotones(b6, "6", juego, 120, 80, "assets/dimitri/dimitri_.ttf", 30);
    dibujarBotones(b7, "7", juego, 70, 130, "assets/dimitri/dimitri_.ttf", 30);
    dibujarBotones(b8, "8", juego, 120, 130, "assets/dimitri/dimitri_.ttf", 30);
    dibujarBotones(atras, "Atras", juego, 25, 160, "assets/porky_s/porkys_.ttf", 16);

    SDL_DestroyTexture(b3->textura);
    SDL_DestroyTexture(b4->textura);
    SDL_DestroyTexture(b5->textura);
    SDL_DestroyTexture(b6->textura);
    SDL_DestroyTexture(b7->textura);
    SDL_DestroyTexture(b8->textura);
    SDL_DestroyTexture(atras->textura);

    TTF_CloseFont(b3->fuente);
    TTF_CloseFont(b4->fuente);
    TTF_CloseFont(b5->fuente);
    TTF_CloseFont(b6->fuente);
    TTF_CloseFont(b7->fuente);
    TTF_CloseFont(b8->fuente);
    TTF_CloseFont(atras->fuente);
}


///si llegamos se hace, sino no
//void dibujarMenuPausa(tJuego* juego, tBotones* reanudar, tBotones* volverAInicio)
//{
//    SDL_Rect subventana = {25, 25, 150, 150};
//    SDL_Color colorTexto = {255, 255, 255, 240}; //blanco
//
//    SDL_SetRenderDrawColor(juego->render, 222, 184, 135, 200);
//    SDL_RenderDrawRect(juego->render, &subventana);
//    SDL_RenderFillRect(juego->render, &subventana);
//    SDL_RenderPresent(juego->render);
//
//    reanudar->fuente = TTF_OpenFont("assets/vcr_osd_mono/VCR_OSD_MONO_1.001.ttf", 16);
//    if (!reanudar->fuente)
//        printf("OpenFont: %s\n", TTF_GetError());
//
//    TTF_SizeUTF8(reanudar->fuente, "Reanudar", &(reanudar->destino.w), &(reanudar->destino.h));
//
//    reanudar->destino.x = 75;
//    reanudar->destino.y = 75;
//
//    reanudar->surface = TTF_RenderUTF8_Blended(reanudar->fuente, "Reanudar", colorTexto);
//    reanudar->textura = SDL_CreateTextureFromSurface(juego->render, reanudar->surface);
//    SDL_FreeSurface(reanudar->surface);
//
//    SDL_RenderCopy(juego->render, reanudar->textura, NULL, &(reanudar->destino));
//    SDL_RenderPresent(juego->render);
//
//    SDL_DestroyTexture(reanudar->textura);
//    TTF_CloseFont(reanudar->fuente);
//
//
//    volverAInicio->fuente = TTF_OpenFont("assets/vcr_osd_mono/VCR_OSD_MONO_1.001.ttf", 12);
//    if (!volverAInicio->fuente)
//        printf("OpenFont: %s\n", TTF_GetError());
//
//    TTF_SizeUTF8(volverAInicio->fuente, "Volver al inicio", &(volverAInicio->destino.w), &(volverAInicio->destino.h));
//
//    volverAInicio->destino.x = 75;
//    volverAInicio->destino.y = 100;
//
//    volverAInicio->surface = TTF_RenderUTF8_Blended(volverAInicio->fuente, "Volver al inicio", colorTexto);
//    volverAInicio->textura = SDL_CreateTextureFromSurface(juego->render, volverAInicio->surface);
//    SDL_FreeSurface(volverAInicio->surface);
//
//    SDL_RenderCopy(juego->render, volverAInicio->textura, NULL, &(volverAInicio->destino));
//    SDL_RenderPresent(juego->render);
//
//    SDL_DestroyTexture(volverAInicio->textura);
//    TTF_CloseFont(volverAInicio->fuente);
//}
