#include "sdl.h"
#include "juego.h"
#include "sonidos.h"
#include "dibujos.h"
#include "menus.h"

bool puntoDentroCirculo(int x, int y, int cx, int cy, int r)
{
    int dx = x - cx, dy = y - cy;
    return dx*dx + dy*dy <= r*r;
}

bool puntoEnRectangulo(int x, int y, int xRect, int yRect, int anchoRect, int altoRect)
{
    return (x >= xRect && x <= xRect + anchoRect) && (y >= yRect && y <= yRect + altoRect);
}

void inicializarPartida(tPartida* partida, tJuego* juego)
{
    //el nombre del jugador ya llega en la struct

    partida->sec = malloc(sizeof(int) * MAX);
    if(!partida->sec)
        return;

    partida->psec = partida->sec + 1;
    partida->ranking->score = 0;
    partida->estado = CONTINUA;
    partida->tiempoNota = TIEMPO_INICIAL; //puede que ya llegue a la funcion con esto ya asignado porque se cambia en la configuracion
    partida->acoteDuracion = TIEMPO_ACOTADO_POR_NOTA;
    asignarSonidos(juego);

    crearSecuenciaAleatoria(partida, juego); //despues va a diferir dependiendo los modos de juego elegidos en el menu. se carga en config
}

float duracionNota(tPartida* partida)
{
    return partida->tiempoNota * (1 - partida->acoteDuracion);
}

void iniciarJuego(tPartida* partida, tJuego* juego, int** mat) //comienza la partida
{
    tBotones volver;
    inicializarPartida(partida, juego);
    dibujar(juego, mat);
    dibujarBordes(juego);
    dibujarBotonCentro(juego, &volver, "Volver");

    while(partida->estado != GAMEOVER && finalizarJuego(partida))
    {
        //ver circularidad de secuencia para que sea continua hasta que el jugador pierda (resize de vector)
        secuenciaJuego(partida, juego, mat); //mostrara la secuencia desde sec a psec a medida que avanza. el vector sec ya tendra en numeros la secuencia con el modo y los tonos definidos previamente en config

        if(respuesta(partida, juego, mat, &volver) == ERROR)
            partida->estado = GAMEOVER;
        else
        {
            partida->ranking->score++;
            partida->psec++;
            partida->tiempoNota = duracionNota(partida);
        }
    }

    //poner de reiniciarlo o algo
    juego->instancia = MENU;
}

int respuesta(tPartida* partida, tJuego* juego, int** mat, tBotones* volver)
{
    int* p = partida->sec;
    SDL_Event evento;

    while (p < partida->psec)
    {
        SDL_WaitEvent(&evento);

        if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, volver->destino.x, volver->destino.y, volver->destino.w, volver->destino.h))
            return ERROR;

        if (evento.type == SDL_MOUSEBUTTONDOWN && puntoDentroCirculo(evento.button.x, evento.button.y, CENTRO_PLAY_X, CENTRO_PLAY_Y, R_EXT))
        {
            juego->mx = evento.button.x;
            juego->my = evento.button.y;

            int boton = botonSeleccionar(juego);
            printf("Clic en (%d,%d)\nBoton detectado: %d\n", evento.button.x, evento.button.y, boton);

            if (boton >= 0)
            {
                // Ilumina el botón clickeado
                iluminarBoton(boton, juego, mat);
                SDL_RenderPresent(juego->render);
                SDL_Delay(partida->tiempoNota);
                apagarBoton(boton, juego, mat);

                if (boton != *p)
                {
                    printf("Error! Esperaba %d pero se toco %d\n", *p, boton);
                    return ERROR;
                }

                p++;
            }
        }
        else if (evento.type == SDL_QUIT)
            return ERROR;
    }

    return CONTINUA;
}

void secuenciaJuego(tPartida* partida, tJuego* juego, int** mat)
{
    int* p = partida->sec;

    while(p < partida->psec)
    {
        iluminarBoton((*p), juego, mat);
        SDL_Delay(partida->tiempoNota);
        apagarBoton((*p), juego, mat);

        p++;
    }
}

void crearSecuenciaAleatoria(tPartida* partida, tJuego* juego) //modo schonberg
{
    int* fin = partida->sec + MAX; //despues resize
    int* p = partida->sec;

    srand(time(NULL));
    while(p < fin)
    {
        (*p) = rand() % juego->botones;
        p++;
    }
}

//hacer funcion de crearArchivoRanking y ordenarArchivo
bool insertarArchivoRankingSinDup(tPartida* partida)
{
    FILE* pf = fopen("Rankings.bin", "r+b");
    tRanking rankArch;

    if(!pf)
        return false; //no se pudo abrir

    fread(&rankArch, sizeof(tRanking), 1, pf);
    while(!feof(pf) && strcmp(partida->ranking->jugador,rankArch.jugador))
        fread(&rankArch, sizeof(tRanking), 1, pf);

    if(!strcmp(partida->ranking->jugador,rankArch.jugador))
        fseek(pf, -1L, SEEK_CUR);

    fwrite(partida->ranking, sizeof(tRanking), 1, pf);
    fclose(pf);

    return true; //todo ok
}

//llamada por funcion respuesta
int botonSeleccionar(tJuego* juego)
{
    double cx = ((MAT_COL-1)/2);
    double cy = ((MAT_FILA-1)/2);
    double r_medio = (R_EXT - R_INT) / 2;
    double ang_boton = (2 * M_PI)/juego->botones;
    double ang_centro;
    double dx, dy;

    int i;

    tBoton* centro = malloc(sizeof(tBoton) * juego->botones);
    if(!centro)
        return ERROR; //ver

    double dist = sqrt((cx - juego->mx) * (cx - juego->mx) + (cy - juego->my) * (cy - juego->my));
    if(dist < R_INT || dist > R_EXT)
        return ERROR;

    tBoton* boton = centro;
    tBoton* menor = centro;
    tBoton* fin = centro + juego->botones; //primer invalido

    for(i=0; i<juego->botones; i++)
    {
        boton->num = i; //coincidira con el color (macro)
        ang_centro = (i + 0.5) * ang_boton;
        boton->x = cx + (r_medio * cos(ang_centro));
        boton->y = cy + (r_medio * sin(ang_centro));
        dx = juego->mx - boton->x;
        dy = juego->my - boton->y;
        boton->distClick = sqrt(dx * dx + dy * dy);
        boton++;
    }

    boton = centro + 1;
    while(boton < fin)
    {
        if(esMenor(menor->distClick, boton->distClick))
            menor = boton;

        boton++;
    }

    return menor->num;
}

bool esMenor(double actual, double nuevo)
{
    return (nuevo - actual) > 0 ? false : true;
}

void liberarMemoria(tJuego* juego, int** mat, tPartida* partida)
{
    int i;

    free(partida->sec);
    for(i=MAT_FILA-1; i>=0; i--)
    {
        free((*mat));
        (*mat)++;
    }

    free(mat);
    free(juego->tonosBotones);
}

int finalizarJuego(tPartida* partida)
{
    SDL_Event evento;
    SDL_PollEvent(&evento);

    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, QUIT_BOTON_X, QUIT_BOTON_Y, ANCHO_BOTON, ALTO_BOTON))
        return GAMEOVER;
    else
        return CONTINUA;
}
