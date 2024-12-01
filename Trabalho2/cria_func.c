/* Davi Martignoni Barros 2311009 3WC */
/* Luiz Felipe Neves Batista 2311024 3WC */

#include <stdio.h>
#include <string.h>
#include "cria_func.h"

typedef unsigned char byte;

void cria_prologo(unsigned char codigo[], int *pos);
void primeiro_parametro(unsigned char codigo[], int *pos, DescParam param, int *param_atual);
void cria_call_na_func(unsigned char codigo[], int *pos, void *f);
void cria_final(unsigned char codigo[], int *pos);

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
 * Essa função receberá todos os parâmetros que possuírem OrigemValor = PARAM de acordo com params. Os outros serão escolhidos com base nas definições do params
 */
void cria_func(void *f, DescParam params[], int n, unsigned char codigo[])
{

    int pos = 0;         // posicao atual da escrita no vetor código
    int param_atual = 1; // indica qual parâmetro de codigo é o próximo a ser configurado (1 - %rdi, 2 - %rsi, 3 - %rdx)

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

void cria_prologo(unsigned char codigo[], int *pos)
{
    int n = 4; // tamanho do vetor prologo

    byte prologo[] = {
        0x55,             // pushq   %rbp
        0x48, 0x89, 0xe5, // movq    %rsp,%rbp
    };

    // para cada posição de prologo, adiciona o byte na posição adequada de codigo
    memcpy(codigo + (*pos), prologo, n);

    *pos += n;

    return;
}

void primeiro_parametro(unsigned char codigo[], int *pos, DescParam param, int *param_atual)
{
    switch (param.orig_val)
    {
    case PARAM:
        // nesse caso, %rdi / %esi já terá o valor correto, recebido pela função criada
        (*param_atual)++; // próximo parâmetro: %rsi SERÁ???? TALVEZ TENHA QUE NÃO ATUALIZAR, PRA QUE O segundo_parametro POSSA PEGAR DO RDI
        break;
    case FIX:
        if (param.tipo_val == INT_PAR)
        {
            int inteiro = param.valor.v_int;
            byte b1 = inteiro;
            byte b2 = inteiro >> 8;
            byte b3 = inteiro >> 16;
            byte b4 = inteiro >> 24;
            byte primeiro_param[] = {
                0x8b, 0x3c, 0x25, // movl para edi
                b1, b2, b3, b4    // bytes do inteiro
            };
            int n = 7;
            memcpy(codigo + (*pos), primeiro_param, n);
            *pos += n;
            (*param_atual)++;
        }
        else // parâmetro é pointer
        {
            void* ptr = param.valor.v_ptr;
            byte b1 = (long)ptr;
            byte b2 = (long)ptr >> 8;
            byte b3 = (long)ptr >> 16;
            byte b4 = (long)ptr >> 24;
            byte b5 = (long)ptr >> 32;
            byte b6 = (long)ptr >> 40;
            byte b7 = (long)ptr >> 48;
            byte b8 = (long)ptr >> 56;
            byte primeiro_param[] = {
                0x8b, 0x3c, 0x25, // movl para edi
                b1, b2, b3, b4    // bytes do inteiro
            };
            int n = 7;
            memcpy(codigo + (*pos), primeiro_param, n);
            *pos += n;
            (*param_atual)++;
        }

        break;
    case IND:
        break;
    }
    return;
}

void cria_call_na_func(unsigned char codigo[], int *pos, void *f)
{
    int n = 12; // tamanho do vetor call_na_func

    byte f1 = (long)f; // byte menos significativo de f
    // converte f para long, desloca 1 byte para a direita e guarda em f2
    // a atribuição faz um typecast implícito para byte
    byte f2 = (long)f >> 8;
    byte f3 = (long)f >> 16;
    byte f4 = (long)f >> 24;
    byte f5 = (long)f >> 32;
    byte f6 = (long)f >> 40;
    byte f7 = (long)f >> 48;
    byte f8 = (long)f >> 56; // byte mais significativo de f

    byte call_na_func[] = {
        0x48, 0xb8, f1, f2, f3, f4, f5, f6, f7, f8, // movabs   f, %rax
        0xff, 0xd0,                                 // call     *%rax
    };

    // para cada posição de call_na_func, adiciona o byte na posição adequada de codigo
    memcpy(codigo + (*pos), call_na_func, n);

    *pos += n;

    return;
}

void cria_final(unsigned char codigo[], int *pos)
{
    int n = 2; // tamanho do vetor final
    byte final[] = {
        0xc9, // leave
        0xc3  // ret
    };

    // para cada posição de final, adiciona o byte na posição adequada de codigo
    memcpy(codigo + (*pos), final, n);

    *pos += n;

    return;
}