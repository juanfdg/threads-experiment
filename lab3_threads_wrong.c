/** Programa para retornar se um número N é perfeito ou não,
 ** iterando pelos números de 1 a N de com processamento
 ** dividido entre P threads, detectando quais são os fatores
 ** de N
 */

#define MAX_THREADS 100
#define MAX_ARRAY 1000000
#define true  1
#define false 0
#define WINAPI

#include <time.h>
#include <stdio.h>
#include <pthread.h>

// Funcoes e variaveis do problema
int n, p;
int iter;
int array[MAX_ARRAY];

// Funcoes e variaveis das threads
pthread_cond_t handleThread[MAX_THREADS];
pthread_t threadId[MAX_THREADS];

void* WINAPI threadFunc(void * lpparam){
	int i, aux;
	while (true) {
		// Condição de corrida das threads no iterador do array
		i = iter++;
		// printf("i = %d\n", i);

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
