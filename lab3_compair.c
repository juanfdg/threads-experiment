#define MAX_THREADS 100
#define MAX_ARRAY 1000000
#define WINAPI
#define true  1
#define false 0
#define debugtxt(FORMAT) printf(" TID %d: " #FORMAT "\n",(int) pthread_self())
#define debug(FORMAT, ARGS...) printf("TID %d: " #FORMAT "\n",(int) pthread_self(),ARGS)

#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>

// Funcoes e variaveis do problema
int n, p;
int iter;
int arrayThreads[MAX_ARRAY];
int arraySequential[MAX_ARRAY];

// Funcoes e variaveis das threads
pthread_cond_t handleThread[MAX_THREADS];
pthread_t threadId[MAX_THREADS];
sem_t iterMutex;
sem_t factorsMutex;

// Truque para sabermos qual o semaforo foi chamado e poder imprimi-lo
#define up(SEM) sem_post(SEM)
#define down(SEM) sem_wait(SEM)

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
		//printf("i = %d\n", i);
		up(&iterMutex);

		// Precisamos percorrer o array apenas até sua metade
		if (i > n/2) break;

		arrayThreads[i] ^= arrayThreads[n-i-1] ^= 
		arrayThreads[i] ^= arrayThreads[n-i-1];
	}
}

int main() {
	iter = 0;
	int sum = 0;

	sem_init(&iterMutex, 0, 1);
	sem_init(&factorsMutex, 0, 1);

	FILE* entry = fopen("entry.txt", "r");
	FILE* outThreads = fopen("out_threads.txt", "w"); 
	FILE* outSequential = fopen("out_sequential.txt", "w");

	printf("Digite N e o número de threads:\n");
	scanf("%d %d", &n, &p);

	for (int i=0; i<n; i++) {
		fscanf(entry, "%d ", &arrayThreads[i]);
		arraySequential[i] = arrayThreads[i];
	}
	fclose(entry);

	clock_t start, end;
    double cpuTimeUsedThreads;
	double cpuTimeUsedSequential;

	start = clock();
	for (int i=0; i<p; i++) {
		pthread_create (&threadId[i],
				NULL,
				threadFunc,
				NULL);
	}

	for(int i=0; i<p; i++) {
		pthread_join (threadId[i], NULL);		
	}
	end = clock();
	cpuTimeUsedThreads = ((double) (end - start)) / CLOCKS_PER_SEC;

	for (int i=0; i<n; i++) {
		fprintf(outThreads, "%d\n", arrayThreads[i]);
	}

	start = clock();
	int aux;
	for (int i=0; i < n/2+1; i++) {
		 arraySequential[i] ^= arraySequential[n-i-1] ^= 
		 arraySequential[i] ^= arraySequential[n-i-1];
	}
	end = clock();
	cpuTimeUsedSequential = ((double) (end - start)) / CLOCKS_PER_SEC;

	for (int i=0; i<n; i++) {
		fprintf(outSequential, "%d\n", arraySequential[i]);
	}

	printf("\n");
	printf("Tempo gasto com threads: %f\n\n", cpuTimeUsedThreads);
	printf("Tempo gasto sequencial: %f\n\n", cpuTimeUsedSequential);
	return 0;
}
