JMP setsig ; se debe cambiar segun la prueba que se quiera correr

seguir:
SET R0, 0xFF
SET R1, 0x11

siguiente:
ADD R0, R1
JC siguiente

halt:
JMP halt

setear:
SET R2, 0xC9
SET R3, 0xC9

prueba:
MIX R2, R3 ;resultado esperado 0x63

haltdos:
JMP haltdos

setsig:
SET R0, 0xFF
SET R1, 0x10
SET R2, 0x01
SET R3, 0x03

pruebasig:
SIG R0     ;resultado esperado: 0x00 sin flags
SIG R1     ;resultado esperado: 0x11 sin flags
SUB R2, R3
SIG R2     ;resultado esperado: 0xFF sin flags

halttres:
JMP halttres

