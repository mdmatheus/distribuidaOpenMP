#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
 
void merge (int *a, int n, int m) {
    int i, j, k;
    int *x = malloc(n * sizeof (int));
    i = 0;
    j = m;
    for (k = 0; k < n; k++) {
        x[k] = j == n      ? a[i++]
             : i == m      ? a[j++]
             : a[j] < a[i] ? a[j++]
             :               a[i++];
    }
    for (i = 0; i < n; i++) {
        a[i] = x[i];
    }
    free(x);
}
 
void merge_sort (int *a, int n) {
    if (n < 2)
        return;
    int m = n / 2;
    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp task
            merge_sort(a, m);
            #pragma omp task
            merge_sort(a + m, n - m);
            #pragma omp taskwait
            merge(a, n, m);
        }
    }
}
 
int main (int argc, char** argv) {
    double startt, endt;
    if(argc < 3){
      printf("Uso: %s <tamanho array> <numero de processadores> \n", argv[0]);
      exit(1);
    }

    printf("Número de processadores da máquina: %d\n\n", omp_get_max_threads());

    /* tamanho do array */
    int tamanho = atoi(argv[1]);
    int nprocs = atoi(argv[2]);

    /* alocacao do array A */
    int *a = (int *) malloc(sizeof(int) * tamanho);

    /* inicializacao do array */
    printf("Gerando array com %d números inteiros...\n", tamanho);
    startt = omp_get_wtime();
    FILE* urandom = fopen("/dev/urandom", "rb");
    fread(a, tamanho, sizeof(int), urandom);
    fclose(urandom);
    endt = omp_get_wtime();
    printf("Espaço total do array: %f MB\n", sizeof(int) * tamanho / (1024.0 * 1024.0));
    printf("Tempo para gerar o array: %f msec\n\n", (endt-startt)*1000);
    
    int n = tamanho;
    int i;
    printf("Ordenando com %d threads\n", nprocs);
    omp_set_num_threads(nprocs);
    //for (i = 0; i < n; i++)
    //    printf("%d%s", a[i], i == n - 1 ? "\n" : " ");
    startt = omp_get_wtime();
    merge_sort(a, n);
    endt = omp_get_wtime();
    printf("Tempo ordenar: %f msec\n\n", (endt-startt)*1000);
    //for (i = 0; i < n; i++)
    //    printf("%d%s", a[i], i == n - 1 ? "\n" : " ");
    printf("Checando ordenacao\n");
    startt = omp_get_wtime();
    for (i = 1; i < n; i++) {
        if (a[i] < a[i-1]) {
            printf("ERRO NA ORDENACAO\n");
            return -1;
        }
    }
    endt = omp_get_wtime();
    printf("Tempo checar: %f msec\n\n", (endt-startt)*1000);
    printf("Ordenacao OK\n");
    return 0;
}
