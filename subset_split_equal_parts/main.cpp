#include <iostream>
#include <vector>

using namespace std;

void reverseBubbleSort(vector<int> &arr)
{
    for (int i = 0; i < arr.size()-1; ++i){
        for (int j = 0; j < arr.size()-i-1; ++j){
            if (arr[j] < arr[j+1]){
                int _temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = _temp;
            }
        }
    }
}

pair<vector<int>, vector<int>> getApproximateSubsets(vector<int> &mainSet){
    reverseBubbleSort(mainSet);
    vector<int> firstSubset;
    vector<int> secondSubset;
    int difference = 0;
    for(int number : mainSet){
        if(number > 0){
            if(difference < 0){
                firstSubset.push_back(number);
                difference += number;
            } else{
                secondSubset.push_back(number);
                difference -= number;
            }
        } else {
            if(difference > 0){
                firstSubset.push_back(number);
                difference += number;
            } else{
                secondSubset.push_back(number);
                difference -= number;
            }
        }
    }
    return make_pair(firstSubset, secondSubset);
}

int main()
{
    freopen("numbers.txt", "r", stdin);

    vector<int> testSet;

    while(!cin.eof()){
        int read;
        cin >> read;
        testSet.push_back(read);
    }

    pair<vector<int>, vector<int>> result = getApproximateSubsets(testSet);
    long sum = 0;
    for(int num : result.first){
        cout << num << " ";
        sum += num;
    }
    cout << endl << endl;
    cout << sum;
    cout << endl << endl << endl;
    sum = 0;
    for(int num : result.second){
        cout << num << " ";
        sum += num;
    }
    cout << endl << endl;
    cout << sum;

    return 0;
}


