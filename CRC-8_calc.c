/**
 * Jose Everardo Torres Orozco
 * Calculo del CRC
 * con Polinomio = 10101101 = 173(Dec)=0xad(Hex)
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
typedef unsigned char Byte;
/**
 * Copia un Bit y hace shift left a la fuente, solo se puede usar 8 veces debido a que son el número de bits en un unsigned char. La función es necesaria para CRC
 * @param *source La fuente de donde se obtendra el primer Bit de Izquierda a derecha
 * @param *dest Donde se almacenara el primer Bit copiad de *source
 */
Byte copiaUnBit(Byte *dest, Byte *source)
{
    *dest = *dest << 1;  // Recorremos una posici+on que es la que necesitamos co´´iar
    if (*source >= 0x80) // Si la parte más alta es la signficativa copiala en dest, si no dejala como estaba(un 0)
        *dest = *dest | 0X001;
    *source = *source << 0X001; // RECORRE el bit copiado para que ya no se vuelva a copiar.
    // printf("CRC-c %u\n", *dest);
}
/**
 * * Calculo de CRC, es necesario que la cadena a calcular tenga un caracter adicional en 0s que es donde se almacenara el residuo.
 * @param s Cadena a sacar CRC
 * @param n  Longitud de cadena
 * @param print Imprimir Operaciones del CRC 0-false 1-true
*/
Byte crc8(char *s, int n, int print)
{
    Byte *msg_r = malloc(n * sizeof(Byte)); // Cadena copia de la cadena original.
    int position_rcv = n - 2;               // Numero de Posiciones a recorrer. n-1 es el remainder n-2 es el ultimo caracter de la cadena.
    printf("%s;\n%d\n", s, n);
    memcpy(msg_r, s, n);
    Byte crc = 0x00;       // Iniciamos en 0 el CRC
    Byte crc_poly = 173;   // Tiene que ser el CRC polinomio
    int position = 0;      // Posición actual en la cadena
    crc = msg_r[position]; // Copiamos el primer byte a crc
    int j = 0;
    while (position < n) // Recorremos todos los caracteres
    {
        for (int i = 0; i < 8; i++) // Por cada caracter hacemos la operación del tamaño Byte
        {
            if (crc >= 0X80) // Si la posición más singificativa es 1 hacemos CRC con el polinomio
            {
                crc = crc_poly ^ crc;
                if (print)
                    printf("CRC(XOR Poly) %d:%d:%d: %u\n", position, i, j, crc);
            }
            else // Si no hacemos CRC con 0x0
            {
                crc = crc ^ 0x00;
                if (print)
                    printf("CRC(XOR Poly) %d:%d:%d: %u\n", position, i, j, crc);
            }
            if (position == position_rcv) // Si estamos recorriendo el ultimo caracter
            {
                if (i < 7) // Si estamos dentro de las ultimas posiciones copia un bit
                {
                    copiaUnBit(&crc, &msg_r[position + 1]);
                }
                // Si llegamos a la posición final no copies nada
            }
            else if (position <= position_rcv)          // Si estamos recorriendo caracteres que no sean el ultimo
                copiaUnBit(&crc, &msg_r[position + 1]); // Copia un Bit.

            j++; // Contador general de operacioens.
        }

        position++; // Modificador de posición.
    }
    printf("CRC Final-DEC: %u\n", crc);
    printf("CRC Final-Hex: %x\n", crc);
    //Imprimir el CRC en binario
    printf("CRC Final-Bin: ");
    for (int i = 0; i < 8; i++)
    {
        if (crc >= 0X80)
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
        crc = crc << 1;
    }
    printf("\n");
    
    return crc;
}
int main()
{
    Byte test_data[37]="Que tranza chavales, ya quedo el CRC\0";
    Byte CRC = crc8(test_data, sizeof(test_data), 0); // Entregara un CRC con 8 digitos, solo necesitamos los primeros 7
    CRC <<= 1;
    test_data[sizeof(test_data) - 1] = CRC;
    crc8(test_data, sizeof(test_data), 0);
    return 0;
}
