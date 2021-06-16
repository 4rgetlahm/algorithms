#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>

using namespace std;

class City;
class Road;

class Road{
private:
    string owner;
    int price;
    City *destination1, *destination2;
    bool visited = false;
public:
    Road(string owner, int price, City* destination1, City* destination2){
        this->owner = owner;
        this->price = price;
        this->destination1 = destination1;
        this->destination2 = destination2;
    }

    string getOwner(){
        return this->owner;
    }

    int getPrice(){
        return this->price;
    }

    City* getDestination1(){
        return this->destination1;
    }

    City* getDestination2(){
        return this->destination2;
    }

    bool isVisited(){
        return this->visited;
    }

    void setVisited(bool visited){
        this->visited = visited;
    }
};

class City{
private:
    string name;
    vector<Road*> roads;
public:
    City(string name){
        this->name = name;
    }

    void addRoad(Road* road){
        this->roads.push_back(road);
    }

    string getName(){
        return this->name;
    }

    vector<Road*> getRoads(){
        return this->roads;
    }

};

vector<City*> readData(){
    vector<City*> cityList;
    ifstream fs("data.txt");
    while(!fs.eof()){
        string cityname1, cityname2, owner;
        int price;
        fs >> cityname1 >> cityname2 >> owner >> price;
        City* city1 = NULL;
        City* city2 = NULL;
        for(City* city : cityList){
            if(city->getName() == cityname1){
                city1 = city;
            } else if(city->getName() == cityname2){
                city2 = city;
            }
        }
        if(city1 == NULL){
            city1 = new City(cityname1);
            cityList.push_back(city1);
        }

        if(city2 == NULL){
            city2 = new City(cityname2);
            cityList.push_back(city2);
        }

        Road* road = new Road(owner, price, city1, city2);
        city1->addRoad(road);
        city2->addRoad(road);
    }

    return cityList;
}


bool isRoadVisited(Road* road, vector<Road*> visitedRoads){
    for(Road* visitedRoad : visitedRoads){
        if(visitedRoad == road){
            return true;
        }
    }
    return false;
}

void travelFrom(City* currentCity, int currentPrice, string currentPath, City* startCity, City* destinationCity, string owner, vector<int> &priceList, vector<string> &pathList){
    for(Road* road : currentCity->getRoads()){
        //cout << currentCity->getName() << endl;
        if(!road->isVisited()){
            //cout << currentPath << "   " << road->getDestination1()->getName() << " " << currentPrice << " " << road->getPrice() << endl;
            City* currentDestination = NULL;
            if(road->getDestination1() == currentCity){
                currentDestination = road->getDestination2();
            } else {
                currentDestination = road->getDestination1();
            }

            if(currentDestination == currentCity){
                continue;
            }

            int addPrice = 0;
            if(road->getOwner() == owner){
                addPrice = road->getPrice() / 2;
            } else {
                addPrice = road->getPrice();
            }
            currentPrice += addPrice;

            road->setVisited(true);

            string addPath = "->" + currentDestination->getName();
            currentPath = currentPath + addPath;

            if(currentDestination == destinationCity){
                priceList.push_back(currentPrice);
                pathList.push_back(currentPath);
            } else {
                travelFrom(currentDestination, currentPrice, currentPath, startCity, destinationCity, owner, priceList, pathList);
            }
            currentPath = currentPath.substr(0, currentPath.size() - addPath.size());
            road->setVisited(false);
            currentPrice -= addPrice;
        }
    }
}

int main()
{
    vector<City*> cityList = readData();

    string start, destination, owner;
    City *startCity, *destinationCity;
    cin >> start >> destination >> owner;

    for(City* city : cityList){
        if(city->getName() == start){
            startCity = city;
        }
        if(city->getName() == destination){
            destinationCity = city;
        }
    }

    if(startCity == NULL || destinationCity == NULL || startCity == destinationCity){
        return 0;
    }

    vector<string> pathList;
    vector<int> priceList;
    string startPath = startCity->getName();
    travelFrom(startCity, 0, startPath, startCity, destinationCity, owner, priceList, pathList);


    if(pathList.size() == 0){
        cout << "No travel path to the city!" << endl;
        return 0;
    }

    int minPrice = INT_MAX;
    string minPath;

    for(int i = 0; i != priceList.size(); i++){
        if(priceList[i] < minPrice){
            minPrice = priceList[i];
            minPath = pathList[i];
        }
    }

    cout << "Lowest price: " << minPrice << endl;
    cout << "Path: " << minPath << endl;

    cout << endl << endl << "Other possible paths and their prices:" << endl;
    for(int i = 0; i != pathList.size(); i++){
        cout << pathList[i] << " " << priceList[i] << endl;
    }


    return 0;
}
