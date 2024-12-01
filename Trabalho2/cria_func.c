/* Davi Martignoni Barros 2311009 3WC */
/* Luiz Felipe Neves Batista 2311024 3WC */

#include <stdio.h>
#include "cria_func.h"

typedef unsigned char byte;

void cria_prologo(unsigned char codigo[], int* pos);
void primeiro_parametro(unsigned char codigo[], int* pos, DescParam param, int* param_atual);
void cria_call_na_func(unsigned char codigo[], int* pos, void* f);
void cria_final(unsigned char codigo[], int* pos);

/**
 * @brief Cria uma função que amarra certos parâmetros de outra função, alterando o vetor "código" para
 * que possua o código da nova função.
 * @param f Ponteiro para a função original
 * @param params Vetor de parâmetros de tipo DescParam. Deve conter um elemento do vetor para cada parâmetro na função original, 
 * para especificar como esse parâmetro será escolhido: simplesmente repassado à função original, determinado por uma constante, 
 * ou obtido a partir de uma variável em tempo de execução.
 * @param n Número de parâmetros no vetor params
 * @param codigo O código, que deve vir vazio. Será escrito nesse vetor uma sequência de bytes que correspondem a uma função em assembly.
 * Deve ser feito um type cast nesse vetor para que seja reconhecido como um ponteiro para função.
 * Essa função receberá todos os parâmetros que possuírem OrigemValor = PARAM de acordo com DescParam
 */
void cria_func(void *f, DescParam params[], int n, unsigned char codigo[])
{
    
    int pos = 0;            // posicao atual da escrita no vetor código
    int param_atual = 1;    // indica qual parâmetro de codigo é o próximo a ser configurado (1 - %rdi, 2 - %rsi, 3 - %rdx)

    cria_prologo(codigo, &pos);

    // params sempre tem pelo menos 1 parâmetro
    primeiro_parametro(codigo, &pos, params[0], &param_atual);

    cria_call_na_func(codigo, &pos, f);

    cria_final(codigo, &pos);

    for (int i = 0; i < pos; i++)
    {
        fprintf(stderr, "%02X ", codigo[i]);
    }

    return;
}

void cria_prologo(unsigned char codigo[], int* pos)
{
    int n = 4;              // tamanho do vetor prologo

    byte prologo[] = {
        0x55,               // pushq   %rbp
        0x48, 0x89, 0xe5,   // movq    %rsp,%rbp
    };
    
    // para cada posição de prologo, adiciona o byte na posição adequada de codigo
    for (int i = 0; i < n; i++)
    {
        codigo[i + (*pos)] = prologo[i];
    }

    *pos += n;

    return;
}

void primeiro_parametro(unsigned char codigo[], int* pos, DescParam param, int* param_atual)
{
    switch (param.orig_val)
    {
    case PARAM:
        // nesse caso, %rdi / %esi já terá o valor correto, recebido pela função criada
        (*param_atual)++;   // próximo parâmetro: %rsi
        break;
    case FIX:
        break;
    case IND:
    }
    return;
}

void cria_call_na_func(unsigned char codigo[], int* pos, void* f)
{
    int n = 12;             // tamanho do vetor call_na_func

    byte f1 = (byte) f;     // byte menos significativo de f
    // converte f para long, desloca 1 byte para a direita e guarda em f2
    // a atribuição faz um typecast implícito para byte
    byte f2 = (long) f >> 8;    
    byte f3 = (long) f >> 16;
    byte f4 = (long) f >> 24;
    byte f5 = (long) f >> 32;
    byte f6 = (long) f >> 40;
    byte f7 = (long) f >> 48;
    byte f8 = (long) f >> 56;   // byte mais significativo de f

    byte call_na_func[] = {
        0x48, 0xb8, f1, f2, f3, f4, f5, f6, f7, f8,	// movabs   f, %rax
        0xff, 0xd0,                                 // call     *%rax
    };
    
    // para cada posição de call_na_func, adiciona o byte na posição adequada de codigo
    for (int i = 0; i < n; i++)
    {
        codigo[i + (*pos)] = call_na_func[i];
    }

    *pos += n;

    return;
}

void cria_final(unsigned char codigo[], int* pos)
{
    int n = 2;              // tamanho do vetor final
    byte final[] = {
        0xc9,   // leave
        0xc3    // ret
    };
    
    // para cada posição de final, adiciona o byte na posição adequada de codigo
    for (int i = 0; i < n; i++)
    {
        codigo[i + (*pos)] = final[i];
    }

    *pos += n;

    return;
}