#include <stdio.h>
#include <string.h>
#include <math.h>
#include <chrono>

// Soma vetorial sequencial simples
void soma_sequencial(int *a, int *b, int *c, int na, int nb) {
    int n = na < nb ? na : nb; //garante que o loop não acesse índices fora do limite dos vetores

    for (int i = 0; i < n; i++) {
        c[i] = a[i] + b[i];

        printf("  [sequencial] c[%d] = %d + %d = %d\n",
               i, a[i], b[i], c[i]);
    }
}
