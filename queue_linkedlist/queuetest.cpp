#include <iostream>
#include "queue.h"

using namespace std;


int main()
{
    Queue newQueue = *(new Queue(100));

    for(int i = 0; i != 50; ++i){
        newQueue.enqueue(i);
    }

    newQueue.clear();

    for(int i = 50; i != 100; ++i){
        newQueue.enqueue(i);
    }

    cout << newQueue.front() << " " << newQueue.rear() << endl;

    for(int i = 0; !newQueue.isEmpty(); ++i){
        cout << newQueue.dequeue() << endl;
    }

    return 0;
}
