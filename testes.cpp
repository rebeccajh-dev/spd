#include <stdio.h>
#include <string.h>
#include <omp.h>
#include "paralelism.cpp"

#define MAX_N 9

//faz a checagem se o teste passou
static int check(const char *nome, int *got, int *esperado, int n) {
    for (int i = 0; i < n; i++) {
        if (got[i] != esperado[i]) {
            printf("  [FALHOU] %s: c[%d] = %d, esperado %d\n",
                   nome, i, got[i], esperado[i]);
            return 0;
        }
    }
    printf("  [OK]     %s\n", nome);
    return 1;
}

void rodar_testes(void) {
    int total  = omp_get_max_threads(); //numero total de nucleos disponiveis(12)
    int metade = total / 2 > 0 ? total / 2 : 1; //metade dos nucleos disponiveis
    int c[MAX_N];
    int ok = 0, total_t = 0;

    printf("\n========== SUITE DE TESTES ==========\n");

    // T1: soma básica, verifica se a soma esta correta sem nenhum erro (coloca a sequencial aqui)
    {
        int a[] = {1,2,3,4}, b[] = {5,6,7,8};
        int esp[] = {6,8,10,12};
        memset(c, 0, sizeof c);
        omp_set_num_threads(total);
        #pragma omp parallel for
        for (int i = 0; i < 4; i++) c[i] = a[i] + b[i];
        ok += check("T1 soma basica {1,2,3,4}+{5,6,7,8}", c, esp, 4);
        total_t++;
    }

    // T2: apenas zeros
    {
        int a[] = {0,0,0,0}, b[] = {0,0,0,0};
        int esp[] = {0,0,0,0};
        memset(c, 0, sizeof c);
        omp_set_num_threads(total);
        #pragma omp parallel for
        for (int i = 0; i < 4; i++) c[i] = a[i] + b[i];
        ok += check("T2 zeros", c, esp, 4);
        total_t++;
    }

    // T3: numeros negativos
    {
        int a[] = {-1,-2,-3,-4}, b[] = {1,2,3,4};
        int esp[] = {0,0,0,0};
        memset(c, 0, sizeof c);
        omp_set_num_threads(total);
        #pragma omp parallel for
        for (int i = 0; i < 4; i++) c[i] = a[i] + b[i];
        ok += check("T3 negativos", c, esp, 4);
        total_t++;
    }

    // T4: Tamanhos diferentes
    {
        int a[] = {1,2,3,4}, b[] = {5,6,7,8,9};
        int esp[] = {6,8,10,12};
        int n = 4;
        memset(c, 0, sizeof c);
        omp_set_num_threads(total);
        #pragma omp parallel for
        for (int i = 0; i < n; i++) c[i] = a[i] + b[i];
        ok += check("T4 tamanhos diferentes (ignora b[4]=9)", c, esp, n);
        total_t++;
    }

    // T5: Todos vs Metada -> Diferentemente do anterior, ele testa a consitência de tempos
    // entre a diferença de threads
    {
        int a[] = {10,20,30,40}, b[] = {1,2,3,4};
        int c2[4];
        memset(c,  0, sizeof c);
        memset(c2, 0, sizeof c2);

        // sequencial (faz a divisão assim)

        // paralelo

        omp_set_num_threads(total);
        #pragma omp parallel for
        for (int i = 0; i < 4; i++) c[i]  = a[i] + b[i];

        omp_set_num_threads(metade);
        #pragma omp parallel for
        for (int i = 0; i < 4; i++) c2[i] = a[i] + b[i];

        int igual = memcmp(c, c2, 4 * sizeof(int)) == 0;
        printf("  %s T5 resultados identicos com %d e %d thread(s)\n",
               igual ? "[OK]    " : "[FALHOU]", total, metade);
        ok += igual;
        total_t++;
    }

    // T6: Vetor com apenas um elemento
    {
        int a[] = {42}, b[] = {58};
        int esp[] = {100};
        memset(c, 0, sizeof c);
        omp_set_num_threads(total);
        #pragma omp parallel for
        for (int i = 0; i < 1; i++) c[i] = a[i] + b[i];
        ok += check("T6 vetor de 1 elemento", c, esp, 1);
        total_t++;
    }

    // T7: valores grandes
    {
        int a[] = {1000000, 2000000};
        int b[] = {3000000, 4000000};
        int esp[] = {4000000, 6000000};
        memset(c, 0, sizeof c);
        omp_set_num_threads(total);
        #pragma omp parallel for
        for (int i = 0; i < 2; i++) c[i] = a[i] + b[i];
        ok += check("T7 valores grandes", c, esp, 2);
        total_t++;
    }

    printf("\n-------------------------------------\n");
    printf("  Resultado: %d/%d testes passaram\n", ok, total_t);
    printf("=====================================\n");
}

int main(void) {
    int total  = omp_get_max_threads();
    int metade = total / 2 > 0 ? total / 2 : 1;

    printf("Sistema: %d nucleo(s) logico(s) detectado(s)\n", total);
    printf("Configuracao: todos=%d  metade=%d\n", total, metade);

    int a[] = {1,2,3,4};
    int b[] = {5,6,7,8,9};
    int c[MAX_N];

    printf("\n========== EXECUCAO PRINCIPAL ==========");
    memset(c, 0, sizeof c); soma_paralela(a, b, c, 4, 5, total,  "TODOS os nucleos");
    memset(c, 0, sizeof c); soma_paralela(a, b, c, 4, 5, metade, "METADE dos nucleos");

    rodar_testes();
    return 0;
}
