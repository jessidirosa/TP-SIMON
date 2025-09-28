#include "dibujos.h"
#include "sdl.h"
#include "juego.h"
#include "sonidos.h"

///del ejemplo de proyecto que nos dieron
Mix_Chunk* cargarSonido(const char *ruta)
{
    Mix_Chunk *sonido;

    sonido = Mix_LoadWAV(ruta);
    if (!sonido)
    {
        printf("Error cargando sonido \"%s:\" %s\n", ruta, Mix_GetError());
    }

    return sonido;
}

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
