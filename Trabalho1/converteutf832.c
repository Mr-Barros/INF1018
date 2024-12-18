/* Davi Martignoni Barros 2311009 3WC */
/* Luiz Felipe Neves Batista 2311024 3WC */

#include <stdio.h>
#include "converteutf832.h"

unsigned int inverteOrdemByte(unsigned int i)
{
    unsigned int inv = (i & 0xFF) << 24;
    inv |= (i & 0xFF00) << 8;
    inv |= (i & 0xFF0000) >> 8;
    inv |= (i & 0xFF000000) >> 24;
    return inv;
}

int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida)
{
    unsigned int c1, c2, c3, c4, cout;
    int bom = 0x0000FEFF;
    fwrite(&bom, sizeof(bom), 1, arquivo_saida);
    if (ferror(arquivo_saida))
    {
        fprintf(stderr, "Erro de gravacao");
        return -1;
    }

    while ((c1 = (unsigned int)fgetc(arquivo_entrada)) != EOF)
    {
        if (ferror(arquivo_entrada))
        {
            fprintf(stderr, "Erro de leitura");
            return -1;
        }
        cout = 0;
        if (!(c1 & 0b10000000))
        { // 1 byte
            cout = c1;
        }
        else if ((c1 & 0b11100000) == 0b11000000)
        { // 2 bytes

            c2 = (unsigned int)fgetc(arquivo_entrada);
            if (ferror(arquivo_entrada))
            {
                fprintf(stderr, "Erro de leitura");
                return -1;
            }

            cout = (0b00011111 & c1) << 6;
            cout |= (0b00111111 & c2);
        }
        else if ((c1 & 0b11110000) == 0b11100000)
        { // 3 bytes

            c2 = (unsigned int)fgetc(arquivo_entrada);
            if (ferror(arquivo_entrada))
            {
                fprintf(stderr, "Erro de leitura");
                return -1;
            }
            c3 = (unsigned int)fgetc(arquivo_entrada);
            if (ferror(arquivo_entrada))
            {
                fprintf(stderr, "Erro de leitura");
                return -1;
            }

            cout = (0b00001111 & c1) << 12;
            cout |= (0b00111111 & c2) << 6;
            cout |= (0b00111111 & c3);
        }
        else
        { // 4 bytes

            c2 = (unsigned int)fgetc(arquivo_entrada);
            if (ferror(arquivo_entrada))
            {
                fprintf(stderr, "Erro de leitura");
                return -1;
            }
            c3 = (unsigned int)fgetc(arquivo_entrada);
            if (ferror(arquivo_entrada))
            {
                fprintf(stderr, "Erro de leitura");
                return -1;
            }
            c4 = (unsigned int)fgetc(arquivo_entrada);
            if (ferror(arquivo_entrada))
            {
                fprintf(stderr, "Erro de leitura");
                return -1;
            }

            cout = (0b00000111 & c1) << 18;
            cout |= (0b00111111 & c2) << 12;
            cout |= (0b00111111 & c3) << 6;
            cout |= (0b00111111 & c4);
        }
        fwrite(&cout, sizeof(cout), 1, arquivo_saida);
        if (ferror(arquivo_saida))
        {
            fprintf(stderr, "Erro de gravacao");
            return -1;
        }
    }

    return 0;
}

int convUtf32p8(FILE *arquivo_entrada, FILE *arquivo_saida)
{
    unsigned int cin;
    unsigned char c1, c2, c3, c4;
    unsigned int bom;
    fread(&bom, sizeof(unsigned int), 1, arquivo_entrada);
    if (ferror(arquivo_entrada)) 
    {
        fprintf(stderr, "Erro de leitura\n");
        return -1;
    }
    int big_endian, little_endian;
    big_endian = (bom == 0xFFFE0000);
    little_endian = (bom == 0x0000FEFF);
    if (!(big_endian || little_endian))
    {
        fprintf(stderr, "BOM invalido!\n");
        return -1;
    }

    while ((fread(&cin, sizeof(unsigned int), 1, arquivo_entrada)) == 1)
    {
        if (ferror(arquivo_entrada))
        {
            fprintf(stderr, "Erro de leitura");
            return -1;
        }
        if (big_endian)
        {
            cin = inverteOrdemByte(cin);
        }
        if (cin < 0x80)
        { // 1 byte
            c1 = (unsigned char)cin;
            fputc(c1, arquivo_saida);
            if (ferror(arquivo_saida))
            {
                fprintf(stderr, "Erro de gravacao\n");
                return -1;
            }
        }
        else if (cin < 0x800)
        { // 2 bytes

            c2 = (cin >> 6) | 0b11000000;
            c1 = (cin & 0b00111111) | 0b10000000;
            fputc(c2, arquivo_saida);
            if (ferror(arquivo_saida))
            {
                fprintf(stderr, "Erro de gravacao\n");
                return -1;
            }
            fputc(c1, arquivo_saida);
            if (ferror(arquivo_saida))
            {
                fprintf(stderr, "Erro de gravacao\n");
                return -1;
            }
        }
        else if (cin < 0x10000)
        { // 3 bytes
            c3 = (cin >> 12) | 0b11100000;
            c2 = ((cin >> 6) & 0b00111111) | 0b10000000;
            c1 = (cin & 0b00111111) | 0b10000000;
            fputc(c3, arquivo_saida);
            if (ferror(arquivo_saida))
            {
                fprintf(stderr, "Erro de gravacao\n");
                return -1;
            }
            fputc(c2, arquivo_saida);
            if (ferror(arquivo_saida))
            {
                fprintf(stderr, "Erro de gravacao\n");
                return -1;
            }
            fputc(c1, arquivo_saida);
            if (ferror(arquivo_saida))
            {
                fprintf(stderr, "Erro de gravacao\n");
                return -1;
            }
        }
        else
        { // 4 bytes
            c4 = (cin >> 18) | 0b11110000;
            c3 = ((cin >> 12) & 0b00111111) | 0b10000000;
            c2 = ((cin >> 6) & 0b00111111) | 0b10000000;
            c1 = (cin & 0b00111111) | 0b10000000;
            fputc(c4, arquivo_saida);
            if (ferror(arquivo_saida))
            {
                fprintf(stderr, "Erro de gravacao\n");
                return -1;
            }
            fputc(c3, arquivo_saida);
            if (ferror(arquivo_saida))
            {
                fprintf(stderr, "Erro de gravacao\n");
                return -1;
            }
            fputc(c2, arquivo_saida);
            if (ferror(arquivo_saida))
            {
                fprintf(stderr, "Erro de gravacao\n");
                return -1;
            }
            fputc(c1, arquivo_saida);
            if (ferror(arquivo_saida))
            {
                fprintf(stderr, "Erro de gravacao\n");
                return -1;
            }
        }
    }

    return 0;
}