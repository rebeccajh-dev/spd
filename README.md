# Alunos
**Ana Klissia Furtado Martins**  
**Rebecca Esmirna Ribeiro dos Reis**


# Descrição
Este projeto implementa o clássico problema de produtor-consumidor utilizando multithreading na linguagem C++.
O sistema consiste em múltiplas threads produtoras e consumidoras que interagem por meio de um buffer compartilhado e de tamanho configurável, respeitando limites de capacidade e evitando condições de corrida.

# Tecnologias Utilizadas
Linguagem: C++  
Bibliotecas: **- condition variable** > É usado junto com o mutex para bloquear uma ou mais threads até que todas estejam modificadas e enviem uma notificação (usando o notify_one/notify_all);    
**Thread** > Permite que múltiplas funções ocorram simultaneamente;  
**Mutex** > Utilizada para proteger dados compartilhados contra acessos simultâneos por várias threads.  

# Referências
**GEEKSFORGEEKS. Producer Consumer Problem and its Implementation with C++. Disponível em: <https://www.geeksforgeeks.org/operating-systems/producer-consumer-problem-and-its-implementation-with-c/>.**  
**GEEKSFORGEEKS. Sleep Function in C++. Disponível em: <https://www.geeksforgeeks.org/cpp/sleep-function-in-cpp/>.**  
**C++ reference - cppreference.com. Disponível em: <https://en.cppreference.com/w/cpp.html>.**  
