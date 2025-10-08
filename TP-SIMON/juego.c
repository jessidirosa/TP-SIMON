#include "sdl.h"
#include "juego.h"
#include "sonidos.h"
#include "dibujos.h"

bool puntoDentroCirculo(int x, int y, int cx, int cy, int r)
{
    int dx = x - cx, dy = y - cy;
    return dx*dx + dy*dy <= r*r;
}

void inicializarPartida(tPartida* partida)
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

    crearSecuenciaAleatoria(partida); //despues va a diferir dependiendo los modos de juego elegidos en el menu
}


void iniciarJuego(tPartida* partida, tJuego* juego, int** mat) //comienza la partida
{
    while(finalizarJuego(partida)) //devolvera 0 cuando se presione "Salir" u otra cosa
    {
        inicializarPartida(partida);

        while(partida->estado != GAMEOVER)
        {
            //ver circularidad de secuencia para que sea continua hasta que el jugador pierda (resize de vector)
            secuenciaJuego(partida, juego, mat); //mostrara la secuencia desde sec a psec a medida que avanza. el vector sec ya tendra en numeros la secuencia con el modo y los tonos definidos previamente en config
            SDL_Delay(100);

            if(respuesta(partida, juego, mat) == ERROR) //registrara la respuesta del jugador y la ira guardando en res hasta detectar un error en el patron
                partida->estado = GAMEOVER;

            else
            {
                partida->ranking->score++;
                partida->psec++;
                //disminuirTiempo(partida);
            }
        }
    }
}

int respuesta(tPartida* partida, tJuego* juego, int** mat)
{
    int *p = partida->sec; //aca esta la sec
    SDL_Event evento;

    SDL_PollEvent(&evento);
    if(evento.type == SDL_MOUSEBUTTONDOWN)//guardamos coordenadas de a donde toco para iluminar boton y ver si esta bien o no
    {
        juego->mx = evento.button.x;
        juego->my = evento.button.y;
    }

    while(botonSeleccionar(juego) == (*p) && p < partida->psec)
    {
        SDL_PollEvent(&evento);
        if(evento.type == SDL_MOUSEBUTTONDOWN)//guardamos coordenadas de a donde toco para iluminar boton y ver si esta bien o no
        {
            juego->mx = evento.button.x;
            juego->my = evento.button.y;

            if(botonSeleccionar(juego) > 0)
                iluminarBoton(botonSeleccionar(juego), juego, mat);
        }

        p++;
    }

    if(botonSeleccionar(juego) != (*p))
        return ERROR;
    else
        return CONTINUA;

}

void secuenciaJuego(tPartida* partida, tJuego* juego, int** mat)
{
    int* p = partida->sec;

    while(p < partida->psec)
    {
        iluminarBoton((*p), juego, mat);
        p++;
    }

    partida->psec++;
}

void crearSecuenciaAleatoria(tPartida* partida)
{
    int* fin = partida->sec + MAX; //despues resize
    int* p = partida->sec;

    srand(time(NULL));
    while(p < fin)
    {
        (*p) = rand() % (MAX_BOTON + 1);
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
}

int finalizarJuego(tPartida* partida)
{
    SDL_Event evento;
    SDL_PollEvent(&evento);

    if(evento.type == SDL_MOUSEBUTTONDOWN &&  puntoDentroCirculo(evento.button.x, evento.button.y, CENTRO_PLAY_X, CENTRO_PLAY_Y, R_INT)) //Cambiar las macros por coord de boton QUIT
        return GAMEOVER;
    else
        return CONTINUA;
}
