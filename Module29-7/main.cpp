//
//  main.cpp
//  Module29-7
//
//  Created by Ольга Полевик on 30.12.2021.
//

#include <iostream>
#include <future>
#include <thread>
#include <mutex>

struct Node
{
  int value;
  Node* next;
  std::mutex* node_mutex;
};

class FineGrainedQueue
{
  Node* head;
  std::mutex* queue_mutex;
    
public:
    FineGrainedQueue()
        : head( new Node )
    {
    }
    
    ~FineGrainedQueue()
    {
    }
    void insertIntoMiddle( int value, int pos)
    {
       
        // получаем указатель на первый элемент списка и блокируем его мьютекс
        Node* current = nullptr;
        Node* prev = nullptr;
        {
            std::unique_lock<std::mutex> lock( *queue_mutex );
          
            current = head;
            current->node_mutex->lock();
        }
        
        // итерируемся по списку до указанной точки вставки или конца списка
        for ( int counter = 0; current->next && counter != pos; ++ counter)
        {
            Node* old_prev = prev;
            prev = current;
            current = current->next;
            current->node_mutex->lock();
            
            if ( old_prev )
                old_prev->node_mutex->unlock();
        }
                
        // выполняем вставку
        if ( current->next )
            current->next->node_mutex->lock();
        
        Node* node = new Node();
        node->value = value;
        node->next = current->next;
        current->next = node;
        
        if ( node->next )
            node->next->node_mutex->unlock();
        
        current->node_mutex->unlock();
        
        if ( prev )
            prev->node_mutex->unlock();
    }
};

 int main()
{
     return 0;
}
