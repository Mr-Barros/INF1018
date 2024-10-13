/* Davi Martignoni Barros 2311009 3WC */
/* Luiz Felipe Neves Batista 2311024 3WC */

#include <stdio.h>
#include <stdlib.h>
#include "converteutf832.h"

FILE *openFile(char *filename, char *mode)
{
    FILE *file = fopen(filename, mode);
    if (!file)
    {
        printf("Failed to open %s", filename);
        exit(1);
    }
    return file;
}

int main(void)
{
    int status;
    // FILE *entrada = openFile("testeentrada.txt", "rb");
    // FILE *saida = openFile("testesaida.txt", "wb");
    // unsigned int inteiro;
    // unsigned int invertido;
    // while ((inteiro = fread(&inteiro, sizeof(unsigned int), 1, entrada)) == 1)
    // {
    //     invertido = inverteOrdemByte(inteiro);
    //     fwrite(&invertido, sizeof(unsigned int), 1, saida);
    // }

    // conversao de utf8 para utf32
    FILE *entrada_utf8 = openFile("entrada/utf8_peq.txt", "rb");
    FILE *saida_utf32 = openFile("saida/utf32_saida.txt", "wb");

    status = convUtf8p32(entrada_utf8, saida_utf32);
    switch (status)
    {
    case 0:
        printf("Conversao de utf8 para utf32 bem-sucedida\n");
        break;
    case -1:
        printf("Erro na conversao de utf8 para utf32\n");
        break;
    default:
        printf("Codigo invalido na conversao de utf8 para utf32\n");
        break;
    }

    fclose(entrada_utf8);
    fclose(saida_utf32);

    // conversao de utf32 para utf8
    // FILE *entrada_utf32 = openFile("entrada/utf32_peq_bom_ruim.txt", "rb");
    FILE *entrada_utf32 = openFile("entrada/utf32_peq.txt", "rb");
    FILE *saida_utf8 = openFile("saida/utf8_saida.txt", "wb");

    status = convUtf32p8(entrada_utf32, saida_utf8);
    switch (status)
    {
    case 0:
        printf("Conversao de utf32 para utf8 (little endian) bem-sucedida\n");
        break;
    case -1:
        printf("Erro na conversao de utf32 para utf8 (little endian)\n");
        break;
    default:
        printf("Codigo invalido na conversao de utf32 para utf8 (little endian)\n");
        break;
    }

    fclose(entrada_utf32);
    fclose(saida_utf8);

    // conversao de utf32 para utf8 (big endian)
    FILE *entrada_utf32_be = openFile("entrada/utf32_peq_big_endian.txt", "rb");
    FILE *saida_utf8_be = openFile("saida/utf8_saida_big_endian.txt", "wb");

    status = convUtf32p8(entrada_utf32_be, saida_utf8_be);
    switch (status)
    {
    case 0:
        printf("Conversao de utf32 para utf8 (big endian) bem-sucedida\n");
        break;
    case -1:
        printf("Erro na conversao de utf32 para utf8 (big endian)\n");
        break;
    default:
        printf("Codigo invalido na conversao de utf32 para utf8 (big endian)\n");
        break;
    }

    fclose(entrada_utf32_be);
    fclose(saida_utf8_be);

    return 0;
}