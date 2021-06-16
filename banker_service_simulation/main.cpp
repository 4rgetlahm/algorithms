#include <iostream>
#include <random>
#include <iomanip>
#include <time.h>
#include <chrono>
#include <thread>
#include "queue.h"

using namespace std;

const int TICK_TIME = 1;
const int QUEUE_SIZE = 1000000;
static std::default_random_engine generator(time(0));

class Worker{
private:
    bool goodWorker = false;
    unsigned long customersServed = 0;
    unsigned long workerServiceTimeMin, workerServiceTimeMax;
    unsigned int totalServicingTime = 0;
    unsigned long clientServicingTimeLeft;
    bool servicing = false;
public:
    Worker(int workerServiceTimeMin, int workerServiceTimeMax, bool goodWorker){
        this->workerServiceTimeMin = workerServiceTimeMin;
        this->workerServiceTimeMax = workerServiceTimeMax;
        this->goodWorker = goodWorker;
    }

    bool isAvailable(){
        return !servicing;
    }

    void tick(){
        if(servicing){
            totalServicingTime += TICK_TIME;
            clientServicingTimeLeft -= TICK_TIME;
            if(clientServicingTimeLeft <= 0){
                customersServed++;
                servicing = false;
            }
        }
    }

    void assignClient(){
        servicing = true;
        std::uniform_int_distribution<int> distribution(workerServiceTimeMin, workerServiceTimeMax);
        clientServicingTimeLeft = distribution(generator);
    }

    bool isGoodWorker(){
        return goodWorker;
    }

    int getServicedCount(){
        return customersServed;
    }

    int getWorkSpeed(){
        return totalServicingTime/customersServed;
    }
};

vector<Worker*> fillWorkers(double workerCount, double goodToBadRatio, int goodTimeMin, int goodTimeMax, int badTimeMin, int badTimeMax){
    vector<Worker*> workerList;
    for(double i = 0; i != workerCount; ++i){
        if(i / workerCount < goodToBadRatio){
            workerList.push_back(new Worker(goodTimeMin, goodTimeMax, true));
        } else {
            workerList.push_back(new Worker(badTimeMin, badTimeMax, false));
        }
    }
    return workerList;
}

bool isClientArrival(double chance){
    std::uniform_real_distribution<double> chanceDistribution(0.0, 1.0);
    if(chanceDistribution(generator) <= chance){
        return true;
    }
    return false;
}

void lineTick(Queue* queue){
    if(queue->isEmpty()){
        return;
    }

    Node* node = queue->getFirstNode();
    while(node->nextNode != NULL){
        node->value += TICK_TIME;
        node = node->nextNode;
    }
}

unsigned long lineWaitTime(Queue* line){
    if(line->isEmpty()){
        return 0;
    }
    unsigned long sum = 0;
    Node* node = line->getFirstNode();
    while(node->nextNode != NULL){
        sum += node->value;
        node = node->nextNode;
    }
    return sum;
}

int main()
{
    unsigned long passedTime = 0;
    double workerCount;
    double ratio, chance;
    unsigned long goodMin, goodMax;
    unsigned long badMin, badMax;
    long runtime;
    unsigned long waitTimeTotal = 0, waitedClients = 0;

    cout << "Enter worker count: ";
    cin >> workerCount;
    cout << "Enter Good worker's minimum client service time: " << endl;
    cin >> goodMin;
    cout << "Enter Good worker's maximum client service time: " << endl;
    cin >> goodMax;
    cout << "Enter Bad worker's minimum client service time: " << endl;
    cin >> badMin;
    cout << "Enter Bad worker's maximum client service time: " << endl;
    cin >> badMax;
    cout << "Enter Good to Bad worker ratio (0.0 - 1.0)" << endl;
    cin >> ratio;
    cout << "Enter client arrival chance (0.0 - 1.0)" << endl;
    cin >> chance;
    cout << "Enter test run time: " << endl;
    cin >> runtime;

    Queue line = *(new Queue(QUEUE_SIZE));

    vector<Worker*> workerList = fillWorkers(workerCount, ratio, goodMin, goodMax, badMin, badMax);


    while(passedTime <= runtime){
        if(isClientArrival(chance)){
            if(line.isEmpty()){
                bool foundWorker = false;
                for(Worker* worker : workerList){
                    if(worker->isAvailable()){
                        foundWorker = true;
                        worker->assignClient();
                        break;
                    }
                }
                if(!foundWorker){
                    line.enqueue(0.0);
                }
            } else{
                line.enqueue(0.0);
            }
        }

        bool queueEmpty = line.isEmpty();
        for(Worker* worker : workerList){
            worker->tick();
            if(worker->isAvailable() && !queueEmpty){
                worker->assignClient();
                waitTimeTotal += line.dequeue();
                ++waitedClients;
                queueEmpty = line.isEmpty();
            }
        }
        lineTick(&line);
        passedTime += TICK_TIME;
    }

    freopen("output.txt", "w", stdout);

    int total = 0, goodServiced = 0, badServiced = 0;

    cout << "------------------------------------------------" << endl;
    for(Worker* worker : workerList){
        if(worker->isGoodWorker()){
            cout << "GOODWORKER SERVICED: " << worker->getServicedCount() << "   AVERAGE WORK SPEED: " << worker->getWorkSpeed() << endl;
            goodServiced += worker->getServicedCount();
        } else {
            cout << "BADWORKER SERVICED: " << worker->getServicedCount() << "   AVERAGE WORK SPEED: " << worker->getWorkSpeed() << endl;
            badServiced += worker->getServicedCount();
        }
        total += worker->getServicedCount();
    }

    cout << "------------------------------------------------" << endl;
    cout << "TOTAL SERVICED: " << total << " CLIENTS" << endl;
    cout << "GOOD WORKERS SERVICED: " << goodServiced << " CLIENTS" << endl;
    cout << "BAD WORKERS SERVICED: " << badServiced << " CLIENTS" << endl;
    cout << "------------------------------------------------" << endl << endl;

    if(waitedClients > 0){
        cout << "------------------------------------------------" << endl;
        cout << "CLIENTS THAT WAITED IN QUEUE AND WERE SERVED: " << waitedClients << endl;
        cout << "AVERAGE WAIT TIME: " << waitTimeTotal/waitedClients << endl;
        cout << "------------------------------------------------" << endl << endl;
    }

    if(!line.isEmpty()){
        cout << "------------------------------------------------" << endl;
        cout << "CLIENTS LEFT IN QUEUE THAT WERE NOT SERVED: " << line.getSize() << endl;
        cout << "CLIENT LEFT IN QUEUE AVERAGE WAIT TIME: " << lineWaitTime(&line)/line.getSize() << endl;
        cout << "------------------------------------------------" << endl << endl;
    }

    if(waitedClients > 0 && !line.isEmpty()){
        cout << "------------------------------------------------" << endl;
        cout << "TOTAL CLIENTS WAITED IN QUEUE: " << waitedClients + line.getSize() << endl;
        cout << "TOTAL CLIENT AVERAGE WAIT TIME: " << (lineWaitTime(&line)+waitTimeTotal)/(line.getSize()+waitedClients) << endl;
        cout << "------------------------------------------------" << endl << endl;
    }

    return 0;
}
