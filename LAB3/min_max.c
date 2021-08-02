#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

long long int n_elementos;
int nthreads;
float *v;

struct valor {
    float valor_max;
    float valor_min;
};

void *tarefa(void *arg) {
    long int id = (long int) arg;
    struct valor *valor_local;
    long int thread_block_size = n_elementos / nthreads;
    long int thread_block_init = id * thread_block_size;
    long int thread_block_end;

    valor_local = (struct valor*) malloc(sizeof(struct valor));
    if(valor_local == NULL) {
        fprintf(stderr, "ERRO: Um erro ocorreu durante o malloc");
        exit(1);
    }

    valor_local->valor_max = v[thread_block_init];
    valor_local->valor_min = v[thread_block_init];

    if(id == nthreads - 1) {
        thread_block_end = n_elementos;
    } else {
        thread_block_end = thread_block_init + thread_block_size;
    }

    for(long int i = thread_block_init; i < thread_block_end; i++) {
        if(v[i] > valor_local->valor_max) {
            valor_local->valor_max = v[i];
        }
        if(v[i] < valor_local->valor_min) {
            valor_local->valor_min = v[i];
        }
    }
    pthread_exit((void *) valor_local);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    struct valor seq_valors;
    struct valor *return_valor, *conc_valor;
    double initial_time, end_time;
    pthread_t *tid;

    if(argc < 3) {
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero de threads> \n", argv[0]);
        return 1;
    }

    n_elementos = atoll(argv[1]);
    nthreads = atoi(argv[2]);

    v = (float *) malloc(sizeof(float) * n_elementos);
    if(v == NULL) {
        fprintf(stderr, "ERRO: Um erro ocorreu durante o malloc");
        return 2;
    }
    for(long int i = 0; i < n_elementos; i++) {
        v[i] = 1000.1/((rand() % 1000) + 1);
    }

    GET_TIME(initial_time);
    seqvalor.valor_max = v[0];
    seq_valor.valor_min = v[0];
    for(long int i = 0; i < n_elementos; i++) {
        if(v[i] > seq_valor.valor_max) {
            seq_valor.valor_max = v[i];
        }

        if(v[i] < seq_valor.valor_min) {
            seq_valor.valor_min = v[i];
        }
    }
    GET_TIME(end_time);
    printf("Tempo sequencial: %lf\n", end_time - initial_time);

    GET_TIME(initial_time);
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if( tid == NULL) {
        fprintf(stderr, "ERRO: Um erro ocorreu durante o malloc\n");
        return 2;
    }

    for(long int i = 0; i < nthreads; i++) {
        if(pthread_create(tid + i, NULL, tarefa, (void *) i)) {
             fprintf(stderr, "ERRO: Um erro ocorreu durante o pthread_create\n");
             return 3;
        }
    }

    conc_valor = (struct valor*) malloc(sizeof(struct valor));
    if(conc_valor == NULL) {
        fprintf(stderr, "ERRO: Um erro ocorreu durante o malloc\n");
        return 2;
    }

    conc_valor->valormax = v[0];
    conc_valor->valor_min = v[0];
    for(long int i = 0; i < nthreads; i++) {
        if(pthread_join(*(tid + i), (void**) &return_valor)) {
             fprintf(stderr, "ERRO: Um erro ocorreu durante o pthread_join\n");
             return 3;
        }
        
        if(return_valor->valor_max > conc_valor->valor_max) {
            conc_valor->valor_max = return_valor->valor_max; 
        }

        if(return_valor->valor_min < conc_valor->valor_min) {
            conc_valor->valor_min = return_valor->valor_min; 
        }

        free(return_valor);
    }
    GET_TIME(end_time);
    printf("Tempo concorrente: %lf\n", end_time - initial_time);

    printf("Max sequencial value: %f\n", seq_valor.valor_max);
    printf("Min sequencial value: %f\n", seq_valor.valor_min);
    printf("Max concurrent value: %f\n", conc_valor->valor_max);
    printf("Min concurrent value: %f\n", conc_valor->valor_min);

    free(v);
    free(tid);
    free(conc_valor);

    return 0;
}
