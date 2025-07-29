#include <stdint.h>
/*
 * CS:APP Data Lab
 *
 * <Please put your name and userid here>
 *
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:

  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code
  must conform to the following style:

  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>

  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.


  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 *
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce
 *      the correct answers.
 */


#endif
//1
/*
 * bitAnd - x&y using only ~ and |
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  /*
  La idea es que al no tener el AND se pueda hacer la misma operación con el OR pero aprovechando la negación de los bits.
  Si ambos valores son 1 => ~x | ~y = 0 => ~z = 1
  Si ambos valores son 0 => ~x | ~y = 1 => ~z = 0
  Si los valores son distintos => ~x | ~y = 1 => ~z = 0
  En todos los casos se obtiene lo mismo que haciendo un AND en cada bit
  */
  int z = ~x | ~y;
  return ~z;
}
/*
 * bitMatch - Create mask indicating which bits in x match those in y
 *            using only ~ and &
 *   Example: bitMatch(0x7, 0xE) = 0x6
 *   Legal ops: ~ & |
 *   Max ops: 14
 *   Rating: 1
 */
int bitMatch(int x, int y) {
  /*
  Especie de XOR invertido: El XOR devuelve 1 si los bits no coinciden. Le hacemos NOT al XOR
  y obtenemos el resultado
  */
  int xORy = x | y;
  int xBitsNotIny = xORy & ~ y; //Unos (1's) de x que no están en y
  int yBitsNotInx = xORy & ~ x; //Unos (1's) de y que no están en x
  int xor = xBitsNotIny | yBitsNotInx; //Bits que no coinciden (XOR)
  return ~xor;
}
/*
 * bitNor - ~(x|y) using only ~ and &
 *   Example: bitNor(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitNor(int x, int y) {

    int negx = ~x;
    int negy = ~y;
    int res = negx & negy;
    // La formula condensada es: (~x&~y)

  return res;
}
/*
 * bitXor - x^y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  /*
  La idea es que al no tener el XOR se pueda hacer la misma operación con el AND pero aprovechando la negación de los bits.
  Si ambos valores son 1 => ~x & ~y = 0, x & y = 1 => ~x1 & ~x2 = 0
  Si ambos valores son 0 => ~x & ~y = 1, x & y = 0 => ~x1 & ~x2 = 0
  Si los valores son distintos => ~x & ~y = 0, x & y = 0 => ~x1 & ~x2 = 1
  En todos los casos se obtiene lo mismo que haciendo una XOR en cada bit  
  */
  int x1 = ~x & ~y;
  int x2 = x & y;
  return ~x1 & ~x2;
}
//2
/*
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  int m8 = 0xAA;
  int m16 = m8 | m8 << 8;
  int m32 = m16 | m16 << 16; //mascara 10101010...

  int oddx = x & m32; //Me quedo con los impares de x
  int evenXOR = oddx ^ m32; //Hago un XOR entre los impares de x y la mascara
  /*
  Este XOR hace que si alguna posicion impar de x NO COINCIDE con la mascara (es decir, hay un 0) 
  pone un 1. Al hacer !resultado daría false si hay un 1.
  Si hay todas coincidencias en las posiciones pares, pone todo ceros. !resultado es true
  */
  return !evenXOR;
}
/*
 * anyEvenBit - return 1 if any even-numbered bit in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples anyEvenBit(0xA) = 0, anyEvenBit(0xE) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int anyEvenBit(int x) {

    int m8 = 0x55;
    int m16 = m8 | m8 << 8;
    int m32 = m16 | m16 << 16;
    int evenx = x & m32;
    // La formula condensada es: !(!(0x55555555 & x))

    return !(!evenx);
}
/*
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
  /*
  Para lograr el swap necesito obtener los bytes que se van justamente a swapear y luego reubicarlos haciendo una operación tal que "limpie"
  el byte que estaba antes y reubique ese nuevo byte en ese lugar. Trabajando cada byte por separado al final es juntar los bytes reubicados
  en las nuevas posiciones con el número original recibido y los lugares "limpios" donde se van a insertar esos bytes.

  Para obtener cada byte se puede usar una máscara apropiada y un shift a la posición menos significativa de los bytes de interés y la operación
  AND garantiza que se obtienen los bytes que estaban tal cual (por eso se usa la máscara 0xFF que tiene todos sus bits en 1)

  Para ubicar cada byte en su nueva posición simplemente se usa el valor recibido por parámetro donde se quiere insertar, de esa manera dejándolo 
  ubicado en la primera posición se vuelve más fácil hacer esto
  */
    int z = 0xFF;
    int n1 = z << (n << 3);
    int n2 = x & n1;
    int n3 = n2 >> (n << 3);
    int n3Prima = n3 & z; //obtener el primer byte
    int m1 = z << (m << 3);
    int m2 = x & m1;
    int m3 = m2 >> (m << 3);
    int m3Prima = m3 & z; //obtener el segundo byte
    int n4 = n3Prima << (m << 3); //mover a la nueva posición al primer byte
    int m4 = m3Prima << (n << 3); //mover a la nueva posición al segundo byte
    int bytes = n4 | m4; //juntar los bytes desplazados a las nuevas posiciones
    int resultado = x & ~(n1 | m1); //limpiar los lugares donde se colocarán los bytes desplazados
    return resultado | bytes; //juntar todas las partes
}
/*
 * fitsBits - return 1 if x can be represented as an
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  /*
  La cantidad maxima de bits con que podemos representar un numero es 32. Queremos ver si el numero X
  entra en un rango de 32-n bits, para así asegurar que puede ser representando con n bits.

  Esto lo podemos hacer desplazando el numero X (32 - n) bits para la izquierda. Recordar que para restar podemos
  sumar el complemento a 2 del numero que queremos restar --> 32 + (~n+1)

  Una vez shifteado para la izquierda, lo volvemos a desplazar hacia la derecha (32 - n) veces para que el numero vuelva 
  a su posicion original, y lo que queremos ver es si el NUMERO SE CORTÓ. Si el numero se cortó / perdió numeros, quiere
  decir que no se puede representar con n bits. 

  Para hacer esta comparación, se compara el número shifteado con el numero original con un XOR: Si hay numeros diferentes,
  el XOR pone unos (1's) en las posiciones, por lo que hacer el NOT al resultado nos dará lo que buscamos
  */

  int shifteadoIzq = x << (32 + (~n+1));
  int regresado = shifteadoIzq >> (32 + (~n+1));

  int comparado = x ^ regresado;

  int res = !comparado;

  return res;

}
/*
 * negate - return -x
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {

    int negx = ~x;
    // La formula condensada es: (~x + 1)

  return negx + 1;
}
/*
 * sign - return 1 if positive, 0 if zero, and -1 if negative
 *  Examples: sign(130) = 1
 *            sign(-23) = -1
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 2
 */
int sign(int x) {
  /*
  Considerando que los números enteros se representan en complemento a 2 se sabe que el bit más significativo es el que indica el signo del mismo,
  por lo que los shifts hacia la derecha serán aritméticos y se completará con ese msb a izquierda. Luego sabiendo que en C si algo = 0 da false 
  y si algo != 0 da true se lo puede aprovechar con el operador NOT lógico "!" ya que:
  Si x = 0 => !!x = false (1)
  Si x != 0 => !!x = true (0) 
  Juntando ambos resultados se logra obtener el valor correcto en cada caso.
  */
    int x1 = !!x;
    int x2 = x >> 31;
    return x1 | x2;
}
//3
/*
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1,
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {

  int suma = x + y;

  //Obtengo los bits mas significativos (signo) de 'x', 'y' e 'x+y'
  int bmsSuma = !!(suma >> 31);
  int bmsX = !!(x >> 31);
  int bmsY = !!(y >> 31);

  //Si signoX = signoY --> 1
  int signoX_Y_Iguales = !(bmsX ^ bmsY);

  //Si signoX = signoSuma --> 1
  int signoX_Suma_Iguales = !(bmsX ^ bmsSuma);

  /*
  S = Signo de 'x' igual al signo de la suma y al signo de 'y'
  s = 1 --> si 'x' tiene el mismo signo que la suma y que 'y'
  s = 0 --> 'x' difiere en un signo y no sabemos con cual (si con 'y' o con la suma)
  */
  int s = signoX_Y_Iguales & signoX_Suma_Iguales;


  return !(s ^ signoX_Y_Iguales);

  /*
  s = 0 --> No sabemos con qué signo difiere 'x'. Puede diferir con 'y', con la suma, o con ambos.

  Si el signo de 'x' era solo diferente al de 'y' --> !(s ^ signoX_Y_Iguales) = !(0 ^ 0)
  Me da 1, es decir, no hay overflow. Esto tiene sentido ya que si 'x' e 'y' difieren en signo, 
  no puedo tener overflow. No importa si el signo con la suma difiere. EPor lo tanto,
  este mismo caso me queda si 'x' difiere en signo con la suma también.

  Si el signo de 'x' era SOLO diferente al de la suma --> !(s ^ signoX_Y_Iguales) = !(0 ^ 1)
  Me da 0, es decir, hay overflow. Tiene sentido ya que si x tiene el mismo signo que 'y' pero
  la suma me da con otro signo, significa que hubo overflow
  */

}
/*
 * bitMask - Generate a mask consisting of all 1's
 *   lowbit and highbit
 *   Examples: bitMask(5,3) = 0x38
 *   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int bitMask(int highbit, int lowbit) {

    int mascara8Bits = 0xFF;
    int mascara32Bits = ~((~mascara8Bits) << 24);

    int limiteInferior = ~(mascara32Bits << lowbit);
    int limiteSuperiorMasUno = mascara32Bits << highbit;
    int highLimit = limiteSuperiorMasUno << 1;

    int pseudoRes = limiteInferior | highLimit;
    int res = ~pseudoRes;
    // La formula condensada es: ~(~(0xFFFFFFFF << lowbit) | ((0xFFFFFFFF << highbit) << 1))

  return res;
}
/*
 * conditional - same as x ? y : z
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
/*
Como en C si algo = 0 es false y si algo != 0 es true entonces lo que se pide implementar es el operador ternario donde se devuelve y si x es true
o z si x es false, al no poder hacer uso de ifs de nuevo como en la función sign se aprovecha el operador NOT lógico "!". En este caso hay que saber
si el x recibido es 0 o no para saber qué otro valor devolver entonces ese operador permite separar esos dos casos.

Con los desplazamientos realizados se logra que sea cual sea el valor obtenido con el NOT se coloca en todos los bits (considerando el desplazamiento
aritmético que se hace al poner ese valor en la primera posición y luego shiftear). Luego con un AND con z e y (negado en uno y no en el otro) se logra
quedarse solo con el valor de los dos que corresponda y al final con un OR sabiendo que uno es cero y el otro no se termina devolviendo el que no lo es
*/
  int x1 = !!x;
  int x2 = x1 << 31; //colocar el bit en la primera posición para que pase a ser el signo
  int x3 = x2 >> 31; //extender el bit de signo
  int y1 = x3 & y;
  int z1 = ~x3 & z;
  return y1 | z1; //se devuelve el valor que no es cero sea cual sea
}
/*
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  
  int minimo = 0x30;
  int maximo = 0x39;

  /*
  Necesito ver si la resta (x - 0x30) da negativa --> x < 0x30
  */
  int restaXconMinimo = x + (~minimo + 1);
  int signoRestaMinimo = restaXconMinimo >> 31;
  int esNegativoRestaMinimo = !(signoRestaMinimo); //0 si es negativo. 1 si es positivo o cero

  /*
  Necesito ver si x > 0x39 --> Si (0x39 - x) me da negativo. Mismo proceso de antes
  */

  int restaXconMaximo = maximo + (~x+1);
  int signoRestaMaximo = restaXconMaximo >> 31;
  int esNegativoRestaMaximo= !(signoRestaMaximo); //0 si es negativo. 1 si es positivo o cero


  /*
  Para que se cumpla: (x - 0x30) debe ser positivo o cero, y (0x39 - x) debe ser positivo o cero
  */
  return esNegativoRestaMinimo & esNegativoRestaMaximo;
}
/*
 * isGreater - if x > y  then return 1, else return 0
 *   Example: isGreater(4,5) = 0, isGreater(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isGreater(int x, int y) {

    int signoX = x >> 31;
    int signoY = y >> 31;
    int xPositivoYNegativo = signoY & (~signoX);
    int yPositivoXnegativo = signoX & (~signoY);

    int opuestoY = ~y + 1;
    int resta = x + opuestoY;
    int resultadoRestaDistintoDeCero = !(!(resta));
    int signoResta = resta >> 31;

    int signoopuestoY = opuestoY >> 31;
    int signosDeLasY = !((signoopuestoY ^ signoY) ^ !(y));

  return (xPositivoYNegativo | !(signoResta) | signosDeLasY) & resultadoRestaDistintoDeCero & !yPositivoXnegativo;
}
/*
 * replaceByte(x,n,c) - Replace byte n in x with c
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: replaceByte(0x12345678,1,0xab) = 0x1234ab78
 *   You can assume 0 <= n <= 3 and 0 <= c <= 255
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 3
 */
int replaceByte(int x, int n, int c) {
  /*
  La idea es, tal como dice la descripción, colocar el byte recibido en la posición recibida pisando el byte que estaba ahí antes. Entonces procediendo
  similar a otras funciones con una máscara apropiada se logra desplazar a la posición donde se tiene que colocar y "limpiar" el byte que estaba ahí antes,
  luego haciendo el desplazamiento en el byte recibido para moverlo a esa posición al final se combina todo para tener el número con el byte nuevo
  insertado en ese lugar.
  */
  int y = c << (n << 3); //se desplaza el byte a la posición a insertar
  int z = 0xFF;
  int z1 = z << (n << 3);
  int z2 = x & ~z1; //se limpia el lugar donde va a ir el nuevo byte
  return z2 | y; //se combinan ambas partes
}
//4
/*
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
  int signoX = x >> 31;
  int xorSigno = (signoX ^ x); //Si x es positivo nos queda igual. Sino, nos queda con los bits invertidos.
  /*
  Si X es positivo nos queda igual. Queremos devolver esto ya que el valor absoluto de un número positivo es 
  el número mismo. Pero si X es negativo, los bits se invirtieron. Cuando calculamos el complemento a 2, invertimos y 
  sumamos 1. Entonces, en estos casos solo necesito sumar 1: ¿Qué puedo hacer para sumar 1 si los bits nos quedaron invertidos
  o no sumar nada si el número nos quedo igual?
  Podemos restar el signo. Osea, sumar (~signo +1). Si el signo era positivo (0), al hacer (~signo+1) volvemos a obtener cero.
  Esto es lo que queríamos ya que si el numero era positivo queremos devolver el mismo número.
  Caso contrario, si el signo era negativo (1) terminaremos sumando 0....01 (Ya que invertir el signo 1 queda 000...0, y sumandole
  1 obtenemos 000...01)
  */
  int resultado = xorSigno + (~signoX + 1);
  return resultado;

}
/*
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int bang(int x) {
    //Divido en 2 partes iguales de cantidad de bits que me interesan y
    // hago un OR BINARIO para comparar los fragmentos que me interesen.
    //Entonces siempre que haya un 1, lo voy "arrastrando"

    int mask1Bit = 0x01;

    int dosBytesx = x | (x >> 16);
    int unBytex = dosBytesx | (dosBytesx >> 8);
    int cuatroBitsx = unBytex | (unBytex >> 4);
    int dosBitx = cuatroBitsx | (cuatroBitsx >> 2);
    int unBitx = dosBitx | (dosBitx >> 1);

    int res = (~unBitx) & mask1Bit;

  return res;
}
/*
 * isNonZero - Check whether x is nonzero using
 *              the legal operators except !
 *   Examples: isNonZero(3) = 1, isNonZero(0) = 0
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int isNonZero(int x) {
  /*
  Al no poder usar el operador NOT lógico para poder hacer el chequeo y devolver el valor correcto se puede pensar en que en la representación de complemento
  a 2 el único número que al negarlo y sumarle 1 me da el mismo número es el cero (esto considerando cual es el mínimo valor negativo posible que se puede
  representar), pero en todos los otros casos da algo distinto. En particular en esos casos al hacer las operaciones de esta función se logra obtener un 1 en
  el msb que luego con el shift al bit menos significativo y un AND con una máscara apropiada se devuelve el valor correcto ya que en ese caso x no era cero
  (pero esto no va a pasar cuando x sea cero porque en el msb quedará un 0 que se mantendrá al hacer el AND)
  */
  int y = 0x01;
  int z1 = ~x + 1;
  int z2 = z1 | x;
  int z3 = z2 >> 31;
  return z3 & y;
}
/*
 * logicalNeg - implement the ! operator, using all of
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int logicalNeg(int x) {
  /*

  La idea general sería descubir si X es cero o diferente de cero, para saber si devolver 1 o 0.

  Si un numero es diferente de cero, al hallar su complemento a 2, los bit mas significativos deberían ser diferentes o ambos 1.
  (Esto se deduce sabiendo que un numero sumado a su complemento a 2 SIEMPRE da cero)

  Al hacer un OR entre el numero y su CA2, si los bms son diferentes o ambos 1, el bms resultado siempre me debe dar 1.

  Si el numero es cero, su CA2 tambien es cero por lo tanto los bits de signo no me darán diferentes, y el OR
  me dará cero en el bit mas significativo. Es el unico caso en el que el OR entre los signos me da cero.

  Hago un shift >> 31 para obtener el signo del resultado del OR

  Como la operacion es NEG, si el numero es 0 quiero devolver 1, entonces simplemente le sumo 1 al signo. 
  Si el numero es diferente de cero, el signo será -1 por lo tanto tambien sumo 1.

  */

 int CA2 = ~x+1;

 int or = x | CA2;
 int signoOR = or >> 31;

 int resultado = signoOR+1;
 
 return resultado;
} 
//float
/*
 * floatAbsVal - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned floatAbsVal(unsigned uf) {

    unsigned mascara8Bits = 0xFF;
    unsigned mascara23Bits = ~((~mascara8Bits) << 15);

    unsigned mantisa = uf & mascara23Bits;
    unsigned ufUnsigned = uf & (~(0x01 << 31));

    if((ufUnsigned >> 23) == 0xFF) {

        if(mantisa) {
            return uf;
        }
    }

  return ufUnsigned;
}
/*
 * floatIsEqual - Compute f == g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 25
 *   Rating: 2
 */
int floatIsEqual(unsigned uf, unsigned ug) {
  /*
  Primero de todo hay que ver si alguno de los argumentos es Not a Number(NaN) y eso puede verse solo observando el exponente de un número (recordando las partes
  que conforman al número fraccionario en binario eso correspondería a los 8 bits que siguen al signo, que es el msb), si alguno de ellos es todo 1 es NaN y se 
  devuelve lo pedido para ese caso.

  Luego dejando de lado ese caso hay que comparar byte con byte para ver que efectivamente los números sean iguales, teniendo cuidado del caso +0 y -0 que se
  menciona también en la descripción ya que al trabajar con unsigned son dos formas de representar el mismo número pero que tienen que ser consideradas como
  iguales por eso mismo por lo que en la siguiente parte se opera para solucionar ese caso. Al final solo se devuelve la comparación
  */
    uint32_t d;
    uint32_t c;
    uint32_t c1;
    uint32_t z1 = 0x7F;
    uint32_t z2 = z1 << 24;
    uint32_t x1 = uf & z2;
    uint32_t y1 = ug & z2;
    uint32_t s1 = 0xFF;
    uint32_t s2 = s1 << 8;
    uint32_t z3 = z1 << 16;
    uint32_t s3 = z3 | s2 | s1;
    uint32_t x2 = uf & s3;
    uint32_t y2 = ug & s3;
    if ((x1 == z2) || (y1 == z2)) {
      if (x2 || y2) {
        return 0;
      }
    }
    d = 0x00;
    c = 0x01;
    c1 = c << 31;
    if ((uf == c1 && ug == d) || (uf == d && ug == c1)) {
      return 1;
    }
    return uf == ug;
}
/*
 * floatNegate - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned floatNegate(unsigned uf) {
  /*
  numeros de la forma: S-EXP-FRAC
  En precision simple:
    - S = 1 bit
    - EXP = 8 bits
    - FRAC = 23 bits
  */

  /*
  Para que sea NaN: exp = 1111...1, frac != 0000...0
  Necesito quedarme con los bits de fran y exp
    -Para quedarme con los bits de exp: 011111111000...0
    El primer cero para descartar el bit S, 8 unos para quedarme con los 8 bits de exp, y el resto cero para descartar
    los bits de frac

    -Para quedarme con los bits de frac: 0000000001111...1
    Primeros 9 bits en cero para descartar el bit S y los 8 bits de exp

  Obtengo 0x7F800000 y 0x7FFFFF
  */
  unsigned int mascaraExp = 0x7F800000;
  unsigned int mascaraFrac = 0x7FFFFF;
  unsigned int mascaraBitSigno = 0x80000000;

  /*
  Para saber si exp = 1111...1 hago (uf & mascara) y el numero resultante debe ser igual a la mascara
              mascara:   111111...00000
              uf:        xxxxxx...xxxxx
              
  Para saber si frac != 0000...0 hago uf & mascara y el resultado debe ser distinto de cero:
              mascara:   000000...1111111
              uf:        xxxxxx...xxxxxxx                          
  */
  unsigned int exp = uf & mascaraExp;
  unsigned int frac = uf & mascaraFrac;

  if ((exp == mascaraExp) && (frac != 0)) {
    return uf;
  }

  /*
  Si no es NaN solo devuelvo el primer bit invertido. Para eso uso una mascara que me obtenga el primer bit: 1000...000 
  (mascaraBitSigno)
  */

  return uf ^ mascaraBitSigno;
  
}
/*
 * floatIsLess - Compute f < g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 3
 */
int floatIsLess(unsigned uf, unsigned ug) {

    unsigned signoUF = 0;
    unsigned signoUG = 0;

    unsigned mascara8Bits = 0xFF;
    unsigned mascara23Bits = ~((~mascara8Bits) << 15);

    unsigned expUF = (uf >> 23) & mascara8Bits;
    unsigned expUG = (ug >> 23) & mascara8Bits;

    unsigned mantisaUF = uf & mascara23Bits;
    unsigned mantisaUG = ug & mascara23Bits;

    unsigned ufSinSigno = uf << 1;
    unsigned ugSinSigno = ug << 1;

    if (ufSinSigno == 0x00) {   // +0 y -0 pasan a ser +0
        uf = 0x00;
    }
    if (ugSinSigno == 0x00) {   // +0 y -0 pasan a ser +0
        ug = 0x00;
    }

    signoUF = uf >> 31;
    signoUG = ug >> 31;

    if (uf == ug) {
        return 0;
    }

    if (expUF == 0xFF) {    //Caso en que UF = NaN
        if (mantisaUF) {
            return 0;
        }
    }
    if (expUG == 0xFF) {    //Caso en que UG = NaN
        if (mantisaUG) {
            return 0;
        }
    }

    if (signoUF > signoUG) {
        return 1;
    }
    else if (signoUF < signoUG) {
        return 0;
    }
    else {         // Acá adentro significa que: signoUF == signoUG

        unsigned opuestoUF = ~(uf) + 1;
        unsigned resta = ug + opuestoUF;
        unsigned signoResta = resta >> 31;

        if (signoUG > signoResta) {
            return 0;
        }
        if (signoUG == signoResta) {
            return 1;
        }
    }
    return 0;
}
/*
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  /*
  La idea en esta función es hacer un fuerte uso de cómo son las partes en las que se divide el número fraccionario cuando está escrito en forma binaria: el
  signo, el exponente y la parte fraccionaria. Primero viendo el exponente se sabe ya si el número es NaN o infinito (cuando todos los bits son 1) y se 
  devuelve lo correspondiente para ese caso. Luego hay que ver el tema rangos por lo que todo lo que esté por encima de un valor máximo devuelve lo pedido por
  la función, y los que tengan exponente cero (los números desnormalizados) terminan devolviendo 0 por el redondeo que se hace ya que son los números que 
  están más cerca de ese número.

  Dejando de lado los casos especiales para considerar el caso de los valores normalizados hay que ver qué pasa con el valor del exponente E, el bias y la parte
  del exponente del número. Al ser una representación de precisión simple (lo aclara en la descripción) se conoce ya el bias que vale 127, luego se puede extraer
  la parte del exponente del número y calcular E = exp - bias que representa el exponente del número decimal original.

  Para hacer el redondeo correcto se ubica la parte fraccionaria del número (mantisa) al principio incluyendo el bit 1 en el msb que se omite por estar siempre, 
  después shiftear E veces a derecha si es positivo o a izquierda si es negativo quedando en el msb el número al que hay que redondear y el que hay que devolver 
  al final
  */
  int cero;
  int c;
  int bias;
  int biasC;
  int e;
  int s1;
  int s2;
  int s3;
  int s4;
  int s5;
  uint32_t m1;
  uint32_t m3;
  uint32_t m4;
  uint32_t m;
  uint32_t z1 = 0xFF;
  uint32_t z2 = z1 << 23;
  uint32_t x1 = uf & z2;
  uint32_t x2 = x1 >> 23;
  uint32_t min = 0x80;
  uint32_t tmin = min << 24;
  if (x2 == z1) {
    return tmin;
  }
  cero = 0x00;
  c = 0x01;
  if (x2 == 0) {
    return cero;
  }
  bias = 0x7F;
  biasC = ~bias + 1;
  e = x2 + biasC;
  if (e > 31) {
    return tmin;
  }
  if (e < -31) {
    return cero;
  }
  s1 = bias << 16;
  s2 = z1 << 8;
  s3 = s1 | s2 | s1;
  s4 = s3 << 7;
  s5 = min | s4;
  if (e >= 0) {
    m = s5 >> e;
  }
  if (e < 0) {
    int eC = ~e + 1;
    m = s5 << eC;
  }
  m1 = m & min;
  if(m1 == 0) {
    return cero;
  }
  m3 = uf >> 31;
  m4 = m3 & c;
  if (m4 == 0) {
    return c;
  }
  return ~cero;
}
/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatPower2(int x) {
  /*
  Declaramos variables:
    cero = 00000000000000000000000000000000
    inf = 01111111100000000000000000000000 = 0x7F800000

    exp = x + 127 ya que sabemos que en la representación de punto flotante tenemos 2^E, donde E = exp - bias
    En nuestro caso, E es el x que nos dan de dato, exp es la incógnita (lo que pondremos en el resultado)
    y bias es 127 ya que    BIAS = 2^(k-1)-1    con k = 8 (k = bits del campo exp --> 8 en precision simple.)

    ¿Cuál es el numero 'E' mas grande y mas chico que podemos tener para que sea posible la representación del numero 
    que nos piden?
    Pues, E = exp - bias  En 'exp' solo tenemos 8 bits, por lo que podemos representar 2^8 numeros, con max = 255
    El caso mas bajo es: exp = 0  pero lo usamos para representar el numero 0. Entonces el número más bajo que NO es cero es:
    E = 1 - 127 = -126
    Y caso mas alto sería exp = 255 pero este lo usamos para representar el infinito, entonces el numero mas alto posible
    que no es infinito es E = 254 - 127 = 127
  */
  unsigned int exp = x + 127;
  unsigned int cero = 0x00000000;
  unsigned int inf = 0x7F800000;
  int mascaraExp = 0x000;
  mascaraExp = mascaraExp | exp;
  mascaraExp = mascaraExp << 23; 

  if (x < -126) {
    return cero;
  } else if (x > 127) {
    return inf;
  }
  return mascaraExp;
}
