# Alunos
**Ana Klissia Furtado Martins**  
**Rebecca Esmirna Ribeiro dos Reis**


# Descrição
Este projeto implementa a solução um de redução/soma paralela em c++.
O sistema consiste em múltiplas threads paralelas responsáveis pela execução concorrente de operações sobre vetores compartilhados, utilizando OpenMP para distribuição das tarefas entre núcleos de processamento, garantindo consistência dos resultados e evitando condições de corrida.

# Tecnologias Utilizadas
Linguagem: C++  
Bibliotecas: **- OpeMP** > É usado para o paralelismo em memória compartilhada, facilitando a designação de núcleos para os processos desejados;      

# Referências
**DIEHL, P. Advanced Parallel Programming in C++. Disponível em: <https://www.diehlpk.de/blog/modern-cpp/>.**  
**INTEL MODERN CODE PARTNER OPENMP -AULA 01. [s.l: s.n.]. Disponível em: <https://www.inf.ufrgs.br/gppd/intel-modern-code/slides/workshop-2/MCP_Pt2_Pratica.pdf>.**  
**GEEKSFORGEEKS. Introduction to Parallel Programming with OpenMP in C++. Disponível em: <https://www.geeksforgeeks.org/cpp/introduction-to-parallel-programming-with-openmp-in-cpp/>.**  

# Como compilar o programa
g++ -fopenmp -O2 -o testes testes.cpp
testes.exe (windows) ./testes (Linux)
