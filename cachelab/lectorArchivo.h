#include "cache.h"
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdarg.h>

void procesarArchivoVerboso(FILE* archivo, cache_t* cache, int n, int m);

void leerLinea(char* linea, cache_t* cache, datosVerboso_t* datos);

void leerLineaVerboso(char* linea, cache_t* cache, datosVerboso_t* datos);

void imprimirLineaVerboso(datosVerboso_t* datos, cache_t* cache);