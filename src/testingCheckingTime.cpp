#include "PriorityQueue.hpp"
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>


using namespace std::chrono;

void testingQueue(){


    PriorityQueue* queue =new PriorityQueue();

    queue->enqueue(2);
    queue->enqueue(5);
    queue->enqueue(90);
    queue->enqueue(83);
    queue->enqueue(10);
    queue->enqueue(60);
    queue->enqueue(32);

    Node* max = queue->dequeue();
    if(max == nullptr || max->value != 90){
        std::cout << "test 1 failed"<< std::endl;
        return;
    }
    
    if(queue->peek()->value != 83){
        std::cout << "test 2 failed" << std::endl;
        return;
    }

    for(int i = 0; i < 6; i++){
        queue->dequeue();
    }

    if(queue->dequeue() != nullptr || queue->peek() != nullptr){
        std::cout << "test 3 failed" << std::endl;
    }

    queue->enqueue(2);
    queue->enqueue(10);
    queue->enqueue(60);

    PriorityQueue* queue2 = new PriorityQueue();

    queue2->enqueue(5);
    queue2->enqueue(90);
    queue2->enqueue(83);

    queue->merge(queue2);
    max = queue->dequeue();

    if(max == nullptr || max->value != 90){
        std::cout << "test 4 failed"<< std::endl;
        return;
    }
    
    if(queue->peek()->value != 83){
        std::cout << "test 5 failed" << std::endl;
        return;
    }

    Node* someNode = queue->heap->trees->at(1)->leftSon->rightSibling->leftSon;
    queue->increasePriority(someNode, 100);

    if(queue->peek()->value != 102){
        std::cout << "test 6 failed" << std::endl;
    }

    std::cout << "all tests passed" << std::endl;
}

long avg(std::vector<long> d){
    int sum = 0;

    for(unsigned i = 0; i < d.size(); i++){
        sum+= d[i];
    }

    return sum / d.size();
}
int main(){
    testingQueue();

    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<int> dist(1, 50000);

    std::vector<long> vector1;
    std::vector<long> vector2;

    PriorityQueue* queue;
    for(int i = 0; i < 13; i++){
        queue = new PriorityQueue();
        int n = 1000 + 1300 * i;

        auto start1 = std::chrono::steady_clock::now();
        for(int j = 0; j < n;j++){
            queue->enqueue(dist(e));
        }

        auto end1 = std::chrono::steady_clock::now();
        
        vector1.push_back(duration_cast<microseconds>(end1 - start1).count());

        auto start2 = std::chrono::steady_clock::now();
        for(int j = 0; j < n;j++){
            queue->dequeue();
        }

        auto end2 = std::chrono::steady_clock::now();
        vector2.push_back(duration_cast<microseconds>(end2 - start2).count());

    }
    std::cout << "enqueue on average input " << (1000 + 1300 * 6) << " works for " << avg(vector1) << " microseconds" << std::endl;
    std::cout << "dequeue on average input " << (1000 + 1300 * 6) << " works for " << avg(vector2) << " microseconds" << std::endl;

    PriorityQueue* mergedQueue;
    vector1 = std::vector<long>();

    for(int i = 0; i < 13; i++){

        queue = new PriorityQueue();
        mergedQueue = new PriorityQueue;

        int n = 1000 + 1300 * i;

        for(int j = 0; j < n;j++){
            queue->enqueue(dist(e));
            mergedQueue->enqueue(dist(e));
        }

        auto start = std::chrono::steady_clock::now();

        queue->merge(mergedQueue);

        auto end = std::chrono::steady_clock::now();
        
        vector1.push_back(duration_cast<microseconds>(end - start).count());
    }

    std::cout << "merge operation on average input for merging queues " << (1000 + 1300 * 6) << " works for " << avg(vector1) << " microseconds" << std::endl;
    std::cout << "merge is not really heavy operation))" << std::endl;

    return 0;
}