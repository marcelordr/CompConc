#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define buffersize 5   //tamanho do buffer
#define NPROD 1  //número de produtores
#define NCONS 1  //numero de consumidores

int buf[buffersize];
int indice = 0;
sem_t condProd, condCons, mutex;

//função que realiza a intereção do produtor com o Buffer
void produz(int id, int ind)
{
  buf[ind]=1;
  printf("Produtor %d produziu\n", id+1); //log de execução
  sem_post(&condProd); //produtor avisa que já acabou de produzir
}


 
void* produtor(void* arg) //função executada pelo produtor
{
  int id = *(int*)arg;
  
  while(1)
   {
    sem_wait(&mutex); //entrada de seção crítica
    if(indice<buffersize-1)
    {
      printf("Produtor %d produzirá em buf[%d]\n", id+1, indice); //log de execução
      int ind = indice; //variável necessária para que a função produz() seja chamada fora do trecho com exclução mútua
      indice++;
      sem_post(&mutex);//saída da seção crítica
      produz(id,ind); //chama a função que produz
    }
    else  //produtor que for trabalhar coma última posição do buffer
     {         
      printf("Produtor %d produzirá em buf[%d]\n", id+1, indice);  //log de execução
      produz(id,indice);  //chama a função que produz
      for(int i=0; i<NPROD ; i++) //espera todas as produtoras acabarem as suas tarefas
      {  
        sem_wait(&condProd);
      }
      sem_post(&condCons); //libera as threads consumidoras
    }
  }
  pthread_exit(NULL);
}

//função executada pelo consumidor
void* consumidor(void* arg)
{
  int id = *(int*)arg;
  while(1)
  {
    sem_wait(&condCons);
    printf("Consumidor %d\n", id+1);
    for(int i=buffersize-1; i>=0 ; i--)
    {
      buf[i]=0;
    }
    indice=0;
    sleep(1);
    sem_post(&mutex);
  }
  pthread_exit(NULL);
}

int main(int argc, char const *argv[]) 
{

  pthread_t pid[NPROD]; //id das threads
  pthread_t cid[NCONS];
  sem_init(&condProd, 0, NPROD); //init dos semaforos
  sem_init(&condCons, 0, 0);
  sem_init(&mutex, 0, 1);

  int i;
  int idp[NPROD]; //argumentos das funções executadas pelas threads
  int idc[NCONS];
  
  for(i=0 ; i<NPROD ; i++)
  {
    idp[i]=i;
    if(pthread_create(&pid[i], NULL, produtor,(void*) &idp[i]))
      {printf("--ERRO: pthread_create()\n"); return 1;}
  }
  for(i=0 ; i<NCONS ; i++)
  {
    idc[i]=i;
    if(pthread_create(&cid[i], NULL, consumidor,(void*) &idc[i]))
      {printf("--ERRO: pthread_create()\n"); return 1;}
  }


  //espera término das threads
  for (i=0; i<NPROD; i++) 
  {
    if (pthread_join(pid[i], NULL))
      {printf("--ERRO: pthread_join() \n"); return 2;}
  }
  for (i=0; i<NCONS; i++)
  {
    if (pthread_join(cid[i], NULL))
      {printf("--ERRO: pthread_join() \n"); return 2;}
  }

  return 0;
}
