#include "juego.h"

void inicializarPartida(tPartida* juego)
{
    //el nombre del jugador ya llega en la struct
    juego->psec = juego->sec;
    juego->ranking->score = 0;
    juego->estado = CONTINUA;
    juego->tiempoNota = TIEMPO_INICIAL; //puede que ya llegue a la funcion con esto ya asignado porque se cambia en la configuracion
    juego->acoteDuracion = TIEMPO_ACOTADO_POR_NOTA;
}

void iniciarJuego(tPartida* juego) //comienza la partida
{
    while(!finalizarJuego(juego)) //devolvera 1 cuando se presione "Salir" u otra cosa
    {
        inicializarPartida(juego);

        while(juego->estado != GAMEOVER)
        {
            secuencia(juego); //mostrara la secuencia desde sec a psec a medida que avanza. el vector sec ya tendra en numeros la secuencia con el modo y los tonos definidos previamente en config

            if(respuesta(juego) == ERROR) //registrara la respuesta del jugador y la ira guardando en res hasta detectar un error en el patron
                juego->estado = GAMEOVER;

            else
            {
                juego->ranking->score++;
                juego->psec++;
                disminuirTiempo(juego);
            }
        }
    }


}

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


