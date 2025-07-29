#include "lectorArchivo.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#define MAX_LINE_LENGHT 1024

void procesarArchivoVerboso(FILE* archivo, cache_t* cache, int n, int m) {
    char linea[MAX_LINE_LENGHT];
    int numOperacion = 0;
    datosVerboso_t* datos = newDatosVerboso();
    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = '\0';
        if(numOperacion >= n && numOperacion <= m) {
            leerLineaVerboso(linea, cache, datos);
        } else {
            leerLinea(linea, cache, datos);
        }
        numOperacion++;
        datos->numOperacion = numOperacion;
    }
    free(datos);
}

void leerLinea(char* linea, cache_t* cache, datosVerboso_t* datos) {
    int tokenCount = 0;
    char* operacion;
    char* direccion;
    char *token = strtok(linea, " ");
    while (token != NULL) {
        if(tokenCount == 1) {
            operacion = token;
        } else if(tokenCount == 2) {
            direccion = token;
        }
        tokenCount++;
        token = strtok(NULL, " ");
    }
    long int valorDecimalDireccion = strtol(direccion, NULL, 16);
    procesarInstruccion(operacion, (uint32_t)(valorDecimalDireccion), cache, datos);
}

void leerLineaVerboso(char* linea, cache_t* cache, datosVerboso_t* datos) {
    int tokenCount = 0;
    char* operacion;
    char* direccion;
    char *token = strtok(linea, " ");
    while (token != NULL) {    
        if(tokenCount == 1) {
            operacion = token;
        } else if(tokenCount == 2) {
            direccion = token;
        }
        tokenCount++;
        token = strtok(NULL, " ");
    }
    long int valorDecimalDireccion = strtol(direccion, NULL, 16);

    procesarInstruccion(operacion, (uint32_t)(valorDecimalDireccion), cache, datos);
    imprimirLineaVerboso(datos, cache);
}

void imprimirLineaVerboso(datosVerboso_t* datos, cache_t* cache) {
    char indexHexa[9];
    sprintf(indexHexa, "%x", datos->cacheIndex);

    char tagHexa[9];
    sprintf(tagHexa, "%x", datos->cacheTag);

    char lineTagHexa[9];
    sprintf(lineTagHexa, "%x", datos->lineTag);

    if (datos->lineTag == (uint32_t) -1) {
        if (cache->cantidadVias == 1) {
            printf("%d %s %s %s %d %d %d %d\n", datos->numOperacion, datos->identificador, indexHexa, tagHexa, datos->cacheLine, datos->lineTag, datos->validBit, datos->dirtyBit);
            return;
        }
        else {
            printf("%d %s %s %s %d %d %d %d %d\n", datos->numOperacion, datos->identificador, indexHexa, tagHexa, datos->cacheLine, datos->lineTag, datos->validBit, datos->dirtyBit, datos->lastUsed);
            return;
        }
    } else {
        if(cache->cantidadVias == 1) {
            printf("%d %s %s %s %d %s %d %d\n", datos->numOperacion, datos->identificador, indexHexa, tagHexa, datos->cacheLine, lineTagHexa, datos->validBit, datos->dirtyBit);
            return;
        } else {
            printf("%d %s %s %s %d %s %d %d %d\n", datos->numOperacion, datos->identificador, indexHexa, tagHexa, datos->cacheLine, lineTagHexa, datos->validBit, datos->dirtyBit, datos->lastUsed);
        }
    }
}
