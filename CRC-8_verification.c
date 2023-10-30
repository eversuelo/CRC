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
 * @brief Imprime un Byte en binario
 * @param b Byte a imprimir
 */
void printByte(Byte b)
{
    for (int i = 0; i < 8; i++)
    {
        printf("%d", (b & 0x80) ? 1 : 0);
        b <<= 1;
    }
    printf("\n");
}
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
Byte crc8_to_send(char *s, int n)
{
    Byte *msg_r = malloc(n * sizeof(Byte));
    printf("%s;\n%d\n", s, n);
    memcpy(msg_r, s, n);
    Byte crc = 0x00;     // Iniciamos en 0
    Byte crc_poly = 173; // Tiene que ser CRC
    int position = 0;    // Posición actual en la cadena
    crc = msg_r[position];
    Byte remainder = 0x00;
    int j = 0;
    while (position < n)
    {
        for (int i = 0; i < 8; i++)
        {

            // printf("CRC-P %d:%d:%d: %u\n", position, i, j, crc);
            if (crc >= 0X80)
            {
                crc = crc_poly ^ crc;
                printf("CRC(XOR Poly) %d:%d:%d: %u\n", position, i, j, crc);
                printf("CRC(XOR Poly)->pos:%d;CRC:", position);
                printByte(crc);
                printf(";poly:");
                printByte(crc_poly);
                printf("\n");
            }
            else
            {
                crc = crc ^ 0x00;
                printf("CRC(XOR Zero) %d:%d:%d: %u\n", position, i, j, crc);
                printf("CRC(XOR Poly)->pos:%d;CRC:", position);
                printByte(crc);
                printf(";poly:");
                printByte(crc_poly);
                printf("\n");
            }
            if (position == n - 2)
            {
                if (i < 7)
                {
                    copiaUnBit(&crc, &msg_r[position + 1]);
                }
            }
            else if (position <= n - 2)

            {
                copiaUnBit(&crc, &msg_r[position + 1]);
            }
            j++;
        }

        position++;
    }
    return crc;
}
Byte crc8_to_recive(char *s, int n)
{
    Byte *msg_r = malloc(n * sizeof(Byte));
    printf("%s;\n%d\n", s, n);
    memcpy(msg_r, s, n);
    Byte crc = 0x00;     // Iniciamos en 0
    Byte crc_poly = 173; // Tiene que ser CRC
    int position = 0;    // Posición actual en la cadena
    crc = msg_r[position];
    Byte remainder = 0x00;
    int j = 0;
    while (position < n)
    {
        for (int i = 0; i < 8; i++)
        {

            // printf("CRC-P %d:%d:%d: %u\n", position, i, j, crc);
            if (crc >= 0X80)
            {
                crc = crc_poly ^ crc;
                printf("CRC(XOR Poly) %d:%d:%d: %u\n", position, i, j, crc);
                printf("CRC(XOR Poly)->pos:%d;CRC:", position);
                printByte(crc);
                printf(";poly:");
                printByte(crc_poly);
                printf("\n");
            }
            else
            {
                crc = crc ^ 0x00;
                printf("CRC(XOR Zero) %d:%d:%d: %u\n", position, i, j, crc);
                printf("CRC(XOR Poly)->pos:%d;CRC:", position);
                printByte(crc);
                printf(";poly:");
                printByte(crc_poly);
                printf("\n");
            }
            if (position == n - 1)
            {
                if (i < 7)
                {
                    copiaUnBit(&crc, &msg_r[position + 1]);
                }
            }
            else if (position <= n - 1)

            {
                copiaUnBit(&crc, &msg_r[position + 1]);
            }
            j++;
        }

        position++;
    }
    return crc;
}
int main()
{
    Byte test_data[29] = "CRC es un tema interesante\0";
    Byte CRC = crc8_to_send(test_data, sizeof(test_data)); // Entregara un CRC con 8 digitos, solo necesitamos los primeros 7
    printf("CRC Final: %x\n", CRC);
    CRC <<= 1;
    test_data[sizeof(test_data) - 1] = CRC;
    printf("Verificando CRC\n");
    printf("CRC Final: %x\n", crc8_to_recive(test_data, sizeof(test_data)));
    return 0;
}
