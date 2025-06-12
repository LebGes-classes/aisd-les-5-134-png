#include <vector>
#include <iostream>

class Node{//сразу дерево и вершина которая хранит 
//значение, ранг, указатель на самого левого сына
//указатели на левого и правого братьев
//ну и указатель на родителя

//в отличие от обычной кучи требование кучи будет другое: значения вершины будет меньше или равно значению значения вершины предка

public:
    int rank;

    int value;
    
    Node* rightSibling = nullptr;
    Node* leftSibling = nullptr;

    Node* father = nullptr;

    Node* leftSon = nullptr;//подобие связного списка
    //поддеревья будут идти в порядке увеличения их ранга

    Node(Node* t1, Node* t2){//конструктор будет соответствовать 
    //определению биномиального дерева
    //то есть мы будем строить дерево ранга n из двух деревьев ранга (n-1)
        Node* upperNode = t1;
        Node* lowerNode = t2;

        if(t2->value >= t1->value){//если значение в корне дерева t1 >= значения корня дерева t2
            //t1->value - корень
            upperNode = t2;
            lowerNode = t1;
        } 
        
        rank = upperNode->rank + 1;
        value = upperNode->value;

        leftSon = upperNode->leftSon;

        lowerNode->father = this;
        joinNewSubtree(lowerNode);

    }

    Node(int value){//создаем дерево 0-го ранга
        rank = 0;
        this->value = value;
    }
private:
    void joinNewSubtree(Node* node){//используем в конструкторе
    //присоединяем к потомкам
    //а также меняем ссылку на отца
        Node* currNode = leftSon;
        if(currNode == nullptr){
            leftSon = node;
            return;
        }
        
        bool flag = true;
        
        while(flag){

            currNode->father = this;

            if(currNode->rightSibling == nullptr){
                currNode->rightSibling = node;
                flag = false;
            }
            else
                currNode = currNode->rightSibling;
            
        }

        node->leftSibling = currNode;
    }

};

std::vector<Node*>* simpleSort(std::vector<Node*>* vector){//сортировка по рангу деревьев
    std::vector<Node*>* newVector = new std::vector<Node*>();
    Node* currNode = nullptr;
    int deletingIndex;

    while(vector->size() != 0){
        
        for(unsigned i = 0; i < vector->size(); i++){

            if(currNode == nullptr || vector->at(i)->rank < currNode->rank){
                currNode = vector->at(i);
                deletingIndex = i;
            }
        }

        if(currNode != nullptr){

            newVector->push_back(vector->at(deletingIndex));
            vector->erase(vector->cbegin() + deletingIndex);
            
        }

        currNode = nullptr;

    }

    return newVector;

}

class BinomialHeap{
public:    
    std::vector<Node*>* trees;
    bool changed = false;
    Node* max = nullptr;


    BinomialHeap(std::vector<Node*>* trees){
        this->trees = trees;
        findMax();
    }
    BinomialHeap(){
        trees = new std::vector<Node*>();
    }

    void findMax();
    Node* getMax();
    Node* extractMax();

    void insert(int x);
    void increaseKey(Node* node, int delta);
    void siftUp(Node* node);

    void mergeTreeWithHeap(Node* tree);
    void mergeHeaps(BinomialHeap* h);
    

};

void BinomialHeap::findMax(){
    max = nullptr;
    changed = false;
    for(unsigned i = 0; i < trees->size(); i++){
        
        if(max == nullptr || trees->at(i)->value > max->value){
            max = trees->at(i);
        }   
    }
}

Node* BinomialHeap::getMax(){
    if(changed)
        findMax();
    
    if(max == nullptr)
        return nullptr;
    return max;
}

void BinomialHeap::increaseKey(Node* node, int delta){
    changed = true;
    node->value += delta;
    siftUp(node);

}

void BinomialHeap::siftUp(Node* node){
    if(node->father == nullptr)
        return;

    Node* fatherNode = node->father;
    int temp;
    if(fatherNode->value < node->value){

        temp = fatherNode->value;
        fatherNode->value = node->value;
        node->value = temp;

    }

    siftUp(fatherNode);
}

void BinomialHeap::insert(int x){
    changed = true;
    Node* newTree = new Node(x);

    mergeTreeWithHeap(newTree);
}

Node* BinomialHeap::extractMax(){
    if(changed)
        findMax();

    changed = true;

    Node* node = max;
    if(max == nullptr)
        return nullptr;
    
    for(unsigned i = 0; i < trees->size(); i++){
        if(trees->at(i) == max){
            trees->erase(trees->cbegin() + i);
        }
    }
    
    std::vector<Node*>* newHeap = new std::vector<Node*>();

    Node* start = max->leftSon;

    Node* rightSibl;

    while(start != nullptr){

        newHeap->push_back(start);

        rightSibl = start->rightSibling;
        start->father = nullptr;
        start->leftSibling = nullptr;
        start->rightSibling = nullptr;

        start = rightSibl;

    }

    newHeap = simpleSort(newHeap);
    
    BinomialHeap* h = new BinomialHeap(newHeap);

    mergeHeaps(h);

    node->leftSon = nullptr;
    
    return node;
}

void BinomialHeap::mergeTreeWithHeap(Node* tree){

    for(unsigned i = 0; i < trees->size(); i++){

        Node* currTree = trees->at(i);

        if(currTree->rank > tree->rank){
            trees->insert(trees->cbegin() + i, tree);
            return;
        }
        else if(currTree->rank == tree->rank){
            trees->erase(trees->cbegin() + i);
            Node* newSubtree = new Node(tree, currTree);
            
            mergeTreeWithHeap(newSubtree);
            return;
        }
        
    }

    trees->push_back(tree);
}

void BinomialHeap::mergeHeaps(BinomialHeap* h){
    
    std::vector<Node*>* otherTrees = h->trees;

    for(unsigned i = 0; i < otherTrees->size(); i++){
        mergeTreeWithHeap(otherTrees->at(i));
    }
}
