#include "sdl.h"
#include "juego.h"
#include "sonidos.h"
#include "dibujos.h"
#include "menus.h"

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


void reproducirNota(int boton, tJuego* juego)
{
    float* pn = juego->tonosBotones;
    Mix_Chunk* tono;

    pn += boton;
    tono = crearTonoPorTimbre((*pn), juego->timbre);
    if(!tono)
        return;

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


    float* pTonos = juego->tonosBotones;

    while(matTonos[i][0] != juego->botones)
        i++;

    for(j=1; j<(MAX_BOTON + 1); j++)
    {
        *pTonos = matTonos[i][j];
        pTonos++;
    }
}

Mix_Chunk* crearTonoBase(float frecuencia, float (*onda)(float))
{
    int total_muestras = FREC_MUESTREO * DURACION_SEG;
    short* buf = (short*)malloc(sizeof(short) * total_muestras);
    if(!buf)
        return NULL;

    for(int i = 0; i < total_muestras; i++)
    {
        float t = (float)i / (float)FREC_MUESTREO;
        float s = onda(2.0f * (float)M_PI * frecuencia * t);
        float v = s * AMPLITUD_TONO;
        if(v > 32767.0f) v = 32767.0f;
        if(v < -32768.0f) v = -32768.0f;
        buf[i] = (short)v;
    }

    Mix_Chunk* ch = (Mix_Chunk*)malloc(sizeof(Mix_Chunk));
    if(!ch)
    {
        free(buf);
        return NULL;
    }

    ch->abuf = (Uint8*)buf;
    ch->alen = (Uint32)(sizeof(short) * total_muestras);
    ch->allocated = 1; //hace que SDL_mixer libere el buffer
    ch->volume = MIX_MAX_VOLUME;

    return ch;
}

float ondaSeno(float x)
{
    return sinf(x);
}

float ondaCuadrada(float x)
{
    if(sinf(x) >= 0.f)
        return 1.f;
    else
        return -1.f;
}

float ondaSierra(float x)
{
    float p = fmodf(x, 2.f * (float)M_PI) / (2.f * (float)M_PI);
    return 2.f * p - 1.f;
}

float ondaTriang(float x)
{
    float p = fmodf(x, 2.f * (float)M_PI) / (2.f * (float)M_PI);
    float v = 1.f - fabsf(2.f * p - 1.f);
    return (v * 2.f) - 1.f;
}

//Wrappers por timbre
Mix_Chunk* crearTonoSeno(float f)
{
    return crearTonoBase(f, ondaSeno);
}

Mix_Chunk* crearTonoCuadrada(float f)
{
    return crearTonoBase(f, ondaCuadrada);
}

Mix_Chunk* crearTonoSierra(float f)
{
    return crearTonoBase(f, ondaSierra);
}

Mix_Chunk* crearTonoTriang(float f)
{
    return crearTonoBase(f, ondaTriang);
}

Mix_Chunk* crearTonoPorTimbre(float f, eTimbre timbre)
{
    switch(timbre)
    {
        case TIMBRE_CUADRADA:
            return crearTonoCuadrada(f);
        case TIMBRE_SIERRA:
            return crearTonoSierra(f);
        case TIMBRE_TRIANG:
            return crearTonoTriang(f);
        case TIMBRE_SENO:
        default:
            return crearTonoSeno(f);
    }
}


void reproducirError(tJuego* juego)
{
    float frecuencia = 110.f;

    Mix_Chunk* tono = crearTonoCuadrada(frecuencia);
    if(!tono)
        return;

    Uint32 muestras_180ms = (Uint32)(FREC_MUESTREO * 0.18f);
    if(muestras_180ms * sizeof(short) < tono->alen)
        tono->alen = muestras_180ms * sizeof(short);

    Mix_PlayChannel(-1, tono, 0);
}
