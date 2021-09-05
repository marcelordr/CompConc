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

int N, NTHREADS, primo[NMAX+1], prox, x; // variaveis globais

pthread_t id [MAX_NTHREADS]; //ID das threads
pthread_mutex_t proxLock = PTHREAD_MUTEX_INITIALIZER; // lock para a variavel 'prox' acessada pelas threads

void elimina(int x) //elimina os multiplos impares a variavel
{  
    int i;
    for (i = 3; i*x <= N; i += 2)
    {
        primo[i*x] = 0;
    }
}

void *tarefa(void* threadN)  {  
    int lim,base;
    int tarefa = 0;  // trabalho feito pela thread
    lim = sqrt(N);
    do  {

      pthread_mutex_lock(&proxLock); //bloqueia o acesso
      base = prox;
      prox += 2;
      
      pthread_mutex_unlock(&proxLock); //libera o acesso 

      if (base <= lim)  {
         if (primo[base])  {
            elimina(base);
            tarefa++;  
         }
      }
      else{
          return (void *) tarefa;
      }
   } while (1);
}

int main(int argc, char *argv[]){

    int resol, qnt_prime; // numero de primos achados
    
    if(argc < 3) //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads) 
    {
        fprintf(stderr, "Digite: %s <dimensao sequencia> <quantidade de threads>\n", argv[0]);
        return 1; 
    }
    N = atoi(argv[1]);
    NTHREADS = atoi(argv[2]);
    
    clock_t tempo1 = clock();

    for (int i = 3; i <= N; i++) // primos 1 ate serem validados 
    {
       if (i%2 == 0) primo[i] = 0;
       else {
           primo[i] = 1;
       }
    }

prox = 3;
    
    for (int i = 0; i < NTHREADS; i++) // dá inicio as threads
    {
        pthread_create(&id[i],NULL,tarefa,NULL);
    }

    
    for (int i = 0; i < NTHREADS; i++)  //espera as threads terminarem
    {
        pthread_join(id[i],&qnt_prime);
        ("%d valor retornado pela thread\n",resol);
    }
    tempo1 = clock() - tempo1;
    double tempoExecucao1 = (double)(tempo1) / CLOCKS_PER_SEC;
    printf("\nTempo concorrente: %f\n", tempoExecucao1);


    // resultados
    qnt_prime = 1;
    for (int i = 3; i <= N; i++)
       if (primo[i])  {
          qnt_prime++;
       }
    printf("o numero de primos achados foi: %d\n",qnt_prime);
    
}
