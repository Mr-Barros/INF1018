/* Davi Martignoni Barros 2311009 3WC */
/* Luiz Felipe Neves Batista 2311024 3WC */

#include "cria_func.h"

/**
 * cria uma função que amarra certos parâmetros de outra função, alterando o vetor "código" para 
 * que possua o código da nova função.
 * @param f ponteiro para a função original
 * @param params vetor de parâmetros de tipo DescParam
 * @param n número de parâmetros passados
 * @param codigo o código, que deve vir vazio
 */
void cria_func(void *f, DescParam params[], int n, unsigned char codigo[])
{
    unsigned char codigo_vetor[] = {
        0x55,               // pushq   %rbp
        0x48, 0x89, 0xe5,   // movq    %rsp,%rbp
        0xff, 0xd7,         // 
    }; 
    for (int i = 0; i < 4; i++)
    {
        codigo[i] = prologo[i];
    }

    unsigned char call_na_func[] = {};

    for (int i = 4; i < 6; i++)
    {
        codigo[i] = call_na_func[i];
    }

    unsigned char final[] = {0xc9, 0xc3};
    for (int i = 0; i < 2; i++)
    {
        codigo[i] = prologo[i];
    }
    return;
}