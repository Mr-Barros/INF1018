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
    FILE *entrada = openFile("testeentrada.txt", "rb");
    FILE *saida = openFile("testesaida.txt", "wb");
    unsigned int inteiro;
    unsigned int invertido;
    while ((inteiro = fread(&inteiro, sizeof(unsigned int), 1, entrada)) == 1)
    {
        invertido = inverteOrdemByte(inteiro);
        fwrite(&invertido, sizeof(unsigned int), 1, saida);
    }

    // conversao de utf8 para utf32
    FILE *entrada_utf8 = openFile("entrada/utf8_peq.txt", "rb");
    FILE *saida_utf32 = openFile("saida/utf32_saida.txt", "wb");

    convUtf8p32(entrada_utf8, saida_utf32);

    fclose(entrada_utf8);
    fclose(saida_utf32);

    // conversao de utf32 para utf8
    // FILE *entrada_utf32 = openFile("entrada/utf32_peq_bom_ruim.txt", "rb");
    FILE *entrada_utf32 = openFile("entrada/utf32_peq.txt", "rb");
    // FILE *entrada_utf32 = openFile("entrada/utf32_peq_big_endian.txt", "rb");
    FILE *saida_utf8 = openFile("saida/utf8_saida.txt", "wb");

    convUtf32p8(entrada_utf32, saida_utf8);

    fclose(entrada_utf32);
    fclose(saida_utf8);

    return 0;
}