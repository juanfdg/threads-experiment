#include <time.h>
#include <stdio.h>
#include <pthread.h>

int main() {
  const int MAXN = 1000000;
  const int MAXT = 100;
  int array[MAXN];
  int n;
  int p;

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
  for (int i=0;i<=n/2;++i) {
    array[i] ^= array[n-i-1] ^= array[i] ^= array[n-i-1];
  }
  clock_t end = clock();

  printf("Array final:\n");
  for (int i=0; i<n; i++) {
    printf("%d\n", array[i]);
  }
  printf("\n");

  printf("Tempo: %g ms\n", 1000*(double)(end-start)/CLOCKS_PER_SEC);

  return 0;
}
