// Este programa resolve de forma concorrente a decomposição LU de Doolittle de matrizes
// Definida por A = LU
// Onde U é uma matriz dita triangular superior (valores acima abaixo da diagonal zerados)
// E L é uma matriz dita triangular inferior (valores acima da diagonal zerados)
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

// Dimensão da matriz e numero de threads
int DIM;
int NTHREADS;

// Alocação das 3 matrizes A=LU
double **matA;
double **matU;
double **matL;
double **matUSeq;
double **matLSeq;

// Tempo sequencial
double tempoSequencial;

// Função sequencial de decomposição LU utilizando o algoritmo de 
void sequencial() {
	float start, finish;

	GET_TIME(start);

	for (int i = 0; i < DIM; i++) {
		// Para a Matriz U
		for (int k = i; k < DIM; k++) {
			int soma = 0;
			for (int j = 0; j < i; j++) {
				soma += (matLSeq[i][j] * matUSeq[j][k]);
			}
			matUSeq[i][k] = matA[i][k] - soma;
		}

		// Para a Matriz L
		for (int k = i; k < DIM; k++) {
			if (i == k)
				matLSeq[i][i] = 1;
			else {
				int soma = 0;
				
				for (int j = 0; j < i; j++) {
					soma += (matLSeq[k][j] * matUSeq[j][i]);
				}
				matLSeq[k][i] = (matA[k][i] - soma) / matUSeq[i][i];
			}
		}
	}

	GET_TIME(finish);

	// Printando as matrizes
	// printf("MATRIZ L: \n");
	// for (int i = 0; i < DIM; i++) {
	// 	for (int j = 0; j < DIM; j++) {
	// 		printf("%.1lf | ", matLSeq[i][j]);
	// 	}
	// 	printf("\n");
	// }
	// printf("\n");

	// printf("MATRIZ U: \n");
	// for (int i = 0; i < DIM; i++) {
	// 	for (int j = 0; j < DIM; j++)	{
	// 		printf("%.1lf | ", matUSeq[i][j]);
	// 	}
	// 	printf("\n");
	// }
	// printf("\n");

	// Calculando tempo Sequencial
	printf("TEMPO SEQUENCIAL: %.15lf \n\n", finish - start);
	tempoSequencial = finish - start;
}

// Tarefa realizadas pelas threads
// Recebe como parametro o identificador local da thread
void* tarefa(void* arg) {
	long int local = (int *) arg;

	// Divisão de tarefas feita de forma alternada
	for (long int i = local; i < DIM; i+= NTHREADS) {
		// Para a Matriz U
		for (int k = i; k < DIM; k++) {
			int soma = 0;
			for (int j = 0; j < i; j++) {
				soma += (matL[i][j] * matU[j][k]);
			}
			matU[i][k] = matA[i][k] - soma;
		}

		// Para a Matriz L
		for (int k = i; k < DIM; k++) {
			if (i == k)
				matL[i][i] = 1;
			else {
				int soma = 0;
				
				for (int j = 0; j < i; j++) {
					soma += (matL[k][j] * matU[j][i]);
				}
				matL[k][i] = (matA[k][i] - soma) / matU[i][i];
			}
		}
	}
	
	pthread_exit(NULL);
}

// Função principal
int main(int argc, char *argv[]) {
	// Variaveis de tempo
	double start, finish; 

	// Variavel aux para inicialização de A
	int aux;

	// Variavel identificadora da thread
	pthread_t *tid;

	// Verificação de parametros
	if (argc < 3) {
		printf("escreva: %s <Dimensao da matriz A> <numero de threads> \n", argv[0]);
		return 1;
	}

	DIM = atoi(argv[1]);
	NTHREADS = atoi(argv[2]);

	// Checagem se o numero de threads não é maior que o número da dimensão da matriz
	if (NTHREADS > DIM) {
		NTHREADS = DIM;
	}

	// Alocação de memória para o identificador do sistema das threads
	tid = (pthread_t *) malloc(sizeof(pthread_t) * NTHREADS);
  if (tid == NULL) {
    fprintf(stderr, "ERRO--malloc\n");
    return 2;
  }

	// Alocação de memória das matrizes
	matA = malloc (DIM * sizeof (double *));
	for (int i = 0; i < DIM; ++i) {
      matA[i] = malloc (DIM * sizeof (double));
	}
	if (matA == NULL) {
		printf("ERROR--malloc\n");
		return 2;
	}

	matL = malloc (DIM * sizeof (double *));
	for (int i = 0; i < DIM; ++i) {
      matL[i] = malloc (DIM * sizeof (double));
	}
	if (matL == NULL)	{
		printf("ERROR--malloc\n");
		return 2;
	}

	matLSeq = malloc (DIM * sizeof (double *));
	for (int i = 0; i < DIM; ++i) {
      matLSeq[i] = malloc (DIM * sizeof (double));
	}
	if (matLSeq == NULL)	{
		printf("ERROR--malloc\n");
		return 2;
	}

	matU = malloc (DIM * sizeof (double *));
	for (int i = 0; i < DIM; ++i) {
      matU[i] = malloc (DIM * sizeof (double));
	}
	if (matU == NULL)	{
		printf("ERROR--malloc\n");
		return 2;
	}

	matUSeq = malloc (DIM * sizeof (double *));
	for (int i = 0; i < DIM; ++i) {
      matUSeq[i] = malloc (DIM * sizeof (double));
	}
	if (matUSeq == NULL)	{
		printf("ERROR--malloc\n");
		return 2;
	}

	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			// Caso queira entrar com os parametros da Matriz
			// Descomente a linha abaixo e comente as outras 2 linhas abaixo
			// scanf("%f | ", &matA[i][j]);
			aux = rand() % 10;
			matA[i][j] = (float) aux; // numeros aleatorios entre 0 e 9
		}
    // printf("\n");
	}

	// printf("MATRIZ A: \n");
	// for (int i = 0; i < DIM; i++)	{
	// 	for (int j = 0; j < DIM; j++)	{
	// 		printf("%.1f | ", matA[i][j]);
	// 	}
	// 	printf("\n");
	// }
	// printf("\n\n");

	// Execução sequencial
	printf("EXECUCAO SEQUENCIAL\n");
	sequencial();

	// Execução concorrente
	printf("EXECUCAO CONCORRENTE\n");
	GET_TIME(start);
	for (long int i = 0; i < NTHREADS; i++) {
		if (pthread_create(tid+i, NULL, tarefa, (void *) i)) {
      fprintf(stderr, "ERRO--pthread_create\n");
      return 3;
    }
	}

	for (long int i = 0; i < NTHREADS; i++) {
    if (pthread_join(*(tid+i), NULL)) {
      fprintf(stderr, "ERRO--pthread_join\n");
      return 3;
    }
	}
	GET_TIME(finish);
	printf("TEMPO CONCORRENTE: %.15lf\n\n\n", finish-start);

	// Printando Matrizes da execução concorrente
	// printf("MATRIZ L: \n");
	// for (int i = 0; i < DIM; i++) {
	// 	for (int j = 0; j < DIM; j++) {
	// 		printf("%.1lf | ", matL[i][j]);
	// 	}
	// 	printf("\n");
	// }
	// printf("\n");

	// printf("MATRIZ U: \n");
	// for (int i = 0; i < DIM; i++) {
	// 	for (int j = 0; j < DIM; j++)	{
	// 		printf("%.1lf | ", matU[i][j]);
	// 	}
	// 	printf("\n");
	// }
	// printf("\n");

	// Validando resultados
  double margem = pow(10,-6);
  // Validação da Matriz L
	for (int i = 0; i < DIM; i++) {
    for (int j = 0; j < DIM; j++) {
			if (abs(matL[i][j] - matLSeq[i][j]) > margem) {
        printf("--ERRO na validação de L: matL[%d][%d] = %lf e matLSeq[%d][%d] = %lf\n\n", i, j, matL[i][j], i,j, matLSeq[i][j]);
				exit(-1); 
      }

			if (abs(matU[i][j] - matUSeq[i][j]) > margem) {
        printf("--ERRO na validação de U: matU[%d][%d] = %lf e matUSeq[%d][%d] = %lf\n\n", i, j, matU[i][j], i,j, matUSeq[i][j]);
				exit(-1); 
      }
    }     
  }

	// Calculando tempo concorrente
	printf("COMPARACAO DE TEMPO:\n");
	printf("TEMPO SEQUENCIAL: %.15lf\n", tempoSequencial);
	printf("TEMPO CONCORRENTE: %.15lf\n", finish-start);
	if (tempoSequencial > finish-start) {
		printf("O TEMPO CONCORRENTE GANHOU\n");
	} else if (tempoSequencial < finish-start) {
		printf("O TEMPO SEQUENCIAL GANHOU\n");
	} else {
		printf("AMBOS OS TEMPOS FORAM IGUAIS!\n");
	}

	// Calculo do ganho de performance
	printf("GANHO DE PERFORMANCE: ~%lf\n", tempoSequencial / (finish - start));
	printf("\n");


	// Liberando espaço de memória
	free(matA);
	free(matL);
	free(matU);
	free(matLSeq);
	free(matUSeq);
	free(tid);

	return 0;
}
