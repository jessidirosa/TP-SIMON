#include "sdl.h"
#include "juego.h"
#include "sonidos.h"
#include "dibujos.h"
#include "menus.h"

///del ejemplo de proyecto que nos dieron
//Mix_Chunk* cargarSonido(const char *ruta)
//{
//    Mix_Chunk *sonido;
//
//    sonido = Mix_LoadWAV(ruta);
//    if (!sonido)
//    {
//        printf("Error cargando sonido \"%s:\" %s\n", ruta, Mix_GetError());
//    }
//
//    return sonido;
//}

Mix_Chunk* crearTono(float frecuencia)
{
    Mix_Chunk* tono;

    short *buffer = malloc(sizeof(short) * FREC_MUESTREO);
    if (!buffer)
    {
        printf("[Error] No se pudo reservar memoria para el tono\n");
        return NULL;
    }

    for (int i = 0; i < FREC_MUESTREO; i++)
    {
        buffer[i] = AMPLITUD_TONO * sin(2.0 * M_PI * frecuencia * i / FREC_MUESTREO);
    }

    tono = malloc(sizeof(Mix_Chunk));
    if (!tono)
    {
        printf("[Error] No se pudo reservar memoria para el tono\n");
        free(buffer);
        return NULL;
    }

    tono->alen = FREC_MUESTREO * sizeof(short); // cantidad de bytes del buffer de audio
    tono->abuf = (unsigned char*)buffer;
    tono->volume = MIX_MAX_VOLUME;

    return tono;
}

void destruirTono(Mix_Chunk *tono)
{
    free(tono->abuf);
    free(tono);
}

///hasta aca


void reproducirNota(int boton, tJuego* juego) //faltaria agregar cuando se equivoca un sonido diferente
{
    int* pn = juego->tonosBotones;
    Mix_Chunk* tono;

    pn += boton; //vamos al boton corresp
    tono = crearTono((*pn));
    Mix_PlayChannel(-1, tono, 0);

}

void asignarSonidos(tJuego* juego) //carga el vector de tonos de juego->tonosBotones con las frec correspondientes (cargadas en matriz) dependiendo la cant de botones
{
    int j, i = 0;
    float matTonos[][MAX_BOTON + 1] =  { {3, 261.63, 329.63, 392.00,  0,      0,      0,      0,      0},
                                         {4, 261.63, 329.63, 392.00,  440.00, 0,      0,      0,      0},
                                         {5, 261.63, 293.66, 329.63,  392.00, 440.00, 0,      0,      0},
                                         {6, 261.63, 293.66, 329.63,  369.99, 415.30, 466.16, 0,      0},
                                         {7, 261.63, 293.66, 311.13,  349.23, 392.00, 415.30, 493.88, 0},
                                         {8, 261.63, 293.66, 329.63,  349.23, 392.00, 440.00, 493.88, 523.25}};


    int* pTonos = juego->tonosBotones;

    while(matTonos[i][0] != juego->botones)
        i++;

    for(j=1; j<(MAX_BOTON + 1); j++)
    {
        *pTonos = matTonos[i][j];
        pTonos++;
    }
}
