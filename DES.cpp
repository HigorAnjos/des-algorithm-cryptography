#include <iostream>
#include <bitset>
#include <stdint.h>

#define LB32_MASK   0x00000001
#define LB64_MASK   0x0000000000000001
#define L64_MASK    0x00000000ffffffff
#define H64_MASK    0xffffffff00000000

// Tabela de Permutação Inicial (IP)
static int IP[] = {58, 50, 42, 34, 26, 18, 10,  2, 
                   60, 52, 44, 36, 28, 20, 12,  4, 
                   62, 54, 46, 38, 30, 22, 14,  6, 
                   64, 56, 48, 40, 32, 24, 16,  8, 
                   57, 49, 41, 33, 25, 17,  9,  1, 
                   59, 51, 43, 35, 27, 19, 11,  3, 
                   61, 53, 45, 37, 29, 21, 13,  5, 
                   63, 55, 47, 39, 31, 23, 15,  7};

// Tabela de Permutação Inversa (PI)
static int PI[] = {40,  8, 48, 16, 56, 24, 64, 32, 
                   39,  7, 47, 15, 55, 23, 63, 31, 
                   38,  6, 46, 14, 54, 22, 62, 30, 
                   37,  5, 45, 13, 53, 21, 61, 29, 
                   36,  4, 44, 12, 52, 20, 60, 28, 
                   35,  3, 43, 11, 51, 19, 59, 27, 
                   34,  2, 42, 10, 50, 18, 58, 26, 
                   33,  1, 41,  9, 49, 17, 57, 25};

// Tabela de Expansão
static int E[] = {32,  1,  2,  3,  4,  5,  
                  4,  5,  6,  7,  8,  9,  
                  8,  9, 10, 11, 12, 13, 
                 12, 13, 14, 15, 16, 17, 
                 16, 17, 18, 19, 20, 21, 
                 20, 21, 22, 23, 24, 25, 
                 24, 25, 26, 27, 28, 29, 
                 28, 29, 30, 31, 32,  1};

// Tabela de Permutação após a S-Box (P)
static int P[] = {16,  7, 20, 21, 
                  29, 12, 28, 17, 
                   1, 15, 23, 26, 
                   5, 18, 31, 10, 
                   2,  8, 24, 14, 
                  32, 27,  3,  9, 
                  19, 13, 30,  6, 
                  22, 11,  4, 25};

// As Tabelas S-Box
static int S[8][64] = {
    {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7, 
     0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8, 
     4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0, 
     15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},
    // outras S-Boxes omitidas por simplicidade
};

// Tabela de Permutação de Escolha 1 (PC1)
static int PC1[] = {57, 49, 41, 33, 25, 17, 9, 1, 
                    58, 50, 42, 34, 26, 18, 10, 2, 
                    59, 51, 43, 35, 27, 19, 11, 3, 
                    60, 52, 44, 36, 63, 55, 47, 39, 
                    31, 23, 15, 7, 62, 54, 46, 38, 
                    30, 22, 14, 6, 61, 53, 45, 37, 
                    29, 21, 13, 5, 28, 20, 12, 4};

// Tabela de Permutação de Escolha 2 (PC2)
static int PC2[] = {14, 17, 11, 24, 1, 5, 
                    3, 28, 15, 6, 21, 10, 
                    23, 19, 12, 4, 26, 8, 
                    16, 7, 27, 20, 13, 2, 
                    41, 52, 31, 37, 47, 55, 
                    30, 40, 51, 45, 33, 48, 
                    44, 49, 39, 56, 34, 53, 
                    46, 42, 50, 36, 29, 32};

// Array de deslocamentos da chave
static int iteration_shift[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

// Função de criptografia DES
uint64_t des(uint64_t input, uint64_t key, char mode) {
    int i, j;
    uint32_t C = 0, D = 0, L = 0, R = 0, temp = 0;
    uint64_t sub_key[16] = {0}, s_input = 0;
    uint64_t init_perm_res = 0, permuted_choice_1 = 0, permuted_choice_2 = 0;
    
    // Permutação inicial
    for (i = 0; i < 64; i++) {
        init_perm_res <<= 1;
        init_perm_res |= (input >> (64 - IP[i])) & LB64_MASK;
    }
    
    L = (uint32_t) (init_perm_res >> 32) & L64_MASK;
    R = (uint32_t) init_perm_res & L64_MASK;
    
    // Inicialização da chave
    for (i = 0; i < 56; i++) {
        permuted_choice_1 <<= 1;
        permuted_choice_1 |= (key >> (64 - PC1[i])) & LB64_MASK;
    }
    
    C = (uint32_t) ((permuted_choice_1 >> 28) & 0x000000000fffffff);
    D = (uint32_t) (permuted_choice_1 & 0x000000000fffffff);
    
    // Cálculo das 16 chaves
    for (i = 0; i < 16; i++) {
        // Deslocamento das partes C e D
        for (j = 0; j < iteration_shift[i]; j++) {
            C = 0x0fffffff & (C << 1) | 0x00000001 & (C >> 27);
            D = 0x0fffffff & (D << 1) | 0x00000001 & (D >> 27);
        }
        
        permuted_choice_2 = 0;
        permuted_choice_2 = (((uint64_t) C) << 28) | (uint64_t) D;
        
        sub_key[i] = 0;
        
        for (j = 0; j < 48; j++) {
            sub_key[i] <<= 1;
            sub_key[i] |= (permuted_choice_2 >> (56 - PC2[j])) & LB64_MASK;
        }
    }
    
    // Ciclo de 16 rodadas
    for (i = 0; i < 16; i++) {
        // Expansão da metade direita
        s_input = 0;
        for (j = 0; j < 48; j++) {
            s_input <<= 1;
            s_input |= (R >> (32 - E[j])) & LB32_MASK;
        }
        
        // XOR com a chave sub_key[i]
        s_input ^= sub_key[i];
        
        // S-Box
        for (j = 0; j < 8; j++) {
            int row = (s_input >> (42 - j*6)) & 0x3;
            int col = (s_input >> (41 - j*6)) & 0x0f;
            s_input &= ~(0x3f << (42 - j*6));
            s_input |= S[j][row * 16 + col] << (42 - j*6);
        }
        
        // Permutação final P
        temp = 0;
        for (j = 0; j < 32; j++) {
            temp <<= 1;
            temp |= (s_input >> (32 - P[j])) & LB32_MASK;
        }
        
        L ^= temp;
        if (i != 15) {
            temp = L;
            L = R;
            R = temp;
        }
    }
    
    // Permutação final PI
    uint64_t output = 0;
    for (i = 0; i < 64; i++) {
        output <<= 1;
        output |= (L >> (32 - IP[i])) & LB64_MASK;
    }
    
    return output;
}

// Função principal para testar
int main() {
    uint64_t input = 0x123456789abcdef0;  // Exemplo de entrada
    uint64_t key = 0x133457799BBCDFF1;    // Exemplo de chave
    uint64_t encrypted = des(input, key, 'E');
    std::cout << "Resultado criptografado: " << std::hex << encrypted << std::endl;
    return 0;
}
