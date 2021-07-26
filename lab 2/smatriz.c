#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"


float *mat1, *mat2, *saida;
int nthreads, dim;

typedef struct{
		int id;
}Thread;

int corretude(float *saida){
	int check=0;
	for(int linha=0; linha<dim; linha++){
    	for(int coluna=0; coluna<dim; coluna++){
    		if(saida[linha*dim+coluna]!=dim*2){
    			check=1;
    		}
     		//printf("%f ", saida[linha*dim+coluna]);
     	}
    //printf("\n");
  	}
  	return check;
}

void * mult(void *arg){
	int somaprod;
	Thread *args= (Thread *) arg;
	printf("Thread %d inicializada\n",args->id);	
	for(int linha=0; linha<dim; linha++){
		if(saida[linha*dim]==0){
		    for(int coluna=0; coluna<dim; coluna++){
		      somaprod=0;
		      for(int i=0; i<dim; i++) somaprod+=mat1[linha*dim+i]*mat2[i*dim+coluna];
		      saida[linha*dim+coluna]=somaprod;
	    	}
		}
	}	
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	float INC,FM,inicio,fim,delta;
	GET_TIME(INC);
	pthread_t *tid;
	Thread *args;
	if(argc<3){
		printf("input dimensão da matriz e quantidade de threads");
		return 1;
	}

	dim=atoi(argv[1]);
	nthreads=atoi(argv[2]);

	GET_TIME(inicio);

	mat1=(float *) malloc(sizeof(float) * dim * dim);
	mat2=(float *) malloc(sizeof(float) * dim * dim);
	saida=(float *) malloc(sizeof(float) * dim * dim);

	if(mat1==NULL || mat2==NULL || saida==NULL){
		printf("ERRO--malloc");
		return 2;
	}


	for (int i = 0; i < dim; ++i)
	{
		for (int j = 0; j < dim; ++j)
		{
			mat1[i*dim+j]=1;
			mat2[i*dim+j]=2;
			saida[i*dim+j]=0;
		}
	}
	GET_TIME(fim);

	delta=fim-inicio;
	printf("inicializacao:%f.3\n",delta );

	GET_TIME(inicio);
	tid=(pthread_t*) malloc(sizeof(pthread_t)*nthreads);
	args=(Thread*) malloc(sizeof(Thread*)*nthreads);
	if (tid==NULL || args==NULL){
		printf("ERRO--malloc");
		return 2;
	}
	for (int i = 0; i < nthreads; ++i){
		(args+i)->id=i;
		if(pthread_create(tid+i, NULL, mult, (void*) (args+i))){
			printf("ERRO--pthread_create\n");
			return 3;
		}
	}	

	for (int i = 0; i < nthreads; ++i)
	{
		pthread_join(*(tid+i), NULL);
	}
	GET_TIME(fim);
	delta=fim-inicio;
	printf("Criação e execução das threads:%f.3\n",delta );

	if(corretude(saida)==0){
  		puts("Correto!");
  	}
  	else{
  		printf("incorreto, os valores deveriam ser %d", dim*2);
  	}

	free(mat1);
	free(mat2);
	free(saida);
	free(tid);
	free(args);

	GET_TIME(FM);
	delta=FM-INC;
	printf("Finalização 100%:%f.3\n",delta );

	return 0;
}
