#ifndef CONVERTEUTF832
#define CONVERTEUTF832

#include <stdio.h>

int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida);

int convUtf32p8(FILE *arquivo_entrada, FILE *arquivo_saida);

unsigned int inverteOrdemByte(unsigned int i);

#endif