/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 4 */
/* Aluno: Marcelo Rodrigues */
/* DRE: 118143203 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 4 //numero de threads

/* variáveis globais */
int ordem = 0; //variavel a ser incrementada por cada thread como condição de ordem e execução
pthread_mutex_t x_mutex;
pthread_cond_t x_cond, y_cond;

/* Thread 1 */
void *A (void *t) //printa depois da thread 2
  {

   pthread_mutex_lock (&x_mutex);
   if( ordem == 0 ) // bloqueia a thread atual se 'ordem' ainda nao tiver sido incrementado na thread 2
    {
     pthread_cond_wait(&x_cond, &x_mutex);
    }

   printf("Fique a vontade\n");
   ordem++;
   if(ordem==3) // libera a thread que estiver bloqueada pela condicional se ordem < 3
    {
     pthread_cond_signal(&y_cond);
    }

   pthread_mutex_unlock(&x_mutex);
   pthread_exit(NULL);
  }


/* Thread 2 */
void *B (void *t) //primeira thread a imprimir, incrementa 'ordem' para liberar as threads 1 e 4
  {

   printf("Seja bem-vindo!\n");
   pthread_mutex_lock(&x_mutex);
   ordem++;
   pthread_cond_broadcast(&x_cond); // desbloqueia a thread que esteja bloqueada pela condicional
   pthread_mutex_unlock(&x_mutex);
   pthread_exit(NULL);
  }

/* Thread 3 */
void *C (void *t) // so pode printar depois da thread 1 e 4
  {

   pthread_mutex_lock(&x_mutex);
   while(ordem!=3) // não libera a thread para imprimir enquanto ordem não for incrementado por thread 1 e 4
   {
       pthread_cond_wait(&y_cond, &x_mutex);
   }
   printf("Volte Sempre!\n");
   pthread_mutex_unlock(&x_mutex);
   pthread_exit(NULL);
  }


/* Thread 4 */
void *D (void *t) //printa depois da thread 2
  {

   pthread_mutex_lock (&x_mutex);
   if( ordem == 0 ) // bloqueia a thread atual se 'ordem' ainda nao tiver sido incrementado na thread 2
    {
     pthread_cond_wait(&x_cond, &x_mutex);
    }
   printf("Sente-se, por favor!\n");

   ordem++;
   if(ordem==3) // libera a thread que estiver bloqueada pela condicional se ordem < 3
    {
     pthread_cond_signal(&y_cond);
    }

   pthread_mutex_unlock(&x_mutex);
   pthread_exit(NULL);
  }




int main ()
  {

   pthread_t threads[NTHREADS];
   pthread_mutex_init(&x_mutex, NULL); // inicializa o mutex
   pthread_cond_init (&x_cond, NULL); // inicializa a condicional

   pthread_create(&threads[0], NULL, A, NULL); //criação das threads
   pthread_create(&threads[1], NULL, B, NULL);
   pthread_create(&threads[2], NULL, C, NULL);
   pthread_create(&threads[3], NULL, D, NULL);

   for (int i = 0; i < NTHREADS; i++) ///Espera as threads completarem
   {
    pthread_join(threads[i], NULL);
   }

   printf ("fim.");

   pthread_mutex_destroy(&x_mutex); // desalocando as variaveis
   pthread_cond_destroy(&x_cond);
   pthread_cond_destroy(&y_cond);

   return 0;
  }
