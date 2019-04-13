/** Programa para retornar se um número N é perfeito ou não,
 ** iterando pelos números de 1 a N de com processamento
 ** dividido entre P threads, detectando quais são os fatores
 ** de N
 */

#define MAX_THREADS 4
#define MAX_ARRAY 1000000
#define true  1
#define false 0
#define WINAPI
#define debugtxt(FORMAT) printf(" TID %d: " #FORMAT "\n",(int) pthread_self())
#define debug(FORMAT, ARGS...) printf("TID %d: " #FORMAT "\n",(int) pthread_self(),ARGS)

#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

// Funcoes e variaveis do problema
int n, p;
int iter;
int array[MAX_ARRAY];

// Funcoes e variaveis das threads
pthread_cond_t handleThread[MAX_THREADS];
pthread_t threadId[MAX_THREADS];
sem_t iterMutex;
sem_t factorsMutex;

// Truque para sabermos qual o semaforo foi chamado e poder imprimi-lo
#define up(SEM) _up(SEM,#SEM)
#define down(SEM) _down(SEM,#SEM)

void _up(sem_t *sem, const char * name) {
	debug("Up %s ...",name);
	sem_post(sem);
	debug("Up %s complete!",name);
}
void _down(sem_t *sem, const char * name) {
	debug("Down %s ...",name);
	sem_wait(sem);
	debug("Down %s complete!",name);
}

void* WINAPI threadFunc(void * lpparam){
	int i, aux;
	while (true) {
		// Condição de corrida das threads no iterador do array
		down(&iterMutex);
		i = iter++;
		// printf("i = %d\n", i);
		up(&iterMutex);

		// Precisamos percorrer o array apenas até sua metade
		if (i > n/2) break;

		aux = array[i];
		array[i] = array[n-i-1]; 
		array[n-i-1] = aux;
	}
}

int main() {
	iter = 0;
	int sum = 0;

	sem_init(&iterMutex, 0, 1);
	sem_init(&factorsMutex, 0, 1);

	FILE* entry = fopen("entry.txt", "r");

	printf("Digite N e o número de threads:\n");
	scanf("%d %d", &n, &p);

	printf("Array inicial:\n");
	for (int i=0; i<n; i++) {
		fscanf(entry, "%d ", &array[i]);
		printf("%d\n", array[i]);
	}
	printf("\n");

	fclose(entry);

        clock_t start = clock();
	for (int i=0; i<p; i++){
		pthread_create (&threadId[i],
				NULL,
				threadFunc,
				NULL);
	}

	for(int i=0; i<p; i++) {
		pthread_join (threadId[i], NULL);		
	}
        clock_t end = clock();

	printf("\nArray trocado:\n");
	for (int i=0; i<n; i++)
		printf("%d\n", array[i]);
	printf("\n");

        printf("Tempo: %g ms\n", 1000*(double)(end-start)/CLOCKS_PER_SEC);
	return 0;
}
