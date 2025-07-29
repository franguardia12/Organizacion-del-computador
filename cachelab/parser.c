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

bool esPotenciaDeDos(int n) {
    if (n == 0) {
        return false;
    }
    return (n & (n - 1)) == 0;
}

int parse(int argc, char *argv[]) {
    int n = 0;
    if(argc != 5 && argc != 8) {
        fprintf(stderr, "Cantidad de argumentos inválida\n");
        return -1;
    }

    if (argc == 8) {
        int sonIguales = strcmp(argv[5], "-v");
        if (sonIguales != 0) {
            fprintf(stderr, "Modo verboso inválido\n");
            return -1;
        }
        //solo en este caso estarán los parámetros n y m
        n = atoi(argv[6]);
        int m = atoi(argv[7]);
        if(n < 0 || m < 0 || n > m) {
            fprintf(stderr, "Los parámetros n y m son inválidos\n");
            return -1;
        }
    }

    const char *filename = argv[1];
    if(access(filename, F_OK) == -1) {
        fprintf(stderr, "El archivo de trazas no existe\n");
        return -1;
    }

    if(!esPotenciaDeDos(atoi(argv[2])) || !esPotenciaDeDos(atoi(argv[3])) || !esPotenciaDeDos(atoi(argv[4]))) {
        fprintf(stderr, "Los tamaños de caché no son potencias de dos\n");
        return -1;
    }

    if(atoi(argv[2]) < atoi(argv[3]) * atoi(argv[4]) * 4) {
        fprintf(stderr, "La combinación de parámetros de la caché es inválida\n");
        return -1;
    }

    return 0;

}