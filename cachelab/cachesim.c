#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <getopt.h>
#include <unistd.h>
#include <stdbool.h>
#include "parser.h"
#include "cache.h"
#include "lectorArchivo.h"


int main (int argc, char *argv[]){
    int err = parse(argc, argv);

    if (err == -1) {
        return -1;
    }

    int numSets = atoi(argv[4]);
    int numVias = atoi(argv[3]);
    int tamCache = atoi(argv[2]);

    cache_t *c = newCache((uint32_t)(numVias), (uint32_t)(numSets), (uint32_t)(tamCache));
    FILE *archivo = fopen(argv[1], "r");
    if (argc == 8) {
        procesarArchivoVerboso(archivo, c, atoi(argv[6]), atoi(argv[7]));
    } else {
        procesarArchivoVerboso(archivo, c, -1, -1);
    }

    fclose(archivo);
    imprimirEstadísticas(c);

    //función que libere toda la caché
    liberarCache(c);


    return 0;
}


