**1) a)**
Sus entradas son:
- clk: representa el ingreso de la señal del clock; se encuentra conectada a los 3 registros en simultáneo.

- input_bit: representa un bit arbitrario; se encuentra conectada a los 3 registros en simultáneo.

- en_input_bit: representa la habilitación o no de la entrada "input_bit" a los registros.

- w: habilita o no el bit en el BUS a un registro; cada registro tiene una entrada “w” independiente.

- en_out: habilita o no la salida del valor guardado en el registro hacia "Reg_output"; cada registro tiene una entrada "en_out" independiente.

Sus salidas son:
- Reg_output: representa el dato guardado en los registros cuando está habilitada la entrada "en_out" de cada uno de ellos;
  -cuando 2 registros son habilitados y tienen guardados bits distintos, en "Reg_out" ocurre un error.
- Reg_Debug:

Las entradas de control son:
- en_input_bit: controla la entrada a los registros.
- w: controla la escritura.
- en_out: controla la salida.

**1) b)**

Suponemos que ingresamos al circuito un 1:

secuencia:

	input_bit = 1  en_input_bit = 1  ;se pone un 1 en la entrada D del flipflop habilitando el componente de tres estados

	w1 = 1  en_input_bit = 0  ;se guarda el valor de D en la salida Q habilitando la entrada de escritura

	w1 = 0  ;el valor de R1 es tomado de Q, así que deshabilitando la entrada de escritura se evita que se sobreescriba

general:

	input_bit = 1  en_input_bit = 1

	w1 = 1  en_input_bit = 0

	w1 = 0

**1) c)**

Suponemos que ingresamos al circuito un 1:

secuencia:

	input_bit  en_iput_bit  w0  ;se pone un 1 en el R0

	en_out0    w1		    ;se transfiere a R1

	en_out2    w0		    ;se transfiere el dato de R2 a R0

	en_out1    w2		    ;se transfiere el dato de R1 a R2
	
	
Generalizado:

general:

	en_iput_bit  w0

	en_out0      w1

	en_out2      w0

	en_out1      w2

	
;En este caso como input_bit no está activado, siempre se ingresa un 0 a R0. En general, para ingresar un dato arbitrario a R0, en_input_bit siempre debe estar activado, y el bit que ingresa lo define la activación (o no) de input_bit

**2) a.1)**
En este caso al ser orgaSmall de arquitectura Von Neumann la memoria de datos e instrucciones está compartida, es decir es como una memoria central por lo que datos y programas conviven en el mismo sistema. Es una memoria de 256 palabras de 8 bits por lo que teniendo en cuenta que el tamaño de palabra es de 8 bits y hay un direccionamiento a byte entonces tiene la capacidad de almacenar 8 entradas de datos de 1 byte (8 bits).

**2) b.1)**
El PC tiene un tamaño de 8 bits (1 Byte), debido a que únicamente almacena la dirección de memoria de la próxima instrucción a ejecutar.

**2) c.1)**
Las instrucciones en orgaSmall están codificadas en 16 bits, los primeros 5 son el opcode y el resto son los parámetros. Son la cantidad de opcode los que limitan la cantidad de instrucciones nuevas que se pueden agregar, en este caso solo se podrían agregar 7 (las que corresponden a los opcode que van desde 9 hasta 15 inclusive) para no pisar una instrucción ya creada.

**2) a.2)** 
La señal inc del PC cumple la función de incrementar en 1 la dirección de memoria que guarda el Program Counter en ese momento, para así apuntar a la siguiente instrucción que debe ejecutar el procesador.

**2) b.2)** 
La señal opW lo que hace es habilitar la escritura de un nuevo valor de un flag ante una operación. Para entender esto, hay que entender como funciona la ALU por adentro: Hay un Flip-Flop D para cada flag: C,Z,N. Estos Flip-FLops permiten almancenar el valor de un flag por cuanto sea necesario. Entonces, cuando queremos escribir y guardar el valor de un flag, necesitamos habilitar la escritura, y para eso está el opW.

**2) c.2)**
El "DE_enOutImm" sirve para habilitar y deshabilitar las entradas al BUS de un valor inmediato.
El Decode es el que indica que registro se va a leer y escribir, luego de recibir la parte baja y alta de una instrucción (si la instrucción así lo requiere).

**2) d.2)** 
A muy bajo nivel, no existen "If's" que nos permitan resolver los saltos condicionales. En la Unidad de Control lo que hay son 3 señales internas: jc_microOp, jz_microOp, jn_microOp, que indican si se encendió una de las señales JN, JC o JZ. Estas señales van a parar a dos lugares diferentes:

	1. Por un lado, las 3 señales van a una compuerta OR. El resultado de esta compuerta indica si se quiere (o no) ejecutar una instrucción condicional. Este bit de resultado pasa a ser el bit de control de un Multiplexor que llamaremos MUX1, que en unos momentos describiremos cuales son sus entradas.

	2. Por otro lado, las 3 señales van cada una a una compuerta AND, y se comparan con el flag que corresponde a cada señal (Es decir, jc_microOp se compara con C, jz_microOp se compara con Z y jn_microOp se compara con N). El resultado de cada compuerta indica si se cumple la condicion de "if flag". Por ejemplo: Si se activó jc_microOp se quiere saber si hay Carry. Si hay carry, C = 1. Entonces, 1 and 1 = 1. Por lo tanto, el resultado nos indica que SÍ hay Carry. Este mismo razonamiento es para los 3 flags diferentes.

    El resultado de las 3 compuertas se comparar en otra compuerta OR, cuyo resultado indicará si se cumplió o no una condición. Este resultado es el bit de control de otro multiplexor que llamaremos MUX2. Las entradas de MUX2 son solo dos: El valor del microPC sumado en 1 o el valor del microPC sumado en 2. Si nunguna de las condiciones evaluadas enteriormente se cumplió, el MUX2 tomará la entrada 0: El valor del microPC sumado en 1 (Que simplemente es ejecutar la siguiente microinstucción, sin ningún salto). Si hubo una condición que se cumplió, el MUX2 toma la entrada 1: EL valor del microPC sumado en 2: Es decir, se va a saltear una microinstrucción.
 
 Nos quedaba definir las entradas del MUX1: Una de las entradas será el bit resultado del MUX2 recién explicado. Y la otra entrada será el OP_code completado con ceros. Entonces, el MUX1 toma como control si se quiere ejecutar o no una instrucción condicional. Si se quiere, el resultado del MUX1 será el bit del MUX2, y si no se quiere, simplemente el OP_code completado con ceros. 
 
 Finalmente, el resultado del MUX1 es lo que va a parar al microPC para ejecutarse.  

 **3) a)**
 Lo que va a hacer es inicialmente en la primer línea saltar a la posición de memoria en la que esté la etiqueta "seguir" puesta para luego ejecutar el código que sigue. Primero carga en el registro R0 el valor 0xFF (-1) y carga en el registro R1 el valor 0x11 (17), después sigue en la dirección de memoria que tiene la etiqueta "siguiente" ejecutando el código que sigue. Realiza la operación ADD entre los registros R0 y R1 lo cual da 0x80 (-128) como resultado (en base decimal) y luego en la siguiente línea hay un salto condicional que evalúa si el ADD anterior tiene el flag de carry encendido, en caso de que sí se saltea la próxima instrucción y pasa directo a la dirección de memoria en la que está la etiqueta halt ejecutando el código que sigue que es un ciclo que nunca termina (ya que es un salto sin condición a la misma etiqueta), en caso de que no salta de nuevo a la etiqueta siguiente volviendo a ejecutar el mismo código que tiene. En este último caso la suma continúa hasta que en algún momento la condición se cumpla y se entre en el ciclo ilimitado del final.

**3) b)**
Como la instrucciones están codificadas en 16 bits (2 bytes) en orgaSmall entonces en este caso cada instrucción en cada línea va a ocupar 2 bytes en memoria osea que por ejemplo "JMP seguir" va a estar en la posición 0x00 y luego "SET R0, OxFF" en la posición 0x02 y "SET R1, 0x11" en la posición 0x04 y así sucesivamente. Teniendo en cuenta lo anterior los valores por los que se reemplazarán las etiquetas serán: 0x02 para "seguir", 0x08 para "siguiente" y 0x0E para "halt".

**3) c)**
Son necesarios 54 ciclos de clock para llegar a "JMP halt". Este número tan elevado es debido a que luego de cada instrucción regresa al fetch (que se compone de 5 ciclos de clock)

**3) d)**
Para realizar el ADD son necesarias 5 microinstrucciones, incluyendo el "reset_microOp" (que se ubica al finalizar todas las instrucciones). De forma detallada:

1) RB_enOut  ALU_enA  RB_selectIndexOut=0

2) RB_enOut  ALU_enB  RB_selectIndexOut=1
 
3) ALU_OP=ADD ALU_opW

4) RB_enIn   ALU_enOut RB_selectIndexIn=0

5) reset_microOp
