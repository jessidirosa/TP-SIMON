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
    partida->sec = malloc(sizeof(int) * MAX);
    if(!partida->sec)
    {
        partida->sec = NULL;
        return;
    }

    partida->psec = partida->sec + 1;
    partida->ranking.score = 0;
    partida->estado = CONTINUA;
    partida->acoteDuracion = TIEMPO_ACOTADO_POR_NOTA;
    asignarSonidos(juego);

    if(partida->modoJuego == SCHONBERG)
        crearSecuenciaAleatoria(partida, juego);

    if(partida->modoJuego == MOZART)
    {
        elegirMelodia(juego, partida);
        crearSecuenciaPorArchivo(partida, juego, partida->archivoMelodia);
    }

    if(partida->modoJuego == DESAFIO)
        crearSecuenciaPorArchivo(partida, juego, partida->archivoMelodia);

}

float duracionNota(tPartida* partida)
{
    return partida->tiempoNota * (1 - partida->acoteDuracion);
}

void iniciarJuego(tPartida* partida, tJuego* juego, int** mat) //comienza la partida
{
    int x = 1;
    SDL_Color colorX = {255, 0, 0, 200};
    SDL_Color colorTotem = {255, 255, 0, 255};
    tBotones volver;
    tBotones totems;
    int vidas = CANT_VIDAS;
    int respuestausuario;

    inicializarPartida(partida, juego);
    dibujar(juego, mat);
    dibujarBordes(juego);
    dibujarBotonCentro(juego, &volver, "Volver");

    dibujarBotones(&totems, "TTT", juego, POS_VIDAS, 170, "fnt/minedings.ttf", 14, colorTotem);
    while(partida->estado != GAMEOVER && partida->estado != SALIR && partida->estado != ERROR_MOZART)
    {
        secuenciaJuego(partida, juego, mat);
        respuestausuario=respuesta(partida,juego,mat,&volver);
        if(respuestausuario== ERROR && vidas==0)
            partida->estado = GAMEOVER;
        else if(respuestausuario == ERROR && vidas>0) {
            vidas--;
//            SDL_SetRenderDrawColor(juego->render, 0, 0, 0, 150);
//            SDL_RenderPresent(juego->render);
            SDL_Delay(10);
            dibujarBotones(&totems, "X", juego, POS_VIDAS + x, 170, "fnt/porkys_.ttf", 14, colorX);
            x += 15;
        }
        else if(respuestausuario == SALIR)
            partida->estado = SALIR;

        else
        {
            partida->ranking.score++;
            partida->psec++;
            partida->tiempoNota = duracionNota(partida);
        }
    }

    insertarArchivoRankingSinDup(partida);
    ///funcion: ordenar archivo ranking

    if(partida->estado == GAMEOVER)
        juego->instancia = RANKING; ///instancia RANKING: muestra ranking y boton reiniciar
    else if(partida->estado == SALIR)
        juego->instancia = MENU;
    else if(partida->estado == ERROR_MOZART)
        juego->instancia = ERROR_MOZART;

}

int respuesta(tPartida* partida, tJuego* juego, int** mat, tBotones* volver)
{
    int* p = partida->sec;
    SDL_Event evento;

    while (p < partida->psec)
    {
        SDL_WaitEvent(&evento);

        if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, volver->destino.x, volver->destino.y, volver->destino.w, volver->destino.h))
            return SALIR;

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
                    reproducirError(juego);
                    SDL_Delay(200);
                    return ERROR;
                }

                p++;
            }
        }
        else if (evento.type == SDL_QUIT)
            return SALIR;
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

bool insertarArchivoRankingSinDup(tPartida* partida)
{
    tRanking rankArch;
    int encontrado = 0;
    FILE* pf = fopen("Rankings.bin", "r+b");
    if(!pf)
    {
        pf = fopen("Rankings.bin", "w+b");   //crea si no existe
        if(!pf)
            return false;
    }

    while(!encontrado && fread(&rankArch, sizeof(tRanking), 1, pf))
    {
        if(!strncmp(partida->ranking.jugador, rankArch.jugador, sizeof(rankArch.jugador)))
        {
            fseek(pf, -(long)sizeof(tRanking), SEEK_CUR);
            encontrado = 1;
        }
    }

    fwrite(&(partida->ranking), sizeof(tRanking), 1, pf);
    fclose(pf);

    return true; //todo ok
}

bool mostrarArchivo(char* nombre)
{
    tRanking rankArch;
    FILE* pf = fopen(nombre, "rb");
    if(!pf)
        return false;

    printf("JUGADOR\tPUNTAJE\n");
    fread(&rankArch, sizeof(tRanking), 1, pf);
    while(!feof(pf))
    {
        printf("%s\t%d\n", rankArch.jugador, rankArch.score);
        fread(&rankArch, sizeof(tRanking), 1, pf);
    }

    fclose(pf);
    return true;
}

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
    {
        free(centro);
        return ERROR; //ver
    }


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

    int num = menor->num;
    free(centro);
    return num;
}

bool esMenor(double actual, double nuevo)
{
    return (nuevo - actual) > 0 ? false : true;
}

void liberarMemoria(tJuego* juego, int** mat, tPartida* partida, tRanking* ranking)
{
    int i;
    for (i = 0; i < MAT_FILA; i++)
        free(mat[i]);
    free(mat);

     if (partida->sec)
        free(partida->sec);

    free(juego->tonosBotones);
    free(ranking);
}

void configPorDefecto(tJuego* juego, tPartida* partida)
{
    partida->modoJuego = SCHONBERG;
    partida->tiempoNota = TIEMPO_INICIAL;
    juego->botones = 4;
    juego->timbre = TIMBRE_SENO;
}

void* crearVector(size_t tam, int ce)
{
    void* vec = calloc(ce, tam);
    if(!vec)
        return NULL;
    return vec;
}

bool redimensionar(void** pv, int ce, size_t tam, int* cap)
{
    int nuevaCap = (*cap > 0) ? (*cap * 2) : 1;
    void* tmp = realloc(*pv, (size_t)nuevaCap * tam);
    if (!tmp)
        return false;

    *pv = tmp;
    *cap = nuevaCap;

    return true;
}

int ordenarArchivo(char* nombre, tRanking** vRank, int* ce, int* maxTam, tJuego* juego)
{
    tRanking* r = *vRank;
    tRanking* fin;
    FILE* pf;

    pf = fopen(nombre, "rb");
    if(!pf)
    {
        pf = fopen(nombre, "w+b");   //crea si no existe
        if(!pf)
            return false;
    }

    while(!feof(pf))
    {
        while((*ce) < (*maxTam) && fread(r, sizeof(tRanking), 1, pf))
        {
            r++;
            (*ce)++;
        }

        if((*ce) == (*maxTam))
        {
            if(!redimensionar((void**)vRank, (*ce), sizeof(tRanking), maxTam))
                return false; //no pudo agrandarse el vector
            r = (*vRank) + (*ce);
        }
    }

    fclose(pf);

    r = (*vRank);
    if((*ce) > 0 && juego->instancia == RANKING)
    {
        qsort(r, (*ce), sizeof(tRanking), compararRankings);
        juego->instancia = RANK_ORDENADO;
    }

    else if((*ce) == 0)
        return VACIO;

    pf = fopen(nombre, "wb");
    if(!pf)
        return false;

    fin = r + (*ce);
    while(r < fin)
    {
        fwrite(r, sizeof(tRanking), 1, pf);
        r++;
    }

    fclose(pf);
    return true;
}

int compararRankings(const void* r1, const void* r2)
{
    tRanking* rank1 = (tRanking*)r1;
    tRanking* rank2 = (tRanking*)r2;

    return (rank2->score - rank1->score);
}

bool crearSecuenciaPorArchivo(tPartida* partida, tJuego* juego, char* nombreArch) // modo mozart / desafio
{
    int* fin = partida->sec + MAX;
    int* p   = partida->sec;
    char buffer[MAX];
    char* n;
    int boton;

    FILE* pf = fopen(nombreArch, "rt");
    if(!pf) {
        juego->instancia = ERROR_MOZART;
        partida->estado = ERROR_MOZART;
        printf("no se pudo abrir archivo de melodia");
        return false;
    }

    if(!fgets(buffer, sizeof(buffer), pf)) {
        fclose(pf);
        return false;
    }
    fclose(pf);

    n = buffer;
    while(p < fin)
    {
        while((*n) && p < fin)
        {
            if((*n) >= '0' && (*n) <= '9')
            {
                boton = (*n) - '0';
                if(boton < juego->botones)
                {
                    (*p) = boton;
                    p++;
                }
            }

            n++;
        }
        n = buffer;
    }

    return true;
}


void elegirMelodia(tJuego* juego, tPartida* partida)
{
    switch (juego->botones) {
    case 3:
        strcpy(partida->archivoMelodia, ARCH_MELODIA_3);
        break;
    case 4:
        strcpy(partida->archivoMelodia, ARCH_MELODIA_4);
        break;
    case 5:
        strcpy(partida->archivoMelodia, ARCH_MELODIA_5);
        break;
    case 6:
        strcpy(partida->archivoMelodia, ARCH_MELODIA_6);
        break;
    case 7:
        strcpy(partida->archivoMelodia, ARCH_MELODIA_7);
        break;
    case 8:
        strcpy(partida->archivoMelodia, ARCH_MELODIA_8);
        break;
    }
}

//desde el menu de config->modos
void ingresarSecuenciaPorBotones(tJuego* juego, int** mat, tPartida* partida) //modo desafio
{
    tBotones grabar;
    SDL_Event evento;

    dibujar(juego, mat);
    dibujarBordes(juego);
    dibujarBotonCentro(juego, &grabar, "Grabar");

    SDL_PollEvent(&evento);
    if(evento.type == SDL_MOUSEBUTTONDOWN && puntoEnRectangulo(evento.button.x, evento.button.y, grabar.destino.x, grabar.destino.y, grabar.destino.w, grabar.destino.h))
    {
        juego->instancia = GRABANDO;
    }

    if (evento.type == SDL_QUIT)
            juego->instancia = SALIR;
}

void grabarMelodia(tJuego* juego, int** mat, tPartida* partida)
{
    tBotones finalizar;
    char nombre[] = "modo_desafio.txt";
    int grabacion[MAX];
    int ce = 0;
    int* pg = grabacion;
    SDL_Event ev;
    bool grabando = true;

    asignarSonidos(juego);
    dibujar(juego, mat);
    dibujarBordes(juego);
    dibujarBotonCentro(juego, &finalizar, "Finalizar");

    while (grabando)
    {
        if (!SDL_WaitEvent(&ev))
            continue;

        switch (ev.type)
        {
        case SDL_QUIT:
            juego->instancia = SALIR;
            return;

        case SDL_MOUSEBUTTONDOWN:
            if (puntoEnRectangulo(ev.button.x, ev.button.y, finalizar.destino.x, finalizar.destino.y, finalizar.destino.w, finalizar.destino.h))
            {
                if (ce > 0)
                {
                    cargarArchivoModoDesafio(nombre, grabacion, ce);
                    strcpy(partida->archivoMelodia, nombre);
                    juego->instancia = MENU;
                }
                else
                    juego->instancia = MODO_DESAFIO; // nada grabado

                grabando = false;
                break;
            }

            if (puntoDentroCirculo(ev.button.x, ev.button.y, CENTRO_PLAY_X, CENTRO_PLAY_Y, R_EXT))
            {
                juego->mx = ev.button.x;
                juego->my = ev.button.y;

                int boton = botonSeleccionar(juego);
                if (boton >= 0 && ce < MAX)
                {
                    iluminarBoton(boton, juego, mat);
                    SDL_Delay(partida->tiempoNota);
                    apagarBoton(boton, juego, mat);
                    (*pg) = boton;
                    pg++;
                    ce++;
                }
            }
            break;
        }
    }

    if (ev.type == SDL_QUIT)
            juego->instancia = SALIR;
}


bool cargarArchivoModoDesafio(char* nombre, int* grabacion, int ce)
{
    int* pg = grabacion;
    int* fin = grabacion + ce;

    FILE* pf = fopen(nombre, "wt");
    if(!pf)
    {
        printf("no se pudo abrir modo desafio");
        return false;
    }

    while(pg < fin)
    {
        fprintf(pf, "%d", (*pg));
        pg++;
    }

    fprintf(pf, "\n");
    fclose(pf);
    return true;
}
