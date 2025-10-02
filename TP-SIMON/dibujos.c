#include "dibujos.h"
#include "sdl.h"
#include "juego.h"
#include "sonidos.h"

SDL_Color colores[] =
{
    {255, 255, 0,   255}, // AM[0] - Amarillo
    {0,   150, 60,  255}, // V[1] - Verde
    {255,   0,  0,  255}, // R[2] - Rojo
    {0,   0,  255,  255}, // AZ[3] - Azul
    {128,   0,  255,  255}, // VI[4] - Violeta
    {255,   192,  203,  255}, // RO[5] - Rosa
    {255,   165,  0,  255}, // NA[6] - Naranja
    {0,   0,   0,   255}, // N[7] - Negro
    {255, 255, 255, 255}, // BL[8] - Blanco
    {0,   0,  0,  0} // T[9] - Transparente
};

//no funciona. probar
void dibujar(tJuego* juego)
{
    int mat[MAT_FILA][MAT_COL];

    int i;
    int j;

    int origenX = (ANCHO_VENTANA - MAT_COL * TAM_PIXEL) / 2;
    int origenY = (ALTO_VENTANA  - MAT_FILA * TAM_PIXEL) / 2;

    int cx = ((MAT_COL-1)/2);
    int cy = ((MAT_FILA-1)/2);

    //ponemos el centro geometrico en el centro de la matriz
    double dx;
    double dy;
    double dist;
    double ang;
    int sector;

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
                    ang += 2 * M_PI; //una vuelta mas para que quede positivo

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
                SDL_Rect pixel = {origenX + TAM_PIXEL*j, origenY + TAM_PIXEL*i, TAM_PIXEL, TAM_PIXEL};
                SDL_RenderFillRect(juego->render, &pixel);
            }

        }
    }
}



