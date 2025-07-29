#ifndef _CACHE_HH_
#define _CACHE_HH_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdarg.h>
#include "stdio.h"

typedef struct linea {
    uint32_t tag;
    uint8_t valido;
    uint8_t dirty;
    uint32_t ultimoAcceso;
    uint32_t tagAnterior; //no lo estoy actualizando nunca
} linea_t;

typedef struct s_set {
    linea_t* lineas;
    uint32_t cantidadLineas;
    uint32_t lecturas;
    uint32_t escrituras;
    uint32_t missesLectura;
    uint32_t missesEscritura;
    uint32_t dirtyReadMisses;
    uint32_t dirtyWriteMisses;
    uint32_t bytesLeidos;
    uint32_t bytesEscritos;
    uint32_t ciclosLectura;
    uint32_t ciclosEscritura;
} set_t;

typedef struct s_cache {
    set_t** sets;
    int cantidadSets;
    int cantidadVias;
    int tamañoBloque;
    uint8_t bitsOffset;
    uint8_t bitsSet;
    uint8_t bitsTag;
    int tamañoCache;

} cache_t;

typedef struct s_datosVerboso {
    uint32_t numOperacion;
    char* identificador;    //Se encarga procesarInstruccion
    uint32_t cacheIndex;     //Indice en hexa
    uint32_t cacheTag;       //Tag en hexa
    uint32_t cacheLine;      //Numero de linea (0, E]
    uint32_t lineTag;       //Tag presente anteriormente
    uint8_t validBit;       //.Valido del estado anterior
    uint8_t dirtyBit;       //.Dirty del estado anterior
    uint32_t lastUsed;
} datosVerboso_t;


cache_t* newCache(uint32_t E, uint32_t S, uint32_t C);
set_t* newSet(uint32_t E);
linea_t newLinea();
datosVerboso_t* newDatosVerboso();

void procesarInstruccion(char* tipo, uint32_t direccion, cache_t* cache, datosVerboso_t* datosVerboso);

void lectura(cache_t* cache, uint32_t set, uint32_t tagNuevo, datosVerboso_t* datosVerboso);

void escritura(cache_t* cache, uint32_t set, uint32_t tagNuevo, datosVerboso_t* datosVerboso);

int buscarTag(linea_t* lineas, uint32_t tagBuscado, uint32_t cantVias);

void imprimirEstadísticas(cache_t* cache);

void liberarCache(cache_t *c);

#endif