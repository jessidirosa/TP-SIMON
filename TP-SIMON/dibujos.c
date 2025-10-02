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
    {178, 255, 255, 255}, // CE[7] - Celeste
    {255, 255, 255, 255}, // BL[8] - Blanco
    {0,   0,  0,  0} // T[9] - Transparente
};

void dibujar(tJuego* juego)
{
    int **mat = crearMatriz(MAT_FILA, MAT_COL, sizeof(int));
    if(!mat)
        return;

    int i;
    int j;

    double cx = ((MAT_COL-1)/2);
    double cy = ((MAT_FILA-1)/2);

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

