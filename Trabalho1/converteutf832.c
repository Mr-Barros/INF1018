/* Davi Martignoni Barros 2311009 3WC */
/* Luiz Felipe Neves Batista 2311024 3WC */

#include <stdio.h>
#include <stdlib.h>
#define BOM 0xfeff

int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida) {
    unsigned int c1, c2, c3, c4, cout = 0;

    fputc(BOM, arquivo_saida);
    
    // primeiro caracter
    c1 = (unsigned int) fgetc(arquivo_entrada);
    unsigned int cout = 0;
    if (!(c1 & 0x80)) { // 1 byte

        cout = c1;

    } else if (!(c1 & 0x20)) { // 2 bytes

        c2 = (unsigned int) fgetc(arquivo_entrada);

        cout = (0x1f & c1) << 6;
        cout |= (0x3f & c2);

    } else if (!(c1 & 0x10)) { // 3 bytes

        char c2 = (unsigned int) fgetc(arquivo_entrada);
        char c3 = (unsigned int) fgetc(arquivo_entrada);

        cout = (0x0f & c1) << 12;
        cout |= (0x3f & c2) << 6;
        cout |= (0x3f & c3);

    } else { // 4 bytes

        char c2 = (unsigned int) fgetc(arquivo_entrada);
        char c3 = (unsigned int) fgetc(arquivo_entrada);
        char c4 = (unsigned int) fgetc(arquivo_entrada);

        cout = (0x07 & c1) << 18;
        cout |= (0x3f & c2) << 12;
        cout |= (0x3f & c3) << 6;
        cout |= (0x3f & c4);

    }
    fputc(cout, arquivo_saida);
    return 0;
}

int convUtf32p8(FILE *arquivo_entrada, FILE *arquivo_saida) {
    unsigned int cin;
    char c1, c2, c3, c4;
    unsigned int bom;
    fread(&bom, sizeof (unsigned int), 1, arquivo_entrada);    

    // primeiro caracter
    fread(&cin, sizeof (unsigned int), 1, arquivo_entrada);
    if (cin < 0x80) { // 1 byte
        c1 = (char) cin;
    } else if (cin < 0x800) { // 2 bytes
        c1 = (char) (0x3f & cin);
        // c2 = (char) ()
    } else if (cin < 0x10000) { // 3 bytes

    } else { // 4 bytes

    }

    return 1;
}