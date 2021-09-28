#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NTHREADS  4 //Numero de threads

// Semaforos
sem_t x , y , z;


// Thread 1 
void *a () {
    sem_wait(&x);
    printf("Fique a vontade.\n");
    sem_post(&y);
    pthread_exit(NULL);
}

// Thread 2
void *b () {
    printf("Seja bem-vindo!\n");
    sem_post(&x);
    sem_post(&x);
    pthread_exit(NULL);
}

// Thread 3
void *c (){
    sem_wait(&y);
    sem_wait(&z);
    printf("Volte sempre!\n");
    pthread_exit(NULL);
}

// Thread 4
void *d (){
    sem_wait(&x);
    printf("Sente-se por favor.\n");
    sem_post(&z);
    pthread_exit(NULL);
}

// Funcao principal 
int main() {
    pthread_t threads[NTHREADS];

    // Inicializando os semaforos
    sem_init(&x,0,0);
    sem_init(&y,0,0);
    sem_init(&z,0,0);

    // Criando as Threads
    pthread_create(&threads[0], NULL, a, NULL);
    pthread_create(&threads[1], NULL, b, NULL);
    pthread_create(&threads[2], NULL, c, NULL);
    pthread_create(&threads[3], NULL, d, NULL);

    // Aguardando o término das threads
    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Desalocando variáveis e encerrando o programa
    sem_destroy(&x);
    sem_destroy(&y);
    sem_destroy(&z);
    return 0;
    }
