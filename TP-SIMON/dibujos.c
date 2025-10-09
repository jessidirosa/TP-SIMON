#include "dibujos.h"
#include "sdl.h"
#include "juego.h"
#include "sonidos.h"

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
    {0,   0,  0,  0} // T[9] - Transparente
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

    SDL_RenderPresent(juego->render);
    SDL_Delay(100);
}

void apagarBoton(int boton, tJuego* juego, int** mat)
{
    int i, j;

    SDL_Color color = colores[boton]; // Tomamos el color original del botón

    // Asegurar opacidad total (si querés evitar efectos raros)
    SDL_SetRenderDrawBlendMode(juego->render, SDL_BLENDMODE_NONE);

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

    SDL_RenderPresent(juego->render);
    SDL_Delay(100);
}

void dibujarBordes(tJuego* juego)
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

