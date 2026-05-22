#include <stdio.h>
#include <string.h>
#include <omp.h>

#include "paralelism.cpp"
#include "sequencial.cpp"

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
        int cSeq[4];
        int cPar[4];

        memset(cSeq, 0, sizeof cSeq);
        memset(cPar, 0, sizeof cPar);
        soma_sequencial(a, b, cSeq, 4, 4);
        omp_set_num_threads(total);
        #pragma omp parallel for
        for (int i = 0; i < 4; i++) {
            cPar[i] = a[i] + b[i];
        }

        ok += check("T1 sequencial {1,2,3,4}+{5,6,7,8}", cSeq, esp, 4);
        ok += check("T1 paralelo {1,2,3,4}+{5,6,7,8}", cPar, esp, 4);
        total_t += 2;
    }

    // T2: apenas zeros
    {
        int a[] = {0,0,0,0}, b[] = {0,0,0,0};
        int esp[] = {0,0,0,0};
        int cSeq[4];
        int cPar[4];

        memset(cSeq, 0, sizeof cSeq);
        memset(cPar, 0, sizeof cPar);
        soma_sequencial(a, b, cSeq, 4, 4);
        omp_set_num_threads(total);
        #pragma omp parallel for
        for (int i = 0; i < 4; i++) {
            cPar[i] = a[i] + b[i];
        }
        ok += check("T2 sequencial", cSeq, esp, 4);
        ok += check("T2 paralelo", cPar, esp, 4);
        total_t += 2;
    }

    // T3: numeros negativos
    {
        int a[] = {-1,-2,-3,-4}, b[] = {1,2,3,4};
        int esp[] = {0,0,0,0};
        int cSeq[4];
        int cPar[4];

        memset(cSeq, 0, sizeof cSeq);
        memset(cPar, 0, sizeof cPar);
        soma_sequencial(a, b, cSeq, 4, 4);
        omp_set_num_threads(total);
        #pragma omp parallel for
        for (int i = 0; i < 4; i++) {
            cPar[i] = a[i] + b[i];
        }
        ok += check("T3 sequencial negativos", cSeq, esp, 4);
        ok += check("T3 paralelo negativos", cPar, esp, 4);
        total_t += 2;
    }

    // T4: Tamanhos diferentes
    {
        int a[] = {1,2,3,4}, b[] = {5,6,7,8,9};
        int esp[] = {6,8,10,12};
        int n = 4;
        int cSeq[4];
        int cPar[4];

        memset(cSeq, 0, sizeof cSeq);
        memset(cPar, 0, sizeof cPar);
        soma_sequencial(a, b, cSeq, 4, 5);
        omp_set_num_threads(total);
        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            cPar[i] = a[i] + b[i];
        }
        ok += check("T4 sequencial tamanhos diferentes (ignora b[4]=9)", cSeq, esp, n);
        ok += check("T4 paralelo tamanhos diferentes (ignora b[4]=9)", cPar, esp, n);
        total_t += 2;
    }

    // T5: Todos vs Metada -> Diferentemente do anterior, ele testa a consitência de tempos
    // entre a diferença de threads
    {
        int a[] = {10,20,30,40}, b[] = {1,2,3,4};

        int cTodos[4];
        int cMetade[4];
        int cSeq[4];

        memset(cTodos, 0, sizeof cTodos);
        memset(cMetade, 0, sizeof cMetade);
        memset(cSeq, 0, sizeof cSeq);

        // sequencial 
        soma_sequencial(a, b, cSeq, 4, 4);

        // paralelo
        omp_set_num_threads(total);
        #pragma omp parallel for
        for (int i = 0; i < 4; i++) {
            cTodos[i] = a[i] + b[i];
        }
        omp_set_num_threads(metade);

        #pragma omp parallel for
        for (int i = 0; i < 4; i++) {
            cMetade[i] = a[i] + b[i];
        }

        int igual1 =
            memcmp(
                cSeq,
                cTodos,
                4 * sizeof(int)
            ) == 0;

        int igual2 =
            memcmp(
                cTodos,
                cMetade,
                4 * sizeof(int)
            ) == 0;
         printf(
            "  %s T5 sequencial vs paralelo\n",
            igual1 ? "[OK]    "
                  : "[FALHOU]"
        );

        printf(
            "  %s T5 todos vs metade das threads\n",
            igual2 ? "[OK]    "
                  : "[FALHOU]"
        );
        ok += igual1;
        ok += igual2;
        total_t += 2;
    }

    // T6: Vetor com apenas um elemento
    {
        int a[] = {42}, b[] = {58};
        int esp[] = {100};
        int cSeq[1];
        int cPar[1];

        memset(cSeq, 0, sizeof cSeq);
        memset(cPar, 0, sizeof cPar);
        soma_sequencial(a, b, cSeq, 1, 1);
        omp_set_num_threads(total);
        #pragma omp parallel for
        for (int i = 0; i < 1; i++) {
            cPar[i] = a[i] + b[i];
        }
        ok += check("T6 sequencial vetor de 1 elemento", cSeq, esp, 1);
        ok += check("T6 paralelo vetor de 1 elemento", cPar, esp, 1);
        total_t += 2;
    }

    // T7: valores grandes
    {
        int a[] = {1000000, 2000000};
        int b[] = {3000000, 4000000};
        int esp[] = {4000000, 6000000};
        int cSeq[2];
        int cPar[2];

        memset(cSeq, 0, sizeof cSeq);
        memset(cPar, 0, sizeof cPar);
        soma_sequencial(a, b, cSeq, 2, 2);
        omp_set_num_threads(total);
        #pragma omp parallel for
        for (int i = 0; i < 2; i++) {
            cPar[i] = a[i] + b[i];
        }
        ok += check("T7 sequencial valores grandes", cSeq, esp, 2);
        ok += check("T7 paralelo valores grandes", cPar, esp, 2);
        total_t += 2;
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

    double t0, t1;

    printf("\n========== EXECUCAO SEQUENCIAL ==========\n");
    memset(c, 0, sizeof c);
    t0 = omp_get_wtime();
    soma_sequencial(a, b, c, 4, 5);
    t1 = omp_get_wtime();
    printf(
        "Tempo sequencial: %.6f ms\n",
        (t1 - t0) * 1000.0
    );

    printf("\n========== EXECUCAO PRINCIPAL ==========\n");
    memset(c, 0, sizeof c);
    soma_paralela( a, b, c, 4, 5, total, "TODOS os nucleos");
    memset(c, 0, sizeof c);
    soma_paralela( a, b, c, 4, 5, metade, "METADE dos nucleos");

    rodar_testes();

    return 0;
}