/* Davi Martignoni Barros 2311009 3WC */
/* Luiz Felipe Neves Batista 2311024 3WC */

#include <stdio.h>
#include <string.h>
#include "cria_func.h"

typedef unsigned char byte;

/// @brief Cria uma função que amarra certos parâmetros de outra função, alterando o vetor "código" para
/// que possua o código da nova função.
/// @param f Ponteiro para a função original
/// @param params Vetor de parâmetros de tipo DescParam. Deve conter um elemento do vetor para cada parâmetro na função original,
/// para especificar como esse parâmetro será escolhido: simplesmente repassado à função original, determinado por uma constante,
/// ou obtido a partir de uma variável em tempo de execução.
/// @param n Número de parâmetros no vetor params
/// @param codigo O código, que deve vir vazio. Será escrito nesse vetor uma sequência de bytes que correspondem a uma função em assembly.
/// Deve ser feito um type cast nesse vetor para que seja reconhecido como um ponteiro para função.
/// Essa função receberá todos os parâmetros que possuírem OrigemValor = PARAM de acordo com params. Os outros serão escolhidos com base nas definições do params
void cria_func(void *f, DescParam params[], int n, unsigned char codigo[]);


// FUNÇÕES AUXILIARES

/// @brief Escreve em codigo as instruções que criam o registro de ativação da função gerada
/// @param codigo Vetor de bytes correspondente ao código da função criada
/// @param pos A posição relativa ao código na qual as novas instruções serão escritas
void cria_prologo(unsigned char codigo[], int *pos);

/// @brief Adiciona ao código as instruções referentes a um parâmetro da função original
/// @param codigo Vetor de bytes correspondente ao código da função criada
/// @param pos A posição relativa ao código na qual as novas instruções serão escritas
/// @param param um ponteiro apontando para o DescParam referente a esse parâmetro da função original
/// @param reg_leitura 1 a 3: O registrador do qual será lido o valor, caso o parâmetro seja do tipo PARAM
/// @param reg_escrita 1 a 3: O registrador no qual será guardado o valor
void guarda_parametro(unsigned char codigo[], int *pos, DescParam *param, int *reg_leitura, int *reg_escrita);

/// @brief Escreve em codigo as instruções para colocar os valores salvos em %r8, %r9 e %r10 nos parâmetros da função original
/// @param codigo Vetor de bytes correspondente ao código da função criada
/// @param pos A posição relativa ao código na qual as novas instruções serão escritas
void prepara_parametros(unsigned char codigo[], int *pos);

/// @brief Escreve em codigo as instruções que chamam a função f
/// @param codigo Vetor de bytes correspondente ao código da função criada
/// @param pos A posição relativa ao código na qual as novas instruções serão escritas
/// @param f O endereço da função original
void cria_call_na_func(unsigned char codigo[], int *pos, void *f);

/// @brief Escreve em codigo as instruções responsáveis por colapsar o RA e retornar da função
/// @param codigo Vetor de bytes correspondente ao código da função criada
/// @param pos A posição relativa ao código na qual as novas instruções serão escritas
void cria_final(unsigned char codigo[], int *pos);


void cria_func(void *f, DescParam params[], int n, unsigned char codigo[])
{
    int pos = 0;         // posicao atual da escrita no vetor código
    int reg_leitura = 1; // indica o registrador a ser lido quando o parametro for do tipo PARAM (1 - %rdi, 2 - %rsi, 3 - %rdx)
    int reg_escrita = 1; // indica o registrador que vai guardar o parâmetro respectivo da função original (1 - %r8, 2 - %r9, 3 - %r10)

    cria_prologo(codigo, &pos);

    for (int i = 0; i < n; i++)
    {
        guarda_parametro(codigo, &pos, &params[i], &reg_leitura, &reg_escrita);
    }

    prepara_parametros(codigo, &pos);

    cria_call_na_func(codigo, &pos, f);

    cria_final(codigo, &pos);

    return;
}

void cria_prologo(unsigned char codigo[], int *pos)
{
    int n = 4; // tamanho do vetor prologo

    byte prologo[] = {
        0x55,             // pushq   %rbp
        0x48, 0x89, 0xe5, // movq    %rsp, %rbp
    };

    // adiciona os n bytes de prologo na posição adequada de codigo
    memcpy(codigo + (*pos), prologo, n);

    // soma o tamanho do vetor à posição de escrita
    *pos += n;

    return;
}

void guarda_parametro(unsigned char codigo[], int *pos, DescParam* param, int *reg_leitura, int *reg_escrita)
{
    int n;      // tamanho do vetor de instruções a ser adicionado em codigo
    byte reg;   // opcode referente ao registrador de escrita, usado nos casos FIX e IND 
    switch (param->orig_val) // possíveis origens do parâmetro
    {
    case PARAM:
        // Vamos mover o valor que está no registrador de leitura para o registrador de escrita, evitando mexer nos parâmetros
        // Para isso, definimos combinacao_regs a depender de quais são os dois registradores
        // Note que não precisamos testar tipo_val, pois passar o registrador completo já passa o registrador de int
        byte combinacao_regs;
        if ((*reg_leitura == 1) && (*reg_escrita == 1))
        {
            combinacao_regs = 0xf8;     // mov      %rdi, %r8
        }
        else if ((*reg_leitura == 2) && (*reg_escrita == 2)) 
        {
            combinacao_regs = 0xf1;     // mov      %rsi, %r9
        }
        else if ((*reg_leitura == 3) && (*reg_escrita == 3)) 
        {
            combinacao_regs = 0xd2;     // mov      %rdx, %r10
        }
        else if ((*reg_leitura == 1) && (*reg_escrita == 2))
        {
            combinacao_regs = 0xf9;     // mov      %rdi, %r9
        }
        else if ((*reg_leitura == 1) && (*reg_escrita == 3))
        {
            combinacao_regs = 0xfa;     // mov      %rdi, %r9
        }
        else if ((*reg_leitura == 2) && (*reg_escrita == 3))
        {
            combinacao_regs = 0xf2;     // mov      %rsi, %r10
        }

        byte param_param[] = { // param, param param param param paraaaaam pararararam
            0x49, 0x89, combinacao_regs // movq     reg_leitura, reg_escrita
        };
        n = 3;
        memcpy(codigo + (*pos), param_param, n);
        
        (*reg_leitura)++;
        break;
    case FIX:
        // definimos o reg (opcode referente ao registrador, na instrução mov abaixo) a depender do reg_escrita
        if (*reg_escrita == 1)
        {
            reg = 0xb8;     // %r8
        }
        else if (*reg_escrita == 2)
        {
            reg = 0xb9;     // %r9
        }
        else
        {
            reg = 0xba;     // %r10
        }
        if (param->tipo_val == INT_PAR)
        {
            // Se o parâmetro for do tipo inteiro, passamos 4 bytes para o código
            int inteiro = param->valor.v_int;
            // A atribuição faz um typecast implícito para byte
            byte b1 = inteiro;          // byte menos significativo de ptr
            byte b2 = inteiro >> 8;
            byte b3 = inteiro >> 16;
            byte b4 = inteiro >> 24;    // byte mais significativo de ptr

            byte param_fix[] = {
                0x41, reg, b1, b2, b3, b4    // movl     inteiro, reg
            };
            n = 6;

            // adiciona os n bytes de param_fix na posição adequada de codigo
            memcpy(codigo + (*pos), param_fix, n);
        }
        else // parâmetro é pointer
        {
            // Se o parâmetro for do tipo ponteiro, passamos 8 bytes para o código
            void* ptr = param->valor.v_ptr;
            byte b1 = (long)ptr;        // byte menos significativo de ptr
            // Converte ptr para long, desloca 1 byte para a direita e guarda em b2
            byte b2 = (long)ptr >> 8;
            byte b3 = (long)ptr >> 16;
            byte b4 = (long)ptr >> 24;
            byte b5 = (long)ptr >> 32;
            byte b6 = (long)ptr >> 40;
            byte b7 = (long)ptr >> 48;
            byte b8 = (long)ptr >> 56;  // byte mais significativo de ptr
            byte param_fix[] = {
                0x49, reg, b1, b2, b3, b4, b5, b6, b7, b8 // movabs   ptr, reg
            };
            n = 10;

            // adiciona os n bytes de param_fix na posição adequada de codigo
            memcpy(codigo + (*pos), param_fix, n);
        }

        break;
    case IND:
        // Pegamos o endereço da variável amarrada
        void* ptr = param->valor.v_ptr;
        byte b1 = (long)ptr;            // byte menos significativo de ptr
        byte b2 = (long)ptr >> 8;
        byte b3 = (long)ptr >> 16;
        byte b4 = (long)ptr >> 24;
        byte b5 = (long)ptr >> 32;
        byte b6 = (long)ptr >> 40;
        byte b7 = (long)ptr >> 48;
        byte b8 = (long)ptr >> 56;      // byte mais significativo de ptr

        if (*reg_escrita == 1) 
        {
            reg = 0x03; // %r8
        }
        else if (*reg_escrita == 2)
        {
            reg = 0x0b; // %r9
        }
        else 
        {
            reg = 0x13; // %r10
        }

        byte instr_tipo;
        if (param->tipo_val == INT_PAR)
        {
            instr_tipo = 0x45; // movimentação de inteiros
        }
        else // parâmetro é pointer
        {
            instr_tipo = 0x4d; // movimentação de ponteiros
        }

        byte param_ind[] = {
            0x49, 0xbb, b1, b2, b3, b4, b5, b6, b7, b8, // movabs   ptr, %r11
            instr_tipo, 0x8b, reg                       // mov(instr_tipo)     (%r11), reg
        };
        n = 13;

        // adiciona os n bytes de param_ind na posição adequada de codigo
        memcpy(codigo + (*pos), param_ind, n);
        break;
    }
    // soma o tamanho do vetor à posição de escrita
    *pos += n;
    // passa para o próximo registrador de escrita
    (*reg_escrita)++;
    return;
}

void prepara_parametros(unsigned char codigo[], int *pos)
{
    int n = 9; // tamanho do vetor prepara

    byte prepara[] = { // que agora é hora
        0x4c, 0x89, 0xc7,            	// movq    %r8, %rdi
        0x4c, 0x89, 0xce,             	// movq    %r9, %rsi
        0x4c, 0x89, 0xd2,               // movq    %r10, %rdx
    };

    // adiciona os n bytes de prepara na posição adequada de codigo
    memcpy(codigo + (*pos), prepara, n);

    // soma o tamanho do vetor à posição de escrita
    *pos += n;
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
        0x48, 0xb8, f1, f2, f3, f4, f5, f6, f7, f8,     // movabs   f, %rax
        0xff, 0xd0,                                     // call     *%rax
    };

    // adiciona os n bytes de call_na_func na posição adequada de codigo
    memcpy(codigo + (*pos), call_na_func, n);

    // soma o tamanho do vetor à posição de escrita
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

    // adiciona os n bytes de final na posição adequada de codigo
    memcpy(codigo + (*pos), final, n);

    // soma o tamanho do vetor à posição de escrita
    *pos += n;
    return;
}
