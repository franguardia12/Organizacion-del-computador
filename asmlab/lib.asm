global strClone
global strPrint
global strCmp
global strLen
global strDelete

global arrayNew
global arrayDelete
global arrayPrint
global arrayGetSize
global arrayAddLast
global arrayGet
global arrayRemove
global arraySwap

global cardCmp
global cardClone
global cardAddStacked
global cardDelete
global cardGetSuit
global cardGetNumber
global cardGetStacked
global cardPrint
global cardNew

extern getCloneFunction
extern getDeleteFunction
extern getPrintFunction
extern malloc
extern free
extern fputs
extern fprintf
extern listDelete
extern listClone
extern listPrint
extern listAddFirst
extern arrayGet
extern listNew
extern strClone
extern intClone
extern intDelete
extern strDelete
extern intClone
extern intCmp


ARRAY_OFF_TYPE EQU 0
ARRAY_OFF_SIZE EQU 4
ARRAY_OFF_CAPACITY EQU 5
ARRAY_OFF_DATA EQU 8
ARRAY_SIZE EQU 16

LIST_OFF_TYPE EQU 0
LIST_OFF_SIZE EQU 4
LIST_OFF_FIRST EQU 8
LIST_SIZE EQU 24

LIST_ELEMENT_OFF_DATA EQU 0
LIST_ELEMENT_OFF_NEXT EQU 8
LIST_ELEMENT_SIZE EQU 24

CARD_OFF_SUIT EQU 0
CARD_OFF_NUMBER EQU 8
CARD_OFF_STACKED EQU 16
CARD_SIZE EQU 24

PUNTERO_SIZE_OFF EQU 8

INT_TYPE EQU 1
STRING_TYPE EQU 2
CARD_TYPE EQU 3

STRING_NULL: db 'NULL', 0

section .data
format_left: db '[', 0
format_right: db ']', 0
comma_space: db ',', 0
corchete_inicial: db '{', 0
corchete_final: db '}', 0
string_guion: db '-', 0

section .text

; ** String **
;Fran
;char* strClone(char* a);
strClone:
    push rbp
    mov rbp, rsp
    push r12
    push r13
    mov r12, rdi

    xor rcx, rcx
    xor rdx, rdx
    .loop:
        cmp byte[r12 + rcx], dl
        je .reservar
        inc rcx
        jmp .loop

    .reservar:
        inc rcx
        mov r13, rcx
        mov rdi, rcx
        call malloc
        xor rdx, rdx
        mov rdi, rax
        jmp .copiar

    .copiar:
        mov cl, [r12 + rdx]
        mov [rdi + rdx], cl
        inc rdx
        cmp rdx, r13
        jne .copiar
        jmp .fin

    .fin:
        mov rax, rdi
        pop r13
        pop r12
        pop rbp
        ret

;Mate
;void strPrint(char* a, FILE* pFile)
strPrint:
    ;Llamaremos a fputs asi que debemos alinear la pila
    push rbp
    mov rbp, rsp

    ;Chequeamos si la cadena está vacía
    cmp byte[rdi], 0
    jz .escribirNull

    ;Si la cadena no está vacía, llamamos a fputs para escribirla (rdi: char *a, rsi: FILE* pfile)
    call fputs
    jmp .fin

    .escribirNull:
        ;Llamamos a fputs para escribir NULL. Para eso, debemos pasar por parametro NULL (Movemos el string NULL a rdi)
        mov rdi, STRING_NULL
        call fputs
        jmp .fin

    .fin:
        pop rbp 
        ret

;uint32_t strLen(char* a);
strLen:
    ;rdi es un puntero a un char, al comienzo de un array de chars y tengo que ir sumando de a 1 (un Byte por char)
    xor rax, rax        ;lo deje en cero

    .condicion:
        cmp byte[rdi + rax], 0
        jz .end         ;corta cuando rdi vale 0 ---->>> '\0'
        inc rax         ;le sumo 1, pero no uso ADD porque es más rápido con INC
        jmp .condicion

    .end:
        ret

;Fran
;int32_t strCmp(char* a, char* b);
strCmp:
    push rbp
    mov rbp, rsp
    push r12
    push r13

    xor rcx, rcx

.loop:
    mov al, byte [rdi + rcx]
    mov bl, byte [rsi + rcx]
    cmp al, bl
    jl .segundoMayor
    jg .primeroMayor
    cmp al, 0
    je .iguales
    inc rcx
    jmp .loop

.segundoMayor:
    mov eax, 1
    jmp .fin

.primeroMayor:
    mov eax, -1
    jmp .fin

.iguales:
    xor eax, eax
    jmp .fin

.fin:
    pop r13
    pop r12
    pop rbp
    ret

;Mate
;void strDelete(char* a);
;Borra el string pasado por parámetro. Esta función es equivalente a la función free.
strDelete:
    push rbp
    mov rbp, rsp
    call free
    pop rbp
    ret


; ** Array **

; uint8_t arrayGetSize(array_t* a)
arrayGetSize:
    xor rax, rax

    mov al, byte[rdi + ARRAY_OFF_SIZE]   ;rdi + type(4 Bytes)
    ret

; void arrayAddLast(array_t* a, void* data)
arrayAddLast:
    push rbp            ;alineada
    mov rbp, rsp
    push r12            ;desalineada
    push r13            ;alineada
    push r14            ;desalineada
    push r15            ;alineada
    push rbx
    sub rsp, 8

    xor r12, r12
    xor r13, r13
    xor r14, r14
    xor r15, r15

    mov r12, rdi        ;r12 = array_t* a
    mov r13, rsi        ;r13 = void* data

    .verifSize:
        mov r14b, byte[r12 + ARRAY_OFF_SIZE]        ;tengo el size del array en r14
        cmp r14b, byte[r12 + ARRAY_OFF_CAPACITY]
        jz .fin

    .copyData:
        mov edi, dword[r12 + ARRAY_OFF_TYPE]
        call getCloneFunction
        mov r15, rax                ;tengo la funcion de clonado en r15

        mov rdi, r13
        call r15                    ;tengo el dato copiado en rbx
        mov rbx, rax

    .agregarUltimo:
        xor rdx, rdx
        lea rdx, [r14 * PUNTERO_SIZE_OFF]
        xor rcx, rcx
        mov rcx, [r12 + ARRAY_OFF_DATA]             ;en rcx tengo el puntero de la data
        mov [rcx + rdx], rbx

        inc r14b
        mov byte[r12 + ARRAY_OFF_SIZE], r14b

    .fin:
        add rsp, 8
        pop rbx
        pop r15
        pop r14
        pop r13
        pop r12
        pop rbp
        ret

;Fran
; void* arrayGet(array_t* a, uint8_t i)
arrayGet:
    push rbp
    mov rbp, rsp
    push r12

    .checkRango:
        cmp byte [rdi + ARRAY_OFF_SIZE], sil
        jle .fueraDeRango
        cmp byte sil, 0
        jl .fueraDeRango

    .obtenerValor:
        mov r12, [rdi + ARRAY_OFF_DATA]
        mov rax, [r12 + rsi*8]
        jmp .fin

    .fueraDeRango:
        xor rax, rax
        jmp .fin

    .fin:
        pop r12
        pop rbp
        ret

; array_t* arrayNew(type_t t, uint8_t capacity)
arrayNew:
    push rbp            ;alineada
    mov rbp, rsp
    push r12            ;desalineada
    push r13            ;alineada
    push r14            ;desalineada
    sub rsp, 8

    xor r12, r12
    xor r13, r13
    xor r14, r14

    mov r12d, edi
    mov r13b, sil

    mov rdi, ARRAY_SIZE
    call malloc wrt ..plt
    mov r14, rax
    mov dword[r14 + ARRAY_OFF_TYPE], r12d         ;guardo el tipo del array nuevo
    mov byte[r14 + ARRAY_OFF_SIZE], 0           ;inicializo el size
    mov byte[r14 + ARRAY_OFF_CAPACITY], r13b     ;guardo la capacidad

    lea rdi, [PUNTERO_SIZE_OFF*r13]             ;uso lea y no mov, porque quiero -> 8 (inmediato) * (valor en r13)
    call malloc wrt ..plt

    xor rdx, rdx
    mov rdx, rax

    mov [r14 + ARRAY_OFF_DATA], rdx

    mov rax, r14

    add rsp, 8
    pop r14
    pop r13
    pop r12
    pop rbp
    ret

;Fran
; void* arrayRemove(array_t* a, uint8_t i)
arrayRemove:
    push rbp
    mov rbp, rsp
    push r12
    push r13

    .checkRango:
        cmp byte [rdi + ARRAY_OFF_SIZE], sil
        jbe .fueraDeRango

    .obtenerValor:
        mov r12, [rdi + ARRAY_OFF_DATA]
        mov r13, [r12 + rsi*8]

    .ubicarPosicion:
        xor rdx, rdx
        xor rcx, rcx
        mov dl, sil
        inc rdx

    .loop:
        cmp dl, [rdi + ARRAY_OFF_SIZE]
        je .devolverValor
        mov rcx, [r12 + rdx*8]
        mov [r12 + rsi*8], rcx
        inc rdx
        inc rsi
        jmp .loop

    .devolverValor:
        dec byte [rdi + ARRAY_OFF_SIZE]
        mov rax, r13
        jmp .fin

    .fueraDeRango:
        xor rax, rax
        jmp .fin

    .fin:
        pop r13
        pop r12
        pop rbp
        ret

;Mate
; void arraySwap(array_t* a, uint8_t i, uint8_t j)
; Invierte el contenido del i-ésimo elemento con el j-ésimo elemento. Si alguno de los dos ı́ndices se encuentra fuera de rango, no realiza ninguna acción.
; rdi: a - 16 bytes   rsi: i - 1 byte   rdx: j - 1 byte

arraySwap:
    push rbp
    mov rbp, rsp
    push r12
    push r13
    push r14
    push r15

    ;como voy a hacer un call a arrayGet, necesito guardar mis valores en registros NO volátiles
    ;Los "limpio" con un xor

    xor r12, r12
    xor r13, r13
    xor r14, r14
    xor r15, r15

    ;Guardo los parametros en registros no volatiles
    mov r12, rdi      ;r12: a 
    mov r13b, sil     ;r13: i
    mov r14b, dl      ;r14: j

    call arrayGet

    mov r15, rax      ;Lo que me devuelve arrayGet (rax) lo guardo en r15

    ;Si un arrayGet nos devuelve 0, el indice esta fuera de rango
    cmp r15, 0
    jz .fin

    ;Vuelvo a setear los parametros
    mov rdi, r12        
    mov rsi, r14

    call arrayGet

    ;Si un arrayGet nos devuelve 0, el indice esta fuera de rango
    cmp rax, 0
    jz .fin

    ;Hasta acá validamos que i y j estén dentro de rango

    ;Ahora debemos invertir los valores: Tenemos en r15: elemA - rax: elemB - r13: i - r14: j

    mov rcx, [r12 + ARRAY_OFF_DATA]  ; En rcx tendremos el INICIO del atributo DATA del array

    ;Cuando querramos sumar indices, no podemos sumar simplemente el indice que tenemos en r13 o r14, ya que estamos trabajando con PUNTEROS
    ;Cada puntero tiene 8 bytes, por lo tanto, para moverme de elemento en elemento debo ir saltando de 8 bytes en 8 bytes
    ;Realizo el ajuste de los indices con un shift left: 1 shift left multiplica a la izquierda por 2, entonces necesito 3 shift left: 2x2x2
    shl r13, 3
    shl r14, 3

    mov [rcx + r13], rax
    mov [rcx + r14], r15
    
    .fin:
        pop r15
        pop r14
        pop r13
        pop r12
        pop rbp
        ret

; void arrayDelete(array_t* a) 
arrayDelete:
    push rbp
    mov rbp, rsp
    push r12
    push r13        ;alineada
    push r14
    push r15        ;alineada

    xor r12, r12
    xor r13, r13
    xor r14, r14
    xor r15, r15

    mov r12, rdi    ;r12 = array_t* a

    mov r13d, [r12 + ARRAY_OFF_TYPE]    ;r13 = a->type

    mov rdi, r13
    call getDeleteFunction
    mov r14, rax                ;r14 = funcion de borrado

    mov r13, [r12 + ARRAY_OFF_DATA]             ;r13 = puntero a data

    .for:
        cmp byte[r12 + ARRAY_OFF_SIZE], r15b
        jz .borrarArray
        mov rdi, [r13 + PUNTERO_SIZE_OFF * r15]
        call r14
        inc r15
        jmp .for

    .borrarArray:
        mov rdi, r13
        call free

    .borrarStruct:
        mov rdi, r12
        call free

    .fin:
        pop r15
        pop r14
        pop r13
        pop r12
        pop rbp
        ret

;Fran
;void arrayPrint(array_t* a, FILE* pFile)
arrayPrint:
    push rbp
    mov rbp, rsp
    push r12
    push r13
    push r14
    push r15
    push rbx
    sub rsp, 8

    mov r12, rdi
    mov r13, rsi
    mov r14, [rdi + ARRAY_OFF_DATA]

    mov edi, [r12 + ARRAY_OFF_TYPE]
    call getPrintFunction
    mov r15, rax

    .imprimirCaracterInicial:
        mov rdi, r13
        mov rsi, format_left
        mov al, 0
        call fprintf
        xor rbx, rbx

    .check:
        mov dil, [r12 + ARRAY_OFF_SIZE]
        cmp dil, 0
        je .imprimirCaracterFinal

    .imprimirContenido:
        mov rdi, [r14 + rbx*8]
        mov rsi, r13
        call r15
        inc rbx
        cmp bl, [r12 + ARRAY_OFF_SIZE]
        je .imprimirCaracterFinal
        mov rdi, r13
        mov rsi, comma_space
        mov al, 0
        call fprintf
        jmp .imprimirContenido

    .imprimirCaracterFinal:
        mov rdi, r13
        mov rsi, format_right
        mov al, 0
        call fprintf
        jmp .fin

    .fin:
        add rsp, 8
        pop rbx
        pop r15
        pop r14
        pop r13
        pop r12
        pop rbp
        ret

; ** Card **

;Mate
; card_t* cardNew(char* suit, int32_t* number)
;Crea una nueva card_t con el suit y el number pasados por parámetro. Debe hacer una copia de los argumentos.

;rdi; suit   rsi: number
cardNew:
    push rbp
    mov rbp, rsp
    push r12
    push r13
    push r14
    push r15

    ;Limpio los registros no volátiles
    xor r12, r12
    xor r13, r13
    xor r14, r14
    xor r15, r15

    mov r12, rdi ;r12: suit
    mov r13, rsi ;r13: number

    mov rdi, CARD_SIZE
    call malloc
    mov r14, rax   ;r14: *carta

    mov rdi, r12
    call strClone
    mov [r14 + CARD_OFF_SUIT], rax

    mov rdi, r13
    call intClone
    mov [r14 + CARD_OFF_NUMBER], rax   ;Guardo en el number el dato clonado que nos pasaron por parametro

    mov rdi, 3
    call listNew
    mov [r14 + CARD_OFF_STACKED], rax   ;Guardo en el stacked la lista nueva que creamos

    mov rax, r14                       ;En r14 teniamos la carta. La seteo en el valor de retorno

    pop r15
    pop r14
    pop r13
    pop r12
    pop rbp
    ret

;char* cardGetSuit(card_t* c)
cardGetSuit:
    mov rax, [rdi + CARD_OFF_SUIT]
    ret

;Fran
;int32_t* cardGetNumber(card_t* c)
cardGetNumber:
    push rbp
    mov rbp, rsp
    push r12

    mov r12, [rdi + CARD_OFF_NUMBER]
    mov rax, r12

    pop r12
    pop rbp
    ret

;Mate
;list_t* cardGetStacked(card_t* c)
;Devuelve un puntero a la lista de cartas apiladas bajo la carta c
cardGetStacked:
    ;rdi: c
    mov rax, [rdi + CARD_OFF_STACKED] 

    ret

;void cardPrint(card_t* c, FILE* pFile)
cardPrint:
    push rbp
    mov rbp, rsp
    push r12
    push r13    ;alineada
    push r14
    push r15    ;alineada
    push rbx
    sub rsp, 8

    mov r12, rdi    ;r12 = card_t* c
    mov r13, rsi    ;r13 = FILE* pFile

    .guardoFunciones:
        mov rdi, INT_TYPE
        call getPrintFunction
        mov rbx, rax            ;rbx = funcion de print int

        mov rdi, STRING_TYPE
        call getPrintFunction
        mov r15, rax            ;r15 = funcion de print strings

    .inicioTexto:
        mov rdi, corchete_inicial
        mov rsi, r13
        call r15

    .suit:
        mov rdi, [r12 + CARD_OFF_SUIT]
        mov rsi, r13
        call r15

    .guion1:
        mov rdi, string_guion
        mov rsi, r13
        call r15

    .number:
        mov rdi, [r12 + CARD_OFF_NUMBER]
        mov rsi, r13
        call rbx

    .guion2:
        mov rdi, string_guion
        mov rsi, r13
        call r15

    .stacked:
        mov rdi, [r12 + CARD_OFF_STACKED]
        mov rsi, r13
        call listPrint

    .finTexto:
        mov rdi, corchete_final
        mov rsi, r13
        call r15

    add rsp, 8
    pop rbx
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbp
    ret

;Mate
;int32_t cardCmp(card_t* a, card_t* b)
;Realiza una comparación entre cartas. En primer lugar, compara en orden lexicográfico los suits y devuelve:
    ;1 si a->suit < b->suit
    ;−1 si b->suit < a->suit
    ;Si son iguales los suits, se procede a comparar número y devuelve:
    ;0 si son iguales
    ;1 si a->number < b->number
    ;−1 si b->number < a->number
cardCmp:
    push rbp
    mov rbp, rsp
    push r12
    push r13
    push r14
    push r15

    mov r12, [rdi + CARD_OFF_SUIT]  ;r12: puntero a suit de a
    mov r13, [rsi + CARD_OFF_SUIT]  ;r13: puntero a suit de b
    mov r14, rdi
    mov r15, rsi

    .compararPalos:
        mov rdi, r12
        mov rsi, r13
        call strCmp
        cmp eax, 0
        je .compararNumeros
        jmp .fin

    .compararNumeros:
        mov rdi, [r14 + CARD_OFF_NUMBER]
        mov rsi, [r15 + CARD_OFF_NUMBER]
        call intCmp
        jmp .fin

    .fin:
        pop r15
        pop r14
        pop r13
        pop r12
        pop rbp
        ret

;card_t* cardClone(card_t* c)
cardClone:
    push rbp
    mov rbp, rsp
    push r12
    push r13    ;alineada
    push r14
    push r15    ;alineada

    mov r12, rdi        ;r12 = card_t* c
    mov r13, [rdi + CARD_OFF_STACKED]       ;r13 = list_t* stacked

    mov rdi, CARD_SIZE
    call malloc
    mov r14, rax        ;r14 = puntero a espacio libre para carta

    .clonoSuit:
        mov rdi, [r12 + CARD_OFF_SUIT]
        call strClone
        mov [r14 + CARD_OFF_SUIT], rax

    .clonoNumber:
        mov rdi, [r12 + CARD_OFF_NUMBER]
        call intClone
        mov [r14 + CARD_OFF_NUMBER], rax

    .clonoStacked:
        mov rdi, [r12 + CARD_OFF_STACKED]
        call listClone
        mov r15, rax            ;r15 = la lista clonada
        mov [r14 + CARD_OFF_STACKED], r15

    mov rax, r14

    pop r15
    pop r14
    pop r13
    pop r12
    pop rbp
    ret

;Fran
;void cardAddStacked(card_t* c, card_t* card)
cardAddStacked:
    push rbp
    mov rbp, rsp

    mov rdi, [rdi + CARD_OFF_STACKED]
    call listAddFirst

    pop rbp
    ret

;Fran
;void cardDelete(card_t* c)
cardDelete:
    push rbp
    mov rbp, rsp
    push r12
    sub rsp, 8

    mov r12, rdi

    mov rdi, [r12 + CARD_OFF_NUMBER]
    call intDelete
    mov rdi, [r12 + CARD_OFF_SUIT]
    call strDelete
    mov rdi, [r12 + CARD_OFF_STACKED]
    call listDelete
    mov rdi, r12
    call free

    add rsp, 8
    pop r12
    pop rbp
    ret
