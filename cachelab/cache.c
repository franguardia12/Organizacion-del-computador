#include "cache.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>

uint8_t calculoCantidadBits(uint32_t tamaño) {
    return (log(tamaño)) / (log(2));
}

int buscarTag(linea_t* lineas, uint32_t tagBuscado, uint32_t cantVias) {
    for (uint32_t i = 0; i < cantVias; i++) {
        if (lineas[i].tag == tagBuscado) {
            return i;
        }
    }
    return -1;
}

cache_t* newCache(uint32_t E, uint32_t S, uint32_t C) {

    cache_t* cache = (cache_t*) malloc(sizeof(cache_t));

    set_t** arregloSets = malloc(8 * S);

    for(uint32_t i = 0; i < S; i++) {
        arregloSets[i] = newSet(E);
    }
    cache->sets = arregloSets;
    cache->cantidadSets = S;
    cache->cantidadVias = E;
    cache->tamañoBloque = C / (E * S);
    cache->tamañoCache = C;

    uint8_t bitsOffset = calculoCantidadBits(cache->tamañoBloque);
    cache->bitsOffset = bitsOffset;

    uint8_t bitsSet = calculoCantidadBits(S);
    cache->bitsSet = bitsSet;

    cache->bitsTag = 32 - bitsOffset - bitsSet;
    return cache;
}

set_t* newSet(uint32_t E) {
    set_t* set = (set_t*) malloc(sizeof(set_t));
    linea_t* arregloLineas = malloc(sizeof(linea_t) * E);

    for (uint32_t i = 0; i < E; i++) {
        arregloLineas[i] = newLinea();
    }

    set->lineas = arregloLineas;
    set->cantidadLineas = E;
    set->lecturas = 0;
    set->escrituras = 0;
    set->missesLectura = 0;
    set->missesEscritura = 0;
    set->dirtyReadMisses = 0;
    set->dirtyWriteMisses = 0;
    set->bytesLeidos = 0;
    set->bytesEscritos = 0;
    set->ciclosLectura = 0;
    set->ciclosEscritura = 0;
    return set;
}

linea_t newLinea() {
    linea_t linea;
    linea.tag = 0;
    linea.valido = 0;
    linea.dirty = 0;
    linea.ultimoAcceso = 0;
    linea.tagAnterior = 0;
    return linea;
}

datosVerboso_t* newDatosVerboso() {
    datosVerboso_t* datos = (datosVerboso_t*) malloc(sizeof(datosVerboso_t));

    datos->numOperacion = 0;
    datos->identificador = "";
    datos->cacheIndex = 0;
    datos->cacheTag = 0;
    datos->cacheLine = 0;
    datos->validBit = 0;
    datos->dirtyBit = 0;
    datos->lastUsed = 0;
    return datos;
}
  
//función que imprima todas las estadísticas
void imprimirEstadísticas(cache_t* cache) {
    int tamañoKB = cache->tamañoCache / 1024;
    int totalLecturas = 0;
    int totalEscrituras = 0; 
    int totalRMiss = 0;
    int totalWMiss = 0;
    int totalDirtyRMiss = 0;
    int totalDirtyWMiss = 0;
    int totalBytesLeidos = 0;
    int totalBytesEscritos = 0;
    int totalTiempoLectura = 0;
    int totalTiempoEscritura = 0;

    if (cache->cantidadVias == 1) {
        printf("direct-mapped, %" PRIu32 " sets, size = %" PRIu32 "KB\n", cache->cantidadSets, tamañoKB);
    } else {
        printf("%" PRIu32 "-way, %" PRIu32 " sets, size = %" PRIu32 "KB\n", cache->cantidadVias, cache->cantidadSets, tamañoKB);
    }

    for (int i = 0; i < cache->cantidadSets; i++) {
        set_t *set = cache->sets[i];
        totalEscrituras += set->escrituras;
        totalLecturas += set->lecturas;
        totalRMiss += set->missesLectura;
        totalWMiss += set->missesEscritura;
        totalDirtyRMiss += set->dirtyReadMisses;
        totalDirtyWMiss += set->dirtyWriteMisses;
        totalBytesLeidos += set->bytesLeidos;
        totalBytesEscritos += set->bytesEscritos;
        totalTiempoLectura += set->ciclosLectura;
        totalTiempoEscritura += set->ciclosEscritura;

    }

    int accesosTotales = totalLecturas+totalEscrituras;
    float missesTotales = (float) (totalRMiss+totalWMiss);
    float missRateTotal =  missesTotales/((float) accesosTotales);

    printf("loads %i stores %i total %i\n", totalLecturas, totalEscrituras, totalEscrituras+totalLecturas);
    printf("rmiss %i wmiss %i total %i\n", totalRMiss, totalWMiss, totalRMiss+totalWMiss);
    printf("dirty rmiss %i dirty wmiss %i\n", totalDirtyRMiss, totalDirtyWMiss);
    printf("bytes read %i bytes written %i\n", totalBytesLeidos, totalBytesEscritos);
    printf("read time %i write time %i\n", totalTiempoLectura, totalTiempoEscritura);
    printf("miss rate %f\n", missRateTotal);
}

//Recibe los datos de 1 línea
void procesarInstruccion(char* tipo, uint32_t direccion, cache_t* cache, datosVerboso_t* datosVerboso) {

    uint8_t b = cache->bitsOffset;
    uint8_t s = cache->bitsSet;

    uint32_t set = (direccion & (~(~(0xFFFFFFFF << b) | (0xFFFFFFFF << (s+b))))) >> b;
    uint32_t tag = (direccion & (0xFFFFFFFF << (s+b))) >> (s+b);

    if (!(strcmp(tipo, "R"))) {
        lectura(cache, set, tag, datosVerboso);
    } else {
        escritura(cache, set, tag, datosVerboso);
    }
}

void lectura(cache_t* cache, uint32_t set, uint32_t tagNuevo, datosVerboso_t* datosVerboso) {
    set_t** conjuntoDeSets = cache->sets;

    set_t* setActual = conjuntoDeSets[set];
    uint32_t cantVias = setActual->cantidadLineas;

    linea_t* vias = setActual->lineas;

    datosVerboso->cacheIndex = set;
    datosVerboso->cacheTag = tagNuevo;

    int existente = buscarTag(vias, tagNuevo, cantVias);
    int posViaAUsar = -1;
    linea_t lineaParaActualizar;

    if (existente != -1) {

        lineaParaActualizar = vias[existente];

        setActual->ciclosLectura += 1;

        datosVerboso->identificador = "1";
        datosVerboso->cacheLine = existente;
        datosVerboso->lineTag = lineaParaActualizar.tag;
        datosVerboso->validBit = lineaParaActualizar.valido;
        datosVerboso->dirtyBit = lineaParaActualizar.dirty;

        datosVerboso->lastUsed = lineaParaActualizar.ultimoAcceso;
        lineaParaActualizar.ultimoAcceso = datosVerboso->numOperacion;

        vias[existente] = lineaParaActualizar;

    } else {
        setActual->missesLectura += 1;
        for (uint32_t i = 0; i < cantVias; i++) {
            if (vias[i].valido == 0) {
                posViaAUsar = i;
                break;
            }
        }
        if (posViaAUsar != -1) { // Hay un lugar libre con valido == 0, dirty == 0

            lineaParaActualizar = vias[posViaAUsar];
            lineaParaActualizar.valido = 1;
            lineaParaActualizar.dirty = 0;
            lineaParaActualizar.tag = tagNuevo;
            lineaParaActualizar.tagAnterior = -1;
            setActual->ciclosLectura = setActual->ciclosLectura + 1 + 100;

            datosVerboso->identificador = "2a";
            datosVerboso->cacheLine = posViaAUsar;
            datosVerboso->lineTag = -1;
            datosVerboso->validBit = 0;
            datosVerboso->dirtyBit = 0;

            setActual->bytesLeidos += (uint32_t) cache->tamañoBloque;

            datosVerboso->lastUsed = lineaParaActualizar.ultimoAcceso;
            lineaParaActualizar.ultimoAcceso = datosVerboso->numOperacion;
            vias[posViaAUsar] = lineaParaActualizar;

        } else { //tengo una via ocupada que debo reemplazar

            setActual->bytesLeidos += (uint32_t) cache->tamañoBloque;
            uint32_t menorVia = vias[0].ultimoAcceso;
            uint32_t indiceMenorVia = 0;
            for (uint32_t i = 1; i < cantVias; i++) {
                if (vias[i].ultimoAcceso < menorVia) {
                    menorVia = vias[i].ultimoAcceso;
                    indiceMenorVia = i;
                }
            }
            lineaParaActualizar = vias[indiceMenorVia];

            lineaParaActualizar.tagAnterior = lineaParaActualizar.tag;
            lineaParaActualizar.valido = 1;

            datosVerboso->validBit = 1;
            datosVerboso->dirtyBit = lineaParaActualizar.dirty;
            datosVerboso->lineTag = lineaParaActualizar.tagAnterior;
            lineaParaActualizar.tag = tagNuevo;

            if (lineaParaActualizar.dirty == 0) {
                datosVerboso->identificador = "2a";
                setActual->ciclosLectura = setActual->ciclosLectura + 1 + 100;

            } else {
                datosVerboso->identificador = "2b";
                setActual->ciclosLectura = setActual->ciclosLectura + 1 + 2*100;
                setActual->dirtyReadMisses += 1;
                lineaParaActualizar.dirty = 0;
                setActual->bytesEscritos += (uint32_t) cache->tamañoBloque;
            }
            datosVerboso->cacheLine = indiceMenorVia;

            datosVerboso->lastUsed = lineaParaActualizar.ultimoAcceso;
            lineaParaActualizar.ultimoAcceso = datosVerboso->numOperacion;
            vias[indiceMenorVia] = lineaParaActualizar;
        }
    }
    setActual->lecturas += 1;
}

void escritura(cache_t* cache, uint32_t set, uint32_t tagNuevo, datosVerboso_t* datosVerboso) {
    set_t** conjuntoDeSets = cache->sets;
    set_t* setActual = conjuntoDeSets[set];
    uint32_t cantVias = setActual->cantidadLineas;

    datosVerboso->cacheIndex = set;
    datosVerboso->cacheTag = tagNuevo;

    linea_t* vias = setActual->lineas;

    int existente = -1;
    for (uint32_t i = 0; i < cantVias; i++) {
        if (vias[i].tag == tagNuevo && vias[i].valido != 0) {
            existente = i;
        }
    }
    int posViaAUsar = -1;
    linea_t lineaParaActualizar;

    if (existente != -1) { //hay un hit

        lineaParaActualizar = vias[existente];
        setActual->ciclosEscritura += 1;

        lineaParaActualizar.tagAnterior = lineaParaActualizar.tag;
        lineaParaActualizar.valido = 1;

        datosVerboso->identificador = "1";
        datosVerboso->cacheLine = existente;
        datosVerboso->lineTag = tagNuevo;
        datosVerboso->validBit = lineaParaActualizar.valido;
        datosVerboso->dirtyBit = lineaParaActualizar.dirty;
        datosVerboso->lastUsed = lineaParaActualizar.ultimoAcceso;
        lineaParaActualizar.ultimoAcceso = datosVerboso->numOperacion;

        lineaParaActualizar.dirty = 1;

        vias[existente] = lineaParaActualizar;

    } else { //hay un miss
        setActual->missesEscritura += 1;
        for (uint32_t i = 0; i < cantVias; i++) {
            if (vias[i].valido == 0) {
                posViaAUsar = i;
                break;
            }
        }
        if (posViaAUsar != -1) {

            lineaParaActualizar = vias[posViaAUsar];

            lineaParaActualizar.tagAnterior = -1;

            lineaParaActualizar.tag = tagNuevo;
            lineaParaActualizar.valido = 1;
            lineaParaActualizar.dirty = 1;
            setActual->ciclosEscritura = setActual->ciclosEscritura + 1 + 100;

            datosVerboso->identificador = "2a";
            datosVerboso->cacheLine = posViaAUsar;
            datosVerboso->lineTag = lineaParaActualizar.tagAnterior;
            datosVerboso->validBit = 0;
            datosVerboso->dirtyBit = 0;
            datosVerboso->lastUsed = lineaParaActualizar.ultimoAcceso;
            lineaParaActualizar.ultimoAcceso = datosVerboso->numOperacion;

            setActual->bytesLeidos += (uint32_t) cache->tamañoBloque;

            vias[posViaAUsar] = lineaParaActualizar;
        } else { //tengo una via que ya fue usada antes

            setActual->bytesLeidos += (uint32_t) cache->tamañoBloque;

            uint32_t menorVia = vias[0].ultimoAcceso;
            uint32_t indiceMenorVia = 0;
            for (uint32_t i = 1; i < cantVias; i++) {
                if (vias[i].ultimoAcceso < menorVia) {
                    menorVia = vias[i].ultimoAcceso;
                    indiceMenorVia = i;
                }
            }
            lineaParaActualizar = vias[indiceMenorVia];
            lineaParaActualizar.valido = 1;

            lineaParaActualizar.tagAnterior = lineaParaActualizar.tag;

            datosVerboso->lineTag = lineaParaActualizar.tagAnterior;
            datosVerboso->validBit = lineaParaActualizar.valido;
            datosVerboso->dirtyBit = lineaParaActualizar.dirty;
            lineaParaActualizar.tag = tagNuevo;

            if (lineaParaActualizar.dirty == 0) {
                datosVerboso->identificador = "2a";
                lineaParaActualizar.dirty = 1;
                setActual->ciclosEscritura = setActual->ciclosEscritura + 1 + 100;
            } else {
                datosVerboso->identificador = "2b";
                setActual->dirtyWriteMisses += 1;
                setActual->ciclosEscritura = setActual->ciclosEscritura + 1 + 2*100;
                setActual->bytesEscritos += (uint32_t) cache->tamañoBloque;
            }
            datosVerboso->cacheLine = indiceMenorVia;

            datosVerboso->lastUsed = lineaParaActualizar.ultimoAcceso;
            lineaParaActualizar.ultimoAcceso = datosVerboso->numOperacion;

            vias[indiceMenorVia] = lineaParaActualizar;
        }
    }
    setActual->escrituras += 1;
}

void liberarCache(cache_t *c) {
    for (int i = 0; i < c->cantidadSets; i++) {
        set_t* set = c->sets[i];
        linea_t* lineas = set->lineas;
        free(lineas);
        free(set);
    }
    free(c->sets);
    free(c);
}
