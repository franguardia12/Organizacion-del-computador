#include "lib.h"
#include <stdio.h>

funcCmp_t *getCompareFunction(type_t t)
{
    switch (t)
    {
    case TypeInt:
        return (funcCmp_t *)&intCmp;
        break;
    case TypeString:
        return (funcCmp_t *)&strCmp;
        break;
    case TypeCard:
        return (funcCmp_t *)&cardCmp;
        break;
    default:
        break;
    }
    return 0;
}
funcClone_t *getCloneFunction(type_t t)
{
    switch (t)
    {
    case TypeInt:
        return (funcClone_t *)&intClone;
        break;
    case TypeString:
        return (funcClone_t *)&strClone;
        break;
    case TypeCard:
        return (funcClone_t *)&cardClone;
        break;
    default:
        break;
    }
    return 0;
}
funcDelete_t *getDeleteFunction(type_t t)
{
    switch (t)
    {
    case TypeInt:
        return (funcDelete_t *)&intDelete;
        break;
    case TypeString:
        return (funcDelete_t *)&strDelete;
        break;
    case TypeCard:
        return (funcDelete_t *)&cardDelete;
        break;
    default:
        break;
    }
    return 0;
}
funcPrint_t *getPrintFunction(type_t t)
{
    switch (t)
    {
    case TypeInt:
        return (funcPrint_t *)&intPrint;
        break;
    case TypeString:
        return (funcPrint_t *)&strPrint;
        break;
    case TypeCard:
        return (funcPrint_t *)&cardPrint;
        break;
    default:
        break;
    }
    return 0;
}

/** Int **/

//Fran
int32_t intCmp(int32_t *a, int32_t *b)
{
    int32_t num1 = *a;
    int32_t num2 = *b;
    if(num1 < num2) {
        return 1;
    } else if (num1 > num2) {
        return -1;
    }
    return 0;
}

// Mate
//Libera la memoria que contiene el dato pasado por parámetro
void intDelete(int32_t *a)
{
    free(a);
}


void intPrint(int32_t *a, FILE *pFile)
{
    fprintf(pFile, "%d", *a);
}

//Fran
int32_t *intClone(int32_t *a)
{
    int32_t *p = (int32_t*) malloc(sizeof(int32_t));
    *p = *a;
    return p;
}

/** Lista **/

// Mate
/*
Crea una nueva list_t vacía donde los punteros a first y last estén inicializados en cero. Además
toma el tipo de datos que almacenará la lista, este dato será utilizado por las diferentes funciones
para obtener las funciones que permiten operar con los datos almacenados.
*/
list_t *listNew(type_t t)
{
    list_t *lista = (list_t*) malloc(sizeof(list_t));
    lista->type = t;
    lista->first = 0;
    lista->last = 0;
    lista->size = 0;
    return lista;
}

uint8_t listGetSize(list_t *l)
{
    if (l->first == 0) {
        return 0;
    }
    uint8_t contador = 0;
    listElem_t* primerElemento = l->first;
    listElem_t* siguienteElemento = primerElemento->next;

    while (siguienteElemento != 0) {
        contador++;
        siguienteElemento = siguienteElemento->next;
    }
    return contador + 1;
}

//Fran
void *listGet(list_t *l, uint8_t i)
{
    if ((l->size == 0) || (i > l->size - 1)) {
        return 0;
    }
    listElem_t* act = l->first;
    while (i > 0) {
        act = act->next;
        i -= 1;
    }
    return act->data;
}

//Mate
/*
Agrega un nuevo nodo al principio de la lista, que almacene data. Esta función debe hacer una
copia del dato
*/
void listAddFirst(list_t *l, void *data)
{
    //Obtengo la función para clonar
    funcClone_t *funcClonar = getCloneFunction(l->type);
    if (l->size == 0) {
        listElem_t* nodo = malloc(sizeof(listElem_t));
        nodo->data = funcClonar(data);
        nodo->next = l->first;
        nodo->prev = 0;
        l->first = nodo;
        l->last = nodo;
        l->size++;
        return;
    }
    
    //Creo el nuevo nodo
    listElem_t* nodo = malloc(sizeof(listElem_t));

    //Asignaciones correspondientes
    nodo->data = funcClonar(data);
    nodo->next = l->first;
    nodo->prev = 0;

    if (l->first) {
        l->first->prev = nodo;
    }
    
    l->first = nodo;
    l->size++;

    if (l->size == 1) {
        //La lista estaba vacía. El nuevo nodo se convierte también en el último de la lista
        l->last = nodo;
        return;
    }
}

void listAddLast(list_t *l, void *data)
{
    funcClone_t* funcionDeClonado = getCloneFunction(l->type); //esto esta bien, como cualquier funcion que use
    if (l->size == 0) {
        listElem_t* nodo = malloc(sizeof(listElem_t));
        nodo->data = funcionDeClonado(data);
        nodo->next = 0;
        nodo->prev = 0;
        l->first = nodo;
        l->last = nodo;
        l->size++;
        return;
    }
    listElem_t* nodo = malloc(sizeof(listElem_t));
    nodo->data = funcionDeClonado(data);
    nodo->next = 0;
    nodo->prev = l->last;
    l->last->next = nodo;
    l->last = nodo;
    l->size++;
    return;
}

//Fran
list_t *listClone(list_t *l)
{
    if (l->size == 0) {

        list_t *q = (list_t*) malloc(sizeof(list_t));
        q->first = 0;
        q->last = 0;
        q->size = 0;
        q->type = l->type;
        return q;
    }
    funcClone_t* b = getCloneFunction(l->type);
    list_t *p = malloc(sizeof(list_t));
    p->first = 0;
    p->last = 0;
    p->size = 0;
    p->type = l->type;
    listElem_t* primero = (listElem_t*) malloc(sizeof(listElem_t));
    primero->data = b(l->first->data);
    primero->prev = 0;
    primero->next = 0;
    p->first = primero;
    p->last = primero;
    p->size++;
    listElem_t* actp = p->first;
    listElem_t* actl = l->first->next;
    while(actl != 0) {
        listElem_t* nodo = (listElem_t*) malloc(sizeof(listElem_t));
        nodo->data = b(actl->data);
        nodo->prev = actp;
        nodo->next = 0;
        actp->next = nodo;
        actp = nodo;
        actl = actl->next;
        p->last = nodo;
        p->size += 1;
    }
    return p;
}

//Mate
/*Quita el i-esimo elemento de la lista, si i se encuentra fuera de rango, retorna 0. El nodo en la lista
es borrado y el elemento retornado por la función
*/
void *listRemove(list_t *l, uint8_t i)
{
    if ((i >= l->size)) {
        return 0;
    }
    int contador = 0;
    listElem_t *actual = l->first;

    while (contador != i) {
        actual = actual->next;
        contador++;
    }
    void *dato = actual->data;
    if (actual->prev && actual->next) {
        actual->prev->next = actual->next;
        actual->next->prev = actual->prev;
    } else if (actual->prev && actual->next == 0) {
        actual->prev->next = actual->next;
        l->last = actual->prev;
    } else if (actual->next && actual->prev == 0) {
        actual->next->prev = actual->prev;
        l->first = actual->next;
    } else {
        //Unico elemento
        l->first = 0;
        l->last = 0;
    }

    free(actual);
    l->size--;
    return dato;
}

void listSwap(list_t *l, uint8_t i, uint8_t j)
{
    listElem_t* elemtI = l->first;
    listElem_t* elemtJ = l->first;
    if(l->size < i || l->size < j) {
        return;
    }

    for (int x=0; x<i; x++) {
        elemtI = elemtI->next;
    }
    for (int x=0; x<j; x++) {
        elemtJ = elemtJ->next;
    }
    void* varAux = elemtI->data;
    elemtI->data = elemtJ->data;
    elemtJ->data = varAux;
}

//Fran
void listDelete(list_t *l)
{
    if (l->size == 0) {
        free(l);
        return;
    }
    funcDelete_t* b = getDeleteFunction(l->type);
    listElem_t* act = l->first;
    while (act != 0) {
        listElem_t* actProx = act->next;
        b(act->data);
        free(act);
        act = actProx;
        l->first = act;
        l->size--;
    }
    l->first = 0;
    l->last = 0;
    free(l);
    return;
}
//Mate
/*
Escribe en el stream indicado por pFile la lista almacenada en l. Para cada dato llama a la función
de impresión del dato correspondiente. El formato de la lista será: [x0 ,...,xn−1 ], suponiendo que
xi es el resultado de escribir el i-ésimo elemento
*/
void listPrint(list_t *l, FILE *pFile)
{
    fprintf(pFile, "[");

    listElem_t *actual = l->first;

    while (actual != 0) {
        if (l->type == 0) {
            break;
        }
        if (l->type == 1) {
            intPrint(actual->data, pFile);
        } else if (l->type == 2) {
            strPrint(actual->data, pFile);
        } else if (l->type == 3) {
            cardPrint(actual->data, pFile);
        }
        if (actual->next) {
            fprintf(pFile, ",");
        }
        actual = actual->next;
    }
    fprintf(pFile, "]");
}

/** Game **/

game_t *gameNew(void *cardDeck, funcGet_t *funcGet, funcRemove_t *funcRemove, funcSize_t *funcSize, funcPrint_t *funcPrint, funcDelete_t *funcDelete)
{
    game_t *game = (game_t *)malloc(sizeof(game_t));
    game->cardDeck = cardDeck;
    game->funcGet = funcGet;
    game->funcRemove = funcRemove;
    game->funcSize = funcSize;
    game->funcPrint = funcPrint;
    game->funcDelete = funcDelete;
    return game;
}
int gamePlayStep(game_t *g)
{
    int applied = 0;
    uint8_t i = 0;
    while (applied == 0 && i + 2 < g->funcSize(g->cardDeck))
    {
        card_t *a = g->funcGet(g->cardDeck, i);
        card_t *b = g->funcGet(g->cardDeck, i + 1);
        card_t *c = g->funcGet(g->cardDeck, i + 2);
        if (strCmp(cardGetSuit(a), cardGetSuit(c)) == 0 || intCmp(cardGetNumber(a), cardGetNumber(c)) == 0)
        {
            card_t *removed = g->funcRemove(g->cardDeck, i);
            cardAddStacked(b, removed);
            cardDelete(removed);
            applied = 1;
        }
        i++;
    }
    return applied;
}
uint8_t gameGetCardDeckSize(game_t *g)
{
    return g->funcSize(g->cardDeck);
}
void gameDelete(game_t *g)
{
    g->funcDelete(g->cardDeck);
    free(g);
}
void gamePrint(game_t *g, FILE *pFile)
{
    g->funcPrint(g->cardDeck, pFile);
}

