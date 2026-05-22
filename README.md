# Alunos
**Ana Klissia Furtado Martins**  
**Rebecca Esmirna Ribeiro dos Reis**

# Descrição
Este projeto implementa operações de soma de vetores em C++, utilizando duas abordagens:

- Soma sequencial baseada em redução por chunks;
- Soma paralela utilizando OpenMP.

O objetivo é comparar o comportamento da execução sequencial com a paralela, analisando desempenho, divisão de trabalho entre threads/núcleos e consistência dos resultados.

A versão paralela utiliza OpenMP para distribuição automática das tarefas entre múltiplos núcleos de processamento, enquanto a versão sequencial simula a mesma estratégia de chunking sem paralelismo real.

# Tecnologias Utilizadas
Linguagem: C++  

Bibliotecas:
- OpenMP → paralelismo em memória compartilhada;
- chrono → medição de tempo;
- stdio/string → operações auxiliares;
- omp.h → gerenciamento das threads OpenMP.

# Estrutura do Projeto

- `paralelism.cpp` → implementação da soma paralela;
- `sequencial.cpp` → implementação da soma sequencial;
- `testes.cpp` → suíte de testes comparando as duas abordagens;
- `README.md` → documentação do projeto.

# Referências

- DIEHL, P. *Advanced Parallel Programming in C++*. Disponível em: <https://www.diehlpk.de/blog/modern-cpp/>.

- GEEKSFORGEEKS. *Introduction to Parallel Programming with OpenMP in C++*. Disponível em: <https://www.geeksforgeeks.org/cpp/introduction-to-parallel-programming-with-openmp-in-cpp/>.

- INTEL MODERN CODE PARTNER OPENMP - Aula 01. Disponível em: <https://www.inf.ufrgs.br/gppd/intel-modern-code/slides/workshop-2/MCP_Pt2_Pratica.pdf>.

- BLELLOCH, G. E. *Prefix Sums and Their Applications*. Disponível em: <https://www.cs.cmu.edu/~blelloch/papers/Ble93.pdf>.

- CHAPMAN, B.; JOST, G.; VAN DER PAS, R. *Using OpenMP: Portable Shared Memory Parallel Programming*. MIT Press,.

- CORMEN, T. H.; LEISERSON, C. E.; RIVEST, R. L.; STEIN, C. *Introduction to Algorithms*. 3. ed. MIT Press.

# Como Compilar

## Linux / macOS

```bash
g++ -fopenmp -O2 testes.cpp -o testes
./testes