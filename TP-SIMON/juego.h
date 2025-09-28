#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 1024

typedef struct {
    int rojo; //1
    int verde; //2
    int azul; //3
    int amarillo; //4
    int violeta; //5
    int naranja; //6
    int rosa; //7
    int celeste; //8

}tBoton;

typedef struct
{
    int secuencia[MAX];
    int* psec;
    int respuesta;
    bool estado;

} tPartida;


#endif // JUEGO_H_INCLUDED
