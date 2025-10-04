#include "sdl.h"
#include "juego.h"
#include "sonidos.h"
#include "dibujos.h"



void inicializarPartida(tPartida* juego)
{
    //el nombre del jugador ya llega en la struct
    juego->psec = juego->sec;
    juego->ranking->score = 0;
    juego->estado = CONTINUA;
    juego->tiempoNota = TIEMPO_INICIAL; //puede que ya llegue a la funcion con esto ya asignado porque se cambia en la configuracion
    juego->acoteDuracion = TIEMPO_ACOTADO_POR_NOTA;
}
//
//void iniciarJuego(tPartida* juego) //comienza la partida
//{
//    while(!finalizarJuego(juego)) //devolvera 1 cuando se presione "Salir" u otra cosa
//    {
//        inicializarPartida(juego);
//
//        while(juego->estado != GAMEOVER)
//        {
//            //ver circularidad de secuencia para que sea continua hasta que el jugador pierda (resize de vector)
//            secuencia(juego); //mostrara la secuencia desde sec a psec a medida que avanza. el vector sec ya tendra en numeros la secuencia con el modo y los tonos definidos previamente en config
//
//            if(respuesta(juego) == ERROR) //registrara la respuesta del jugador y la ira guardando en res hasta detectar un error en el patron
//                juego->estado = GAMEOVER;
//
//            else
//            {
//                juego->ranking->score++;
//                juego->psec++;
//                disminuirTiempo(juego);
//            }
//        }
//    }
//}

//hacer funcion de crearArchivoRanking y ordenarArchivo
bool insertarArchivoRankingSinDup(tPartida* juego)
{
    FILE* pf = fopen("Rankings.bin", "r+b");
    tRanking rankArch;

    if(!pf)
        return false; //no se pudo abrir

    fread(&rankArch, sizeof(tRanking), 1, pf);
    while(!feof(pf) && strcmp(juego->ranking->jugador,rankArch.jugador))
        fread(&rankArch, sizeof(tRanking), 1, pf);

    if(!strcmp(juego->ranking->jugador,rankArch.jugador))
        fseek(pf, -1L, SEEK_CUR);

    fwrite(juego->ranking, sizeof(tRanking), 1, pf);
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

    return menor->num + 1;
}

bool esMenor(double actual, double nuevo)
{
    return (nuevo - actual) > 0 ? false : true;
}
