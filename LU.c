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
double **matLConc;
double **matUConc;
double **matLSeq;
double **matUSeq;

// Tempo sequencial
double tempoSequencial;

// Criacao do Lock
pthread_mutex_t lock;
// pthread_mutex_lock(&lock);
// pthread_mutex_unlock(&lock);

// Função sequencial de decomposição LU utilizando o algoritmo de Doolittle
void sequencial() {
	double start, finish;

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
	printf("MATRIZ L: \n");
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			printf("%.1lf | ", matLSeq[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	printf("MATRIZ U: \n");
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++)	{
			printf("%.1lf | ", matUSeq[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	// Calculando tempo Sequencial
	tempoSequencial = finish - start;
	printf("TEMPO SEQUENCIAL: %.15lf \n\n", tempoSequencial);
}

// Tarefa realizadas pelas threads
// Recebe como parametro o identificador local da thread
void *tarefa(void *arg) {
	long int localId = (long int) arg;

	// Divisão de tarefas feita de forma alternada
	for (long int i = localId; i < DIM; i += NTHREADS) {
		// Para a Matriz U
		for (long int k = i; k < DIM; k++) {
			int soma = 0;
			for (long int j = 0; j < i; j++) {
				soma += (matLConc[i][j] * matUConc[j][k]);
			}
			matUConc[i][k] = matA[i][k] - soma;
		}

		// Para a Matriz L
		for (long int k = i; k < DIM; k++) {
			if (i == k) {
				matLConc[i][i] = 1;
			}
			else {
				int soma = 0;
				for (long int j = 0; j < i; j++) {
					soma += (matLConc[k][j] * matUConc[j][i]);
				}
				matLConc[k][i] = (matA[k][i] - soma) / matUConc[i][i];
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
	tid = (pthread_t *)malloc(sizeof(pthread_t) * NTHREADS);
	if (tid == NULL) {
		fprintf(stderr, "ERRO--malloc\n");
		return 2;
	}

	// Alocação de memória das matrizes
	matA = malloc(DIM * sizeof(double *));
	for (int i = 0; i < DIM; ++i) {
		matA[i] = malloc(DIM * sizeof(double));
	}
	if (matA == NULL) {
		printf("ERROR--malloc\n");
		return 2;
	}

	matLConc = malloc(DIM * sizeof(double *));
	for (int i = 0; i < DIM; ++i) {
		matLConc[i] = malloc(DIM * sizeof(double));
	}
	if (matLConc == NULL) {
		printf("ERROR--malloc\n");
		return 2;
	}

	matLSeq = malloc(DIM * sizeof(double *));
	for (int i = 0; i < DIM; ++i) {
		matLSeq[i] = malloc(DIM * sizeof(double));
	}
	if (matLSeq == NULL) {
		printf("ERROR--malloc\n");
		return 2;
	}

	matUConc = malloc(DIM * sizeof(double *));
	for (int i = 0; i < DIM; ++i) {
		matUConc[i] = malloc(DIM * sizeof(double));
	}
	if (matUConc == NULL) {
		printf("ERROR--malloc\n");
		return 2;
	}

	matUSeq = malloc(DIM * sizeof(double *));
	for (int i = 0; i < DIM; ++i) {
		matUSeq[i] = malloc(DIM * sizeof(double));
	}
	if (matUSeq == NULL) {
		printf("ERROR--malloc\n");
		return 2;
	}

	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			// Caso queira entrar com os parametros da Matriz
			// Descomente a linha abaixo e comente as outras 2 linhas abaixo
			// scanf("%f | ", &matA[i][j]);
			aux = rand() % 10; // numeros aleatorios entre 0 e 9
			matA[i][j] = (double)aux;
		}
		// printf("\n");
	}

	printf("MATRIZ A: \n");
	for (int i = 0; i < DIM; i++)	{
		for (int j = 0; j < DIM; j++)	{
			printf("%.1f | ", matA[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");

	// Execução sequencial
	printf("EXECUCAO SEQUENCIAL\n");
	sequencial();

	// Execução concorrente
	printf("EXECUCAO CONCORRENTE\n");
	GET_TIME(start);

	for (long int i = 0; i < NTHREADS; i++) {
		if (pthread_create(tid + i, NULL, tarefa, (void *) i)) {
			fprintf(stderr, "ERRO--pthread_create\n");
			return 3;
		}
	}

	for (long int i = 0; i < NTHREADS; i++) {
		if (pthread_join(*(tid + i), NULL)) {
			fprintf(stderr, "ERRO--pthread_join\n");
			return 3;
		}
	}

	GET_TIME(finish);
	double tempoConcorrente = finish - start;
	printf("TEMPO CONCORRENTE: %.15lf\n\n\n", tempoConcorrente);

	// Printando Matrizes da execução concorrente
	printf("MATRIZ L: \n");
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			printf("%.1lf | ", matLConc[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	printf("MATRIZ U: \n");
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++)	{
			printf("%.1lf | ", matUConc[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	// Validando resultados
	double epsilon = pow(10, -9);
	// Validação da Matriz L
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			if (abs(matLConc[i][j] - matLSeq[i][j]) > epsilon) {
				printf("--ERRO na validacao de L: matLConc[%d][%d] = %lf e matLSeq[%d][%d] = %lf\n\n",
							 i, j, matLConc[i][j], i, j, matLSeq[i][j]);
				exit(-1);
			}

			if (abs(matUConc[i][j] - matUSeq[i][j]) > epsilon) {
				printf("--ERRO na validacao de U: matUConc[%d][%d] = %lf e matUSeq[%d][%d] = %lf\n\n",
							 i, j, matUConc[i][j], i, j, matUSeq[i][j]);
				exit(-1);
			}
		}
	}

	// Calculando tempo concorrente
	printf("COMPARACAO DE TEMPO:\n");
	printf("TEMPO SEQUENCIAL: %.15lf\n", tempoSequencial);
	printf("TEMPO CONCORRENTE: %.15lf\n", tempoConcorrente);
	
	if (tempoSequencial > tempoConcorrente) {
		printf("O TEMPO CONCORRENTE GANHOU\n");
	}
	else if (tempoSequencial < tempoConcorrente) {
		printf("O TEMPO SEQUENCIAL GANHOU\n");
	}
	else {
		printf("AMBOS OS TEMPOS FORAM IGUAIS!\n");
	}

	// Calculo do ganho de performance
	printf("GANHO DE PERFORMANCE: ~%lf\n", tempoSequencial / tempoConcorrente);
	printf("\n");

	// Liberando espaço de memória
	free(matA);
	free(matLConc);
	free(matUConc);
	free(matLSeq);
	free(matUSeq);
	free(tid);

	return 0;
}
