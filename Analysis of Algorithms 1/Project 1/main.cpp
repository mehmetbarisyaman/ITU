/*
 Student Name: Mehmet Barış Yaman 
 Student Number: 150130136
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <ctime>
#include <math.h>

using namespace std;


class warehouse{
public:
    long long id;
    long long xCoordinate;
    long long yCoordinate;
    long long eucledianDistance;    // for comparing the warehouses
};


// The function which is about finding the furthest warehouse in a warehouse array

long long findFurthestWarehouseIndexOnTheArray(warehouse *chosenArray, long long chosenArraySize){
    long long furthestWarehouseIndex=0;
    warehouse chosenOne =chosenArray[0];        //initialized at first
    for(long long i = 1; i<chosenArraySize; i++){
        if(chosenArray[i].eucledianDistance > chosenOne.eucledianDistance){
            chosenOne = chosenArray[i];
            furthestWarehouseIndex = i;
        }
    }
    return furthestWarehouseIndex;
}


void linearSearch(warehouse *inputArray, long long inputSize, long long outputSize){
    long long inputFileIndex=outputSize;
    long long furthestWarehouseIndex;
    warehouse *tempArray = new warehouse[outputSize];     // Temporary array with size K is created
    for(long long k=0; k<outputSize; k++)
        tempArray[k] = inputArray[k];                     // Initializing the temporary array
    do {
        furthestWarehouseIndex = findFurthestWarehouseIndexOnTheArray(tempArray, outputSize);
        // Change the furthest warehouse if there is a warehouse nearer
        if(tempArray[furthestWarehouseIndex].eucledianDistance > inputArray[inputFileIndex].eucledianDistance)
            tempArray[furthestWarehouseIndex] = inputArray[inputFileIndex];
        inputFileIndex++;
    } while (inputFileIndex<inputSize);         // Terminate the process if the warehouse array index is at the end
    //Getting the data back from the temporary array
    for(long long k=0; k<outputSize; k++)
        inputArray[k] = tempArray[k];
    delete [] tempArray;
}



void merge(warehouse *dataArray, long long startingNumber, long long middleNumber, long long endingNumber){
    long long countOfFirstArray = (middleNumber - startingNumber) + 1;
    long long countOfSecondArray = endingNumber - middleNumber;
    warehouse *leftObjects;
    warehouse *rightObjects;
    
    // left and right arrays must be dynamically created
    leftObjects = new warehouse[countOfFirstArray +1];
    rightObjects = new warehouse[countOfSecondArray + 1];
    long long i, j, k;
    
    for(i=0; i<countOfFirstArray; i++){
        leftObjects[i] = dataArray[startingNumber+i];
    }
    
    // there is no difference between the int max or long max for the last member of left and right array objects
    leftObjects[countOfFirstArray].eucledianDistance = numeric_limits<int>::max();

    for(j=0; j<countOfSecondArray; j++){
        rightObjects[j] = dataArray[middleNumber + j + 1];
    }
    
    rightObjects[countOfSecondArray].eucledianDistance = numeric_limits<int>::max();
    
    i=0;
    j=0;
    
    // Euledian Distance of the warehouses should be compared
    for(k = startingNumber; k<=endingNumber; k++){
        if(leftObjects[i].eucledianDistance <= rightObjects[j].eucledianDistance){
            dataArray[k] = leftObjects[i];
            i++;
        }
        else{
            dataArray[k] = rightObjects[j];
            j++;
        }
    }
    delete []leftObjects;
    delete []rightObjects;
}


// Same as the pseudocode in lecture slides
void mergeSort(warehouse *dataArray, long long startingNumber, long long endingNumber){
    long long middleNumber;
    if(startingNumber < endingNumber){
        middleNumber = (startingNumber + endingNumber)/2;
        mergeSort(dataArray, startingNumber, middleNumber);
        mergeSort(dataArray, middleNumber+1, endingNumber);
        merge(dataArray, startingNumber, middleNumber, endingNumber);
    }
}



// Same as the pseudocode in lecture slides
void insertionSort(warehouse *dataArray, long long numberOfElements){
    warehouse key;
    long long index;
    for(long long counter = 1; counter <numberOfElements; counter++){
        key = dataArray[counter];
        index = counter -1;
        // Eucledian Distance of the warehouses must be compared
        while(index >=0 && dataArray[index].eucledianDistance>key.eucledianDistance){
            dataArray[index+1] = dataArray[index];
            index = index -1;
        }
        dataArray[index +1] = key;
    }
}



int main(int argc, const char * argv[]) {
    
    ifstream inputFile("warehouselocations.txt");
    ofstream outputFile("output.txt");
    
    clock_t algorithmTime;
    long long numberOfLocations;        // N
    long long numberOfNeededClosestLocations;   // K
    string algorithmType;
    long long xAxis;
    long long yAxis;
    warehouse *warehouses;
    int numberOfParameters;
    
    
    // Typing the strings into long long data types
    numberOfParameters = argc;
    numberOfLocations = atoll(argv[1]);
    numberOfNeededClosestLocations = atoll(argv[2]);
    algorithmType = argv[3];
    xAxis = atoll(argv[4]);
    yAxis = atoll(argv[5]);
    

    
    if(numberOfParameters != 6){
        cout << "ERROR! Not enough input parameters. " << endl;
        cout << "Program is terminated. " << endl;
        return -1;
    }
    
    
    if(algorithmType!="LS" && algorithmType!= "MS" && algorithmType != "IS"){
        cout << "ERROR! Please select an appropriate algorithm type. " << endl;
        cout << "Program is terminated. " << endl;
        return -1;
    }
    
    
    if(!inputFile.is_open()){
        cout << "ERROR! Input file could not be opened." << endl;
        cout << "Program is terminated. " << endl;
        return -1;
    }
    
    
    warehouses = new warehouse[numberOfLocations];  // dynamically created warehouse array is needed

    
    for(long long i=0; i<numberOfLocations; i++){
        inputFile >> warehouses[i].id >> warehouses[i].xCoordinate >> warehouses[i].yCoordinate;
        // Calculating the eucledian distance of the warehouse array members
        warehouses[i].eucledianDistance = sqrt(powl(xAxis-warehouses[i].xCoordinate, 2) + powl(yAxis-warehouses[i].yCoordinate, 2));
    }
    
    
    // Algorithm runs and the time calculations
    if(algorithmType == "IS"){
        algorithmTime = clock();
        insertionSort(warehouses, numberOfLocations);
        algorithmTime = clock() - algorithmTime;
        cout << "N: " << numberOfLocations << "  K: " << numberOfNeededClosestLocations << endl;
        cout << "Run time of Insertion Sort Algorithm is: ";
        cout << algorithmTime << " click(" << (float)algorithmTime / CLOCKS_PER_SEC << " seconds)" << endl;

    }
    
    else if(algorithmType=="LS"){
        algorithmTime = clock();
        linearSearch(warehouses, numberOfLocations, numberOfNeededClosestLocations);
        algorithmTime = clock() - algorithmTime;
        cout << "N: " << numberOfLocations << "  K: " << numberOfNeededClosestLocations << endl;
        cout << "Run time of Linear Search Algorithm is: ";
        cout << algorithmTime << " click(" << (float)algorithmTime / CLOCKS_PER_SEC << " seconds)" << endl;

    }
    
    else if(algorithmType == "MS"){
        algorithmTime = clock();
        mergeSort(warehouses, 0, numberOfLocations-1);
        algorithmTime = clock() - algorithmTime;
        cout << "N: " << numberOfLocations << "  K: " << numberOfNeededClosestLocations << endl;
        cout << "Run time of Merge Sort Algorithm is: ";
        cout << algorithmTime << " click(" << (float)algorithmTime / CLOCKS_PER_SEC << " seconds)" << endl;
    }
    
    
    // Writing the data into the output file
    for(long long i=0; i<numberOfNeededClosestLocations; i++){
        outputFile << warehouses[i].id << " " <<  warehouses[i].xCoordinate << " " << warehouses[i].yCoordinate << endl;
    }
    
    
    
    
    
    
    inputFile.close();
    outputFile.close();
    
    delete [] warehouses;
    return 0;
}
