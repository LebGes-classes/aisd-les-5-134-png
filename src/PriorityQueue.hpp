#include "BinomialHeap.hpp"


class PriorityQueue{
public:

    BinomialHeap* heap = nullptr;

    PriorityQueue(){

        heap = new BinomialHeap();
    }

    void enqueue(int value);
    Node* dequeue();

    Node* peek();
    void increasePriority(Node* priority, int delta);

    void merge(PriorityQueue* otherQueue);
};

void PriorityQueue::enqueue(int value){

    heap->insert(value);
}

Node* PriorityQueue::dequeue(){
    return heap->extractMax();
}

Node* PriorityQueue::peek(){
    return heap->getMax();
}

void PriorityQueue::increasePriority(Node* priority, int delta){
    heap->increaseKey(priority, delta);
}

void PriorityQueue::merge(PriorityQueue* otherQueue){
    heap->mergeHeaps(otherQueue->heap);
}