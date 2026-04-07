// 1. Faça um sistema Produtor x Consumidor

#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <queue>
#include <random>
#include <thread>
#include <vector>

using namespace std;

// consumidor --> c-- (remove de posição que já foi consumida)
// produtor --> c++ (insere em posição em que não foi consumida)

class Buffer {
private:
  queue<int> queueOne;
  int capacity;

  mutex mtx;
  condition_variable not_full; //faz a verificação se o buffer está cheio
  condition_variable not_empty; //faz a verificação se o buffer está vazio

public:
  Buffer(int capacity) : capacity(capacity) {}
  void produce(int item, int id) {
    // trava o acesso ao buffer
    unique_lock<mutex> lock(mtx);

    //se estiver cheio, ele espera
    not_full.wait(lock, [&]() { return queueOne.size() < capacity; });

    queueOne.push(item);
    cout << "Produtor " << id << " produziu: " << item << endl;
    
    // avisa um consumidor que agora existe item disponível
    not_empty.notify_one();
  }

  int consume(int id) {
    // trava o acesso ao buffer
    unique_lock<mutex> lock(mtx);
    //mesma coisa: se estiver vazio, ele espera
    not_empty.wait(lock, [&]() { return !queueOne.empty(); });
    int item = queueOne.front();
    queueOne.pop();

    cout << "Consumidor " << id << " consumiu: " << item << endl;
    
    // avisa o produtor que agora ele tem espaço livre
    not_full.notify_one();

    return item;
  }
};

// a. P threads produtoras, P configurável.
void producer(Buffer &buffer, int id, int items) {
  for (int i = 0; i < items; i++) {
    this_thread::sleep_for(chrono::milliseconds(rand() % 1000));
    buffer.produce(i, id);
  }
};

  // b. C threads consumidoras, C configurável.
void consumer(Buffer & buffer, int id, int items) {
  for (int i = 0; i < items; i++) {
    this_thread::sleep_for(chrono::milliseconds(rand() % 1000));
    buffer.consume(id);
  }
};

// 1. Faça um sistema Produtor x Consumidor
void run_test(int numeroProdutores, int numeroConsumidores, int tamanhoBuffer) {
  cout << "Teste: P=" << numeroProdutores
       << " | C=" << numeroConsumidores
       << " | T=" << tamanhoBuffer << endl;

  Buffer buffer(tamanhoBuffer);

  vector<thread> producers;
  vector<thread> consumers;

  int items_per_producer = 10;

  // total de itens produzidos
  int total_items = numeroProdutores * items_per_producer;

  // divide corretamente entre consumidores 
  int items_per_consumer = total_items / numeroConsumidores;

  // criando produtores
  for (int i = 0; i < numeroProdutores; i++) {
    producers.emplace_back(producer, ref(buffer), i, items_per_producer);
  }

  // criando consumidores
  for (int i = 0; i < numeroConsumidores; i++) {
    consumers.emplace_back(consumer, ref(buffer), i, items_per_producer);
  }

  for (auto &t : producers) {
    t.join();
  }

  for (auto &t : consumers) {
    t.join();
  }

  cout << "Teste finalizado\n";
}

int main() {
    
  // 4 a) P == C >= 10
  run_test(10, 10, 1);  // buffer mínimo 
  run_test(10, 10, 5);  // buffer maior

  // 4 b) P == 2C >= 10
  run_test(20, 10, 5);

  // 4 c) C == 2P >= 10
  run_test(10, 20, 5);

  cout << "\nTodos os testes finalizaram.\n";
  return 0;
  
}
// c. Buffer compartilhado de tamanho T configurável.

