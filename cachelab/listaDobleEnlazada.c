#include "listaDobleEnlazada.h"
#include <stdio.h>

list_t *listNew(){

    list_t *lista = (list_t*) malloc(sizeof(list_t));
    lista->first = 0;
    lista->last = 0;
    lista->size = 0;
    return lista;
}

uint32_t listGetSize(list_t *l){

    if (l->first == 0) {
        return 0;
    }
    uint32_t contador = 0;
    listElem_t* primerElemento = l->first;
    listElem_t* siguienteElemento = primerElemento->next;

    while (siguienteElemento != 0) {
        contador++;
        siguienteElemento = siguienteElemento->next;
    }
    return contador + 1;
}

int listGet(list_t *l, uint32_t i){

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

void listAddFirst(list_t *l, int valor){

    if (l->size == 0) {
        listElem_t* nodo = malloc(sizeof(listElem_t));
        nodo->data = valor;
        nodo->next = l->first;
        nodo->prev = 0;
        l->first = nodo;
        l->last = nodo;
        l->size++;
        return;
    }

    listElem_t* nodo = malloc(sizeof(listElem_t));

    nodo->data = valor;
    nodo->next = l->first;
    nodo->prev = 0;

    if (l->first) {
        l->first->prev = nodo;
    }

    l->first = nodo;
    l->size++;

    if (l->size == 1) {
        l->last = nodo;
        return;
    }
}

void listAddLast(list_t *l, int valor){

    if (l->size == 0) {
        listElem_t* nodo = malloc(sizeof(listElem_t));
        nodo->data = valor;
        nodo->next = 0;
        nodo->prev = 0;
        l->first = nodo;
        l->last = nodo;
        l->size++;
        return;
    }

    listElem_t* nodo = malloc(sizeof(listElem_t));
    nodo->data = valor;
    nodo->next = 0;
    nodo->prev = l->last;
    l->last->next = nodo;
    l->last = nodo;
    l->size++;
    return;
}

uint32_t listRemoveElement(list_t* l, int elem) {

    if (l->size == 0) {
        return -1;
    }
    listElem_t* act = l->first;
    for (uint32_t i = 1; i <= l->size; i++){
        if (act->data == elem) {
            return listRemove(l, i);
        }
        act = act->next;
    }
    return -1;
}

uint32_t listRemoveAllEqualElements(list_t* l, int elem) {
    if (l->size == 0) {
        return -1;
    }

    uint32_t tamaño = l->size;
    for (uint32_t i = 0; i < tamaño; i++){
        listElem_t* act = l->first;
        for (uint32_t j = 1; j <= l->size; j++){
            if (act->data == elem) {
                listRemove(l, j);
                tamaño--;
            }
            act = act->next;
        }
    }
    return -1;
}

uint32_t listRemove(list_t *l, uint32_t i){

    if ((i >= l->size)) {
        return 0;
    }
    uint32_t contador = 0;
    listElem_t *actual = l->first;

    while (contador != i) {
        actual = actual->next;
        contador++;
    }
    int dato = actual->data;
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
        l->first = 0;
        l->last = 0;
    }

    free(actual);
    l->size--;
    return dato;
}

void listSwap(list_t *l, uint8_t i, uint8_t j){

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
    int varAux = elemtI->data;
    elemtI->data = elemtJ->data;
    elemtJ->data = varAux;
}

void listDelete(list_t *l){

    if (l->size == 0) {
        free(l);
        return;
    }
    listElem_t* act = l->first;
    while (act != 0) {
        listElem_t* actProx = act->next;
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