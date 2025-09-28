#include "dibujos.h"
#include "sdl.h"
#include "juego.h"
#include "sonidos.h"

SDL_Color colores[] =
{
    {0,   0,   0,   255}, // N[0] - Negro
    {255, 255, 0,   255}, // AM[1] - Amarillo
    {255, 255, 255, 255}, // BL[2] - Blanco
    {0,   150, 60,  255}, // V[3] - Verde
    {255,   0,  0,  255}, // R[4] - Rojo
    {0,   0,  255,  255}, // AZ[9] - Azul
    {128,   0,  255,  255}, // VI[6] - Violeta
    {255,   192,  203,  255}, // RO[7] - Rosa
    {255,   165,  0,  255}, // NA[8] - Naranja
    {0,   0,  0,  0}, // T[5] - Transparente
};
