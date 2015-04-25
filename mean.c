#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/**
 * Calcula a média de um array de números reais com o número de processadores especificado
 */

int main(int argc, char **argv) {
  if(argc < 3){
    printf("Uso: %s <tamanho array> <numero de processadores> \n", argv[0]);
    exit(1);
  }

  printf("Número de processadores da máquina: %d\n\n", omp_get_max_threads());

  /* tamanho do array */
  int tamanho = atoi(argv[1]);
  int nprocs = atoi(argv[2]);

  /* alocacao do array A */
  float *A = (float *) malloc(sizeof(float) * tamanho);

  /* inicializacao do array */
  printf("Gerando array com %d números reais...\n", tamanho);
  int i;
  for (i = 0; i < tamanho; i++) {
    A[i] = i;
  }
  printf("Espaço ocupado por um float: %lu bytes\n", sizeof(float));
  printf("Espaço total do array: %f MB\n\n", sizeof(float) * tamanho / (1024.0 * 1024.0));

  omp_set_num_threads(nprocs);
  double soma = 0;
  #pragma omp parallel
  {

    #pragma omp single
    printf("Calculando média com %d processadores...\n", omp_get_num_threads());

    #pragma omp for reduction(+: soma)
    for(i = 0; i < tamanho; i++) {
      soma += A[i];
    }
  }

  double mean = soma / tamanho;
  printf("Média dos valores: %f\n", mean);

  return 0;
}
