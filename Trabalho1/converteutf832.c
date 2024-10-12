/* Davi Martignoni Barros 2311009 3WC */
/* Luiz Felipe Neves Batista 2311024 3WC */

#include <stdio.h>
#include <stdlib.h>
#include "converteutf832.h"
#define BOM 0x0000FEFF

int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida)
{
    unsigned int c1, c2, c3, c4, cout = 0;
    int bom = BOM;
    fwrite(&bom, sizeof(bom), 1, arquivo_saida);

    while ((c1 = (unsigned int)fgetc(arquivo_entrada)) != EOF)
    {
        cout = 0;
        if (!(c1 & 0b10000000))
        { // 1 byte

            cout = c1;
        }
        else if (!(c1 & 0b11000000))
        { // 2 bytes

            c2 = (unsigned int)fgetc(arquivo_entrada);

            cout = (0b00011111 & c1) << 6;
            cout |= (0b00111111 & c2);
        }
        else if (!(c1 & 0b11100000))
        { // 3 bytes

            c2 = (unsigned int)fgetc(arquivo_entrada);
            c3 = (unsigned int)fgetc(arquivo_entrada);

            cout = (0b00001111 & c1) << 12;
            cout |= (0b00111111 & c2) << 6;
            cout |= (0b00111111 & c3);
        }
        else
        { // 4 bytes

            c2 = (unsigned int)fgetc(arquivo_entrada);
            c3 = (unsigned int)fgetc(arquivo_entrada);
            c4 = (unsigned int)fgetc(arquivo_entrada);

            cout = (0b00000111 & c1) << 18;
            cout |= (0b00111111 & c2) << 12;
            cout |= (0b00111111 & c3) << 6;
            cout |= (0b00111111 & c4);
        }
        fwrite(&cout, sizeof(cout), 1, arquivo_saida);
    }

    return 0;
}

int convUtf32p8(FILE *arquivo_entrada, FILE *arquivo_saida)
{
    unsigned int cin;
    char c1, c2, c3, c4;
    unsigned int bom;
    fread(&bom, sizeof(unsigned int), 1, arquivo_entrada);

    // primeiro caracter
    fread(&cin, sizeof(unsigned int), 1, arquivo_entrada);
    if (cin < 0x80)
    { // 1 byte
        c1 = (char)cin;
    }
    else if (cin < 0x800)
    { // 2 bytes
        c1 = (char)(0x3f & cin);
        // c2 = (char) ()
    }
    else if (cin < 0x10000)
    { // 3 bytes
    }
    else
    { // 4 bytes
    }

    return 1;
}