/* Davi Martignoni Barros 2311009 3WC */
/* Luiz Felipe Neves Batista 2311024 3WC */

#include "cria_func.h"

/**
 * cria uma função que amarra certos parâmetros de outra função, alterando o vetor "código" para
 * que possua o código da nova função.
 * @param f ponteiro para a função original
 * @param params vetor de parâmetros de tipo DescParam. Deve conter um elemento do vetor para cada parâmetro na função original, para especificar como
 * esse parâmetro será escolhido: simplesmente repassado à função original, determinado por uma constante, ou obtido a partir de uma variável
 * em tempo de execução.
 * @param n número de parâmetros no vetor params
 * @param codigo o código, que deve vir vazio. Será escrito nesse vetor uma sequência de bytes que correspondem a uma função em assembly.
 * Deve ser feito um type cast nesse vetor para que seja reconhecido como um ponteiro para função.
 * Essa função receberá todos os parâmetros que possuírem OrigemValor = PARAM de acordo com DescParam
 */
void cria_func(void *f, DescParam params[], int n, unsigned char codigo[])
{
    unsigned char prologo[] = {
        0x55,             // pushq   %rbp
        0x48, 0x89, 0xe5, // movq    %rsp,%rbp

    };
    for (int i = 0; i < 4; i++)
    {
        codigo[i] = prologo[i];
    }

    unsigned char call_na_func[] = {
        0xff, 0xd7, // call *%rdi
    };

    for (int i = 0; i < 2; i++)
    {
        codigo[i+4] = call_na_func[i];
    }

    unsigned char final[] = {
        0xc9, // leave
        0xc3  // ret
    };
    for (int i = 0; i < 2; i++)
    {
        codigo[i+6] = final[i];
    }

    return;
}