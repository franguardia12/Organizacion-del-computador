#ifndef _LISTADOBLEENLAZADA_HH_
#define _LISTADOBLEENLAZADA_HH_

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

/* List */

typedef struct s_list {
	struct s_listElem* first;
	struct s_listElem* last;
	uint32_t  size;
} list_t;

typedef struct s_listElem {
	struct s_listElem* next;
	struct s_listElem* prev;
	int data;
} listElem_t;

list_t* listNew();
uint32_t  listGetSize(list_t* l);
void  listAddFirst(list_t* l, int data);
void  listAddLast(list_t* l, int data);
int listGet(list_t* l, uint32_t i);
uint32_t listRemove(list_t* l, uint32_t i);
uint32_t listRemoveElement(list_t* l, int elem);
uint32_t listRemoveAllEqualElements(list_t* l, int elem);
void  listSwap(list_t* l, uint8_t i, uint8_t j);
void  listDelete(list_t* l);


#endif