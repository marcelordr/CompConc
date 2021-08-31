/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Trabalho 1 */
/* Aluno: Marcelo Rodrigues */
/* DRE: 118143203 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_NTHREADS 20 //numero de threads
#define NMAX 100000000 //tamanho max da sequencia a´te onde vai identificar os primos

int N, NTHREADS, primo, prox;


pthread_mutex_t x_mutex;
pthread_t id [MAX_NTHREADS];
