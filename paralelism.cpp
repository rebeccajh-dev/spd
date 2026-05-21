#include <stdio.h>
#include <omp.h>

//DISCLAIMER: Foi adaptado o uso de threads para o funcionamento dos núcleos, visto que
//cada thread está funcionando em um núcleo específico.


//soma paralela, declaração dos nucleos que serao usados e uso do omp parallel
void soma_paralela(int *a, int *b, int *c, int na, int nb,
                   int threads, const char *label) {
    int n = na < nb ? na : nb;
    int nucleos_usados = 0;
    omp_set_num_threads(threads);

    double t0 = omp_get_wtime();

    #pragma omp parallel //regiao paralela com um numero padrao de threads
    {
        #pragma omp single
        nucleos_usados = omp_get_num_threads(); //retorna o ID das threads

        #pragma omp for schedule(static)
        for (int i = 0; i < n; i++) {
            c[i] = a[i] + b[i];
            printf("  [nucleo %d/%d] c[%d] = %d + %d = %d\n",
                   omp_get_thread_num() + 1,
                   omp_get_num_threads(),
                   i, a[i], b[i], c[i]);
        }
    }// fim da região paralela

    double t1 = omp_get_wtime(); //utilizado para medir o tempo do wall-clock em segundos

    printf("\n[%s | %d nucleo(s)]\n", label, nucleos_usados);
    printf("  Resultado: { ");
    for (int i = 0; i < n; i++) printf("%d ", c[i]);
    printf("}\n");

    if (nb > na) {
        printf("  b[%d] = %d nao tem par em a "
               "(elemento ignorado)\n", na, b[na]);
    }

    printf("  Tempo: %.6f ms\n", (t1 - t0) * 1000.0);
}
