#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

sem_t semX , semY , semZ; // Semaforos

// Thread 1 
void *a () {
    sem_wait(&semX);
    printf("Fique a vontade.\n");
    sem_post(&semY);
    pthread_exit(NULL);
}

// Thread 2
void *b () {
    printf("Seja bem-vindo!\n");
    sem_post(&semX);
    sem_post(&semX);
    pthread_exit(NULL);
}

// Thread 3
void *c (){
    sem_wait(&semY);
    sem_wait(&semZ);
    printf("Volte sempre!\n");
    pthread_exit(NULL);
}

// Thread 4
void *d (){
    sem_wait(&semX);
    printf("Sente-se por favor.\n");
    sem_post(&semZ);
    pthread_exit(NULL);
}

// Funcao principal 
int main() 
{
    pthread_t NTHREADS[4];
    int i;
    sem_init(&semX,0,0); // Inicializando os semaforos
    sem_init(&semY,0,0);
    sem_init(&semZ,0,0);
    pthread_create(&NTHREADS[0], NULL, a, NULL); // Criando as Threads
    pthread_create(&NTHREADS[1], NULL, b, NULL);
    pthread_create(&NTHREADS[2], NULL, c, NULL);
    pthread_create(&NTHREADS[3], NULL, d, NULL);

    // Aguardando o término das threads
    for (int i = 0; i < NTHREADS; i++) { // pthread_join
        pthread_join(NTHREADS[i], NULL);
    }

    sem_destroy(&semX);   // Desalocando
    sem_destroy(&semY);
    sem_destroy(&semZ);
    return 0;
    }
