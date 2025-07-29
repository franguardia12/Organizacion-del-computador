#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "lib.h"

void casoArray() {
    uint8_t capacidad = 6;
    array_t *mazo = arrayNew(3, capacidad);

    int32_t number1 = 12;
    char str1[] = "espada";
    card_t *carta1Array = cardNew(str1, &number1);

    arrayAddLast(mazo, carta1Array);

    int32_t number2 = 8;
    char str2[] = "oro";
    card_t *carta2Array = cardNew(str2, &number2);

    arrayAddLast(mazo, carta2Array);

    int32_t number3 = 1;
    char str3[] = "espada";
    card_t *carta3Array = cardNew(str3, &number3);

    arrayAddLast(mazo, carta3Array);

    int32_t number4 = 10;
    char str4[] = "basto";
    card_t *carta4Array = cardNew(str4, &number4);

    arrayAddLast(mazo, carta4Array);

    int32_t number5 = 4;
    char str5[] = "basto";
    card_t *carta5Array = cardNew(str5, &number5);

    arrayAddLast(mazo, carta5Array);
    

    FILE *file = fopen("outPruebaArray.txt", "w");
    arrayPrint(mazo, file);

    uint8_t indiceCarta1 = 4;
    card_t *cartaObtenida1 = arrayGet(mazo, indiceCarta1);

    uint8_t indiceCarta2 = 3;
    card_t *cartaObtenida2 = arrayGet(mazo, indiceCarta2);

    cardAddStacked(cartaObtenida1, cartaObtenida2);

    arrayPrint(mazo, file);
    fclose(file);

    arrayDelete(mazo);
    cardDelete(carta1Array);
    cardDelete(carta2Array);
    cardDelete(carta3Array);
    cardDelete(carta4Array);
    cardDelete(carta5Array);
}

void casoList() {
    list_t *mazo = listNew(3);

    int32_t number1 = 12;
    char str1[] = "espada";
    card_t *carta1List = cardNew(str1, &number1);

    listAddLast(mazo, carta1List);

    int32_t number2 = 8;
    char str2[] = "oro";
    card_t *carta2List = cardNew(str2, &number2);

    listAddLast(mazo, carta2List);

    int32_t number3 = 1;
    char str3[] = "espada";
    card_t *carta3List = cardNew(str3, &number3);

    listAddLast(mazo, carta3List);

    int32_t number4 = 10;
    char str4[] = "basto";
    card_t *carta4List = cardNew(str4, &number4);

    listAddLast(mazo, carta4List);

    int32_t number5 = 4;
    char str5[] = "basto";
    card_t *carta5List = cardNew(str5, &number5);

    listAddLast(mazo, carta5List);
    

    FILE *file = fopen("outPruebaList.txt", "w");
    listPrint(mazo, file);

    uint8_t indiceCarta1 = 4;
    card_t *cartaObtenida1 = listGet(mazo, indiceCarta1);

    uint8_t indiceCarta2 = 3;
    card_t *cartaObtenida2 = listGet(mazo, indiceCarta2);

    cardAddStacked(cartaObtenida1, cartaObtenida2);

    listPrint(mazo, file);
    fclose(file);

    listDelete(mazo);
    cardDelete(carta1List);
    cardDelete(carta2List);
    cardDelete(carta3List);
    cardDelete(carta4List);
    cardDelete(carta5List);
}

int main (void){  
    casoArray(); 
    casoList();
    return 0;
}


