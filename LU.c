#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

int DIM;
int NTHREADS;

float **matA;
float **matU;
float **matL;

void sequencial() {
	float start, finish;

	GET_TIME(start);

	for (int i = 0; i < DIM; i++) {
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

	GET_TIME(finish);

	// printf("MATRIZ L: \n");
	// for (int i = 0; i < DIM; i++) {
	// 	for (int j = 0; j < DIM; j++) {
	// 		printf("%.1lf ", matL[i][j]);
	// 	}
	// 	printf("\n");
	// }
	// printf("\n");

	// printf("MATRIZ U: \n");
	// for (int i = 0; i < DIM; i++) {
	// 	for (int j = 0; j < DIM; j++)	{
	// 		printf("%.1lf ", matU[i][j]);
	// 	}
	// 	printf("\n");
	// }
	printf("\n");
	// Calculando tempo Sequencial
	printf("TEMPO SEQUENCIAL: %.15lf \n\n", finish - start);

	printf("\n\n");
}

void* tarefa(void* arg) {
	long int local = (int *) arg;

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

int main(int argc, char *argv[]) {
	double start, finish; 

	int aux;
	pthread_t *tid;

	if (argc < 3) {
		printf("escreva: %s <Dimensao da matriz> <numero de threads> \n", argv[0]);
		return 1;
	}

	DIM = atoi(argv[1]);
	NTHREADS = atoi(argv[2]);
	if (NTHREADS > DIM) {
		NTHREADS = DIM;
	}

	tid = (pthread_t *) malloc(sizeof(pthread_t) * NTHREADS);
  if (tid == NULL) {
    fprintf(stderr, "ERRO--malloc\n");
    return 2;
  }

	matA = malloc (DIM * sizeof (float *));
	for (int i = 0; i < DIM; ++i) {
      matA[i] = malloc (DIM * sizeof (float));
	}
	if (matA == NULL) {
		printf("ERROR--malloc\n");
		return 2;
	}

	matL = malloc (DIM * sizeof (float *));
	for (int i = 0; i < DIM; ++i) {
      matL[i] = malloc (DIM * sizeof (float));
	}
	if (matL == NULL)	{
		printf("ERROR--malloc\n");
		return 2;
	}

	matU = malloc (DIM * sizeof (float *));
	for (int i = 0; i < DIM; ++i) {
      matU[i] = malloc (DIM * sizeof (float));
	}
	if (matU == NULL)	{
		printf("ERROR--malloc\n");
		return 2;
	}

	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			// scanf("%f", &matA[i][j]);
			aux = rand() % 10;
			matA[i][j] = (float) aux; // numeros aleatorios entre 0 e 9
		}
	}

	// printf("MATRIZ A: \n");
	// for (int i = 0; i < DIM; i++)	{
	// 	for (int j = 0; j < DIM; j++)	{
	// 		printf("%.1f ", matA[i][j]);
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

	// Printando Matrizes da execução concorrente
	// printf("MATRIZ L: \n");
	// for (int i = 0; i < DIM; i++) {
	// 	for (int j = 0; j < DIM; j++) {
	// 		printf("%.1lf ", matL[i][j]);
	// 	}
	// 	printf("\n");
	// }
	// printf("\n");

	// printf("MATRIZ U: \n");
	// for (int i = 0; i < DIM; i++) {
	// 	for (int j = 0; j < DIM; j++)	{
	// 		printf("%.1lf ", matU[i][j]);
	// 	}
	// 	printf("\n");
	// }
	// printf("\n");

	// Calculando tempo concorrente
	printf("TEMPO CONCORRENTE: %.15lf\n\n", finish-start);


	// Liberando espaço de memória
	free(matA);
	free(matL);
	free(matU);
	free(tid);

	return 0;
}
