# trab1_compconc20202
Diretorio para Trabalho 1 da disciplina Comp Conc 2020/2 (UFRJ) proposto pela [Profa. Silvana Rossetto](https://dcc.ufrj.br/~silvana/);

Integrantes:

Roberto Leonie Ferreira Moreira 

116192266 - Thales Monteiro Pierini Macena - thalesmacena@gmail.com

## Sobre
O Objetivo do trabalho é implementar o [Algoritmo Doolittle](https://en.wikipedia.org/wiki/LU_decomposition#Doolittle_algorithm) para o calculo da [decomposição A = LU](https://en.wikipedia.org/wiki/LU_decomposition) de forma sequencial e concorrente a fim de calcular o tempo de execução da decomposição LU sequencial e concorrente (outros tempos como a inicialização da matriz A está sendo deixada de lado) de forma que é possivel calcular o ganho de performance. 

A [decomposição A = LU](https://en.wikipedia.org/wiki/LU_decomposition) decompoe uma matriz A em uma [Matriz U](https://en.wikipedia.org/wiki/Triangular_matrix#Description) que é dita uma matriz triangular superior e uma [Matriz L](https://en.wikipedia.org/wiki/Triangular_matrix#Description) que é dita uma matriz triangular inferior. Os computadores geralmente resolvem sistemas quadrados de equações lineares usando a decomposição LU, e também é uma etapa fundamental ao inverter uma matriz ou calcular o determinante de uma matriz.

O ganho de desempenho (aceleração) obtido é calculado pela seguinte fórmula: `Tsequencial / Tconcorrente`. Onde `Tsequencial` é o tempo de execução do algoritmo sequencial e `Tconcorrente` é definido pela equação `ts + tp (P)` onde `ts` é o tempo da parte sequencial do programa (que não será dividido entre threads) e `tp(P)` é o tempo da parte paralela do programa usando processadores `P`.

A validação de assertividade do algoritmo é calculado através da diferença entre o resultado da execução Sequencial e a execução concorrente, como existe o problema de representação e do acumulo de erros nas somas com ponto flutuante e double é necessário fazer uma aproximação para validar o resultado das matrizes.

## Ganho de Desempenho
### Matriz 100x100

N# Threads | Sequencial | Concorrente | Ganho
--- | --- | --- | ---
**1** | 0.0029s | 0.0035s | ~0.83 
**2** | 0.0029s | 0.01166s | ~0.25
**4** | 0.0029s | 0.0321s | ~0.09

### Matriz 500x500

N# Threads | Sequencial | Concorrente | Ganho
--- | --- | --- | ---
**1** | 0.3725s | 0.3670s | ~1.01
**2** | 0.3725s | 0.4468s | ~0.83
**4** | 0.3725s |  0.5920s | ~0.63

### Matriz 1000x1000

N# Threads | Sequencial | Concorrente | Ganho
--- | --- | --- | ---
**1** | 3.3341s | 3.1808s | ~1.05 
**2** | 3.2883s | 3.6186s | ~0.91
**4** | 3.3519s | 3.9078s | ~0.86

### Matriz 2000x2000

N# Threads | Sequencial | Concorrente | Ganho
--- | --- | --- | ---
**1** | 37.8179s | 35.2777s | ~1.07
**2** | 31.9959s | 0s | ~0
**4** | 0s | 0s | ~0
