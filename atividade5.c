/**********************************************
Aluno: Marcelo Augusto de Jesus Rodrigues
Professora: Silvana Rossetto
Disciplina: Computação Concorrente
Objetivo:Implementar um programa concorrente, com duas threads (alem da thread
principal), para elevar ao quadrado cada elemento de um vetor de 10000 elementos.

**********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TAM 10000
#define NTHREADS 2

int vetor[TAM];
int vetorAux[TAM];

typedef struct { // ira armazenar argumentos da thread
   int inicioThread;
   int fimThreads;
} t_Args;

void *vetorQuadrado (void *arg) { //função realizada pelas threads
  
  int ident = * (int *) arg; //type cast para int
  
  for(int i = ident; i < TAM; i+=2){
    
	  vetor[i] = (i+1)*(i+1);
    
  }
  
  pthread_exit(NULL);
  
}

void testaValor(){ //checa se o vetor esta correto
    
    for(int i = 0; i<TAM; i++){
      
        if(vetorAux[i] != vetor[i]){
            printf("Algum valor esta incorreto.\n");
            printf("\n");
        }
        else{
            printf("O vetor esta correto!\n");
            printf("\n");
        }
      
    }
 
}
  
void printaVetor(){ //imprime o vetor
  
    for(int i = 0; i < TAM; i++){
      
        printf("%d\n", vetor[i]);
    }
}

int main(void){ // função principal
  
  pthread_t tid[NTHREADS]; // identificador da thread no sistema
  int ident[NTHREADS]; // identificador da thread local

  for(int i = 0; i<NTHREADS ; i++){  // threads criadas com o identificador que define a paridade passada como parametro 
    
    ident[i] = i;

    if(pthread_create(&tid[i],NULL,vetorQuadrado,(void*) &ident[i]))
        printf("Erro pthread_create\n");
  }
  
  for (int i = 0; i < NTHREADS; i++){      
  
      if (pthread_join(tid[i], NULL)) //espera as threads terminarem
          printf("ERRO -- pthread_join\n");    
   }
          
          
    //printaVetor(); //chama a função que imprime o vetor

    //(testaValor(); //chama a função que imprime o resultado da checagem
       
      
  pthread_exit(NULL);
  return 0;
}
