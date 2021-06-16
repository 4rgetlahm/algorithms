#ifndef QUEUE_H
#define QUEUE_H

class Node{
public:
    Node(int value){
        this->value = value;
    }

    int value;
    Node* nextNode = NULL;
};

class Queue{
private:
    int capacity, size;
    Node* firstNode;

public:
    Queue(int capacity){
        this->capacity = capacity;
        this->size = 0;
        this->firstNode = NULL;
    }

    //frees up all nodes
    void clear(){
        while(this->size != 0){
            this->dequeue();
        }
    }

    //frees up all the space including itself
    void remove(){
        this->clear();
        delete this;
    }

    //returns true if queue is full, otherwise will return false
    bool isFull(){
        if(this->size == this->capacity){
            return true;
        }
        return false;
    }

    //returns true if queue is empty, otherwise will return false
    bool isEmpty(){
        if(this->size == 0 || this->firstNode == NULL){
            return true;
        }
        return false;
    }

    //adds element to last place of the line
    void enqueue(int value){
        if(this->isFull()){
            throw 0;
        }

        if(this->firstNode == NULL){
            this->firstNode = new Node(value);
            ++(this->size);
            return;
        }

        Node* addNode = this->firstNode;
        while(addNode->nextNode != NULL){
            addNode = addNode->nextNode;
        }

        addNode->nextNode = new Node(value);
        ++(this->size);
    }

    //removes first element in line
    int dequeue(){
        if(isEmpty()){
            throw 1;
        }

        int returnValue = this->firstNode->value;
        Node* nodeToRemove = this->firstNode;
        this->firstNode = this->firstNode->nextNode;

        delete nodeToRemove;

        --(this->size);
        return returnValue;
    }

    //returns first value in queue
    int front(){
        if(isEmpty()){
            throw 1;
        }
        return this->firstNode->value;
    }

    //returns last value in queue
    int rear(){
        if(isEmpty()){
            throw 1;
        }
        Node* lastNode = this->firstNode;
        while(lastNode->nextNode != NULL){
            lastNode = lastNode->nextNode;
        }
        return lastNode->value;
    }
};

#endif
