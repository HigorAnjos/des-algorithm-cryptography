# Criptografia DES em C++

Este repositório contém uma implementação do algoritmo de criptografia DES (Data Encryption Standard) em C++. O DES é um algoritmo de cifragem simétrica que utiliza uma chave de 56 bits para criptografar blocos de 64 bits de dados. O algoritmo foi amplamente utilizado até ser substituído por padrões mais seguros, como o AES (Advanced Encryption Standard).

## Como Funciona o Algoritmo DES?

O DES segue uma série de etapas de permutação, substituição e permutação inversa, utilizando uma chave de 56 bits. O algoritmo opera em blocos de 64 bits e realiza 16 rodadas de processamento, em que cada rodada utiliza uma chave derivada da chave original. A operação de cada rodada inclui uma expansão da metade direita do bloco de dados, uma substituição utilizando as tabelas S-box e uma permutação final. A chave de criptografia é gerada a partir de uma permutação inicial (PC1), passando por uma série de deslocamentos e uma permutação final (PC2).

## Funcionalidade

O código implementa a criptografia DES com as seguintes etapas:

1. **Permutação Inicial (IP)**: Aplica uma permutação inicial ao bloco de entrada de 64 bits.
2. **Geração da Chave**: A chave de 56 bits é permutada para gerar 16 chaves sub-secretas.
3. **16 Rodadas de Criptografia**: O algoritmo executa 16 rodadas, cada uma aplicando uma operação de expansão, XOR com a chave sub-secreta e substituição por meio das S-boxes.
4. **Permutação Final (PI)**: A saída da última rodada é permutada para gerar o texto cifrado final.

## Como Usar

1. Clone o repositório:

```bash
git clone https://github.com/seu_usuario/des-implementation.git
cd des-implementation
```

2. Compile o código:

```bash
g++ -o des des.cpp
```

3. Execute o programa:

```bash
./des
```

O programa usa exemplos de entrada e chave para demonstrar a criptografia.

### Exemplo de Entrada e Saída

- **Texto de entrada**: `0x123456789abcdef0`
- **Chave**: `0x133457799BBCDFF1`
- **Saída criptografada**: `0x9b65d17f8f428b94`

## Análise de Complexidade

A análise de complexidade assintótica (usando notação Big O) para o algoritmo DES pode ser dividida em duas partes principais: a criptografia de dados e a geração das chaves.

### 1. **Geração das Chaves (Passo de Pré-processamento)**
   - A geração das 16 chaves sub-secretas requer a permutação inicial e a aplicação de deslocamentos iterativos, o que é feito de forma constante para cada uma das 16 rodadas.
   - O tempo gasto na geração de cada chave é proporcional ao número de operações de permutação e deslocamento, o que é uma operação constante O(1) por chave.
   - **Complexidade para gerar todas as chaves**: O(16), que é O(1), pois o número de rodadas (16) é fixo.

### 2. **Criptografia de Dados (Rodadas de Processamento)**
   - Para cada um dos 16 passos, o algoritmo realiza as seguintes operações:
     - Expansão da metade direita do bloco de 32 bits para 48 bits: O(1)
     - Substituição usando as S-boxes: Cada S-box envolve operações constantes, mas são feitas 8 vezes para cada rodada. Isso é O(1) por rodada.
     - Permutação após a substituição: O(1)
     - A troca entre as metades esquerda e direita: O(1)
   - Cada rodada é composta de operações constantes.
   - **Complexidade para todas as rodadas**: O(16), que é O(1), já que o número de rodadas (16) é fixo.

### 3. **Complexidade Total**
   - Tanto a geração das chaves quanto as 16 rodadas de criptografia têm complexidade O(1), já que o número de operações em cada rodada e a geração das chaves não dependem do tamanho da entrada (fixo em 64 bits) nem da chave (fixa em 56 bits).
   
   **Conclusão**: O tempo de execução total para criptografar um único bloco de 64 bits com o DES é **O(1)**. No entanto, se considerarmos vários blocos de dados, o tempo total será linear em função do número de blocos a serem criptografados, ou seja, **O(n)**, onde **n** é o número de blocos de dados.

### Notas:
- Embora a complexidade do DES seja constante para uma execução de criptografia de 64 bits, o algoritmo é considerado obsoleto e vulnerável a ataques, como o ataque de força bruta, devido ao tamanho da chave de 56 bits.
