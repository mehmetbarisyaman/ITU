//
//  main.cpp
//  Moby Dick
//
//  Created by Baris on 10/30/16.
//  Copyright © 2016 Mehmet Barış Yaman. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <string>
#include <cstdlib>

using namespace std;

/* Normally we make the variables are private in projects
   But in this assignment making the members public prevents 
   the code to be more ambiguous.
 */
class Word{
public:
    string name;
    int order;
};


class Sentence{
public:
    vector<Word>words; // Since every line has different words
    long long ID;
};

// Since the size of the array for quicksort is not known
vector<Sentence>sentences;
int quickSortCounter;

// The inputs given before running of the program
long long wantedLoopCount;
long long wantedIndex;

// For spreading the data into classes
void convertToClasses(long long takenID, string words, string wordOrders){
    Sentence newSentence;
    newSentence.ID = takenID;
    size_t found; // Used for finding '_' character
    
    // Deleting '{' and '}' characters
    words.erase(words.begin());
    words.erase(words.end()-1);
    wordOrders.erase(wordOrders.begin());
    wordOrders.erase(wordOrders.end()-1);
    
    do{
        // For name of the words
        found = words.find_first_of('_');
        Word newWord;
        newWord.name.append(words, 0, found);
        words.erase(0, found+1); // Deleting '_' character
        
        // For order of the words
        found = wordOrders.find_first_of('_');
        string tempString;
        tempString.append(wordOrders, 0, found);
        newWord.order = stoi(tempString); // C++11 function
        newSentence.words.push_back(newWord);
        wordOrders.erase(0, found+1);
    }while(found!=string::npos);
    sentences.push_back(newSentence);
}

// Since words, sentences and number are changed, templated functions are needed
template<typename T>
void exchange(T *array, long long arg1, long long arg2){
    T temp = array[arg1];
    array[arg1] = array[arg2];
    array[arg2] = temp;
}

// Since the classes have different members there is no template function for partition

long long partitionForSentence(Sentence *array, long long beginIndex, long long endIndex){
    Sentence x = array[endIndex];
    long long i = beginIndex-1;
    for(long long j = beginIndex; j<endIndex; j++){
        if(array[j].ID <= x.ID){
            i++;
            exchange(array, i, j);
        }
    }
    exchange(array, i+1, endIndex);
    return i+1;
}

long long partitionForWord(Word *array, long long beginIndex, long long endIndex){
    Word x = array[endIndex];
    long long i = beginIndex-1;
    for(long long j = beginIndex; j<endIndex; j++){
        if(array[j].order <= x.order){
            i++;
            exchange(array, i, j);
        }
    }
    exchange(array, i+1, endIndex);
    return i+1;
}

long long partition(long long *array, long long beginIndex, long long endIndex){
    long long x = array[endIndex];
    long long i = beginIndex-1;
    for(long long j = beginIndex; j<endIndex; j++){
            if(array[j] <= x){
                i++;
                exchange(array, i, j);
            }
    }
    exchange(array, i+1, endIndex);
    return i+1;
}

// For sorting long long types, used for question 2 of Part B

void quickSort(long long *array, long long beginIndex, long long endIndex){
    long long middleIndex;
    if(beginIndex<endIndex){
        middleIndex = partition(array, beginIndex, endIndex);
        quickSort(array, beginIndex, middleIndex-1);
        quickSort(array, middleIndex, endIndex);
    }
}

// No Quicksort for Word class objects is needed

void quickSortForSentence(Sentence *array, long long beginIndex, long long endIndex){
    long long middleIndex;
    quickSortCounter++;
    if(quickSortCounter==wantedLoopCount){
        cout << "ID of the wanted index is: " << array[wantedIndex].ID << endl;
    }
    if(beginIndex<endIndex){
        middleIndex = partitionForSentence(array, beginIndex, endIndex);
        quickSortForSentence(array, beginIndex, middleIndex-1);
        quickSortForSentence(array, middleIndex, endIndex);
    }
}

long long randomizedPartition(Word *array, long long beginIndex, long long endIndex){
    srand(255); // Making pivot elements chosed to be different from time to time
    long long i = (rand()%(endIndex-beginIndex))+beginIndex;
    exchange(array, endIndex, i);
    return partitionForWord(array, beginIndex, endIndex);
}

// For Sorting Word Class Objects

void randomizedQuickSort(Word *array, long long beginIndex, long long endIndex){
    if(beginIndex < endIndex){
        long long q = randomizedPartition(array, beginIndex, endIndex);
        randomizedQuickSort(array, beginIndex, q-1);
        randomizedQuickSort(array, q+1, endIndex);
    }
}


int main(int argc, const char * argv[]) {
    
    int numberOfParameters = argc;
    int workType = atoi(argv[1]);
    
    // For program 1
    if(numberOfParameters == 4 && workType==1){
        ifstream inputFile("mobydick.txt");
        ofstream outputFile("novel.txt");
        
        quickSortCounter = 0;
        wantedLoopCount = atol(argv[2]);
        wantedIndex= atol(argv[3]);
        
        
        
        if(!inputFile.is_open()){
            cout << "ERROR! Input file could not be opened." << endl;
            cout << "Program is terminated. " << endl;
            return -1;
        }
        Sentence *quickSortArray;
        long long arrayIndex = 0;
        
        // Variables used for reading the input file
        long long takenID;
        string takenWords;
        string takenWordOrders;
        while(!inputFile.eof()){
            inputFile >> takenID >> takenWords >> takenWordOrders;
            convertToClasses(takenID, takenWords, takenWordOrders);
        }
        
        // For copying the data into array before QuickSort
        quickSortArray = new Sentence[sentences.size()];
        while(sentences.size()>0){
            
            // Size is decremented in each loop
            quickSortArray[sentences.size()-1] = sentences.back();
            sentences.pop_back();
            arrayIndex++;   // For the array size
        }
        
        
        quickSortForSentence(quickSortArray, 0, arrayIndex-1);
        Word wordArray[200];    // For the randomized Quick Sort
        
        for(long long index=0; index <arrayIndex; index++){
            size_t length = quickSortArray[index].words.size();
            int counter =0;
            while(quickSortArray[index].words.size()>0){
                // Data is copied from last element to the first element
                wordArray[length-1] = quickSortArray[index].words.back();
                quickSortArray[index].words.pop_back();
                length--;
                counter++;  // For array size
            }
            randomizedQuickSort(wordArray, 0, counter-1);
            for(int k=0; k<counter; k++){
                outputFile << wordArray[k].name << " ";
            }
        }
        delete [] quickSortArray;
    }
    // For program 2
    else if(numberOfParameters==3 && workType==2){
        
        ifstream inputFile("numbers_to_sort.txt");
        ofstream outputFile("numbers.txt");
        
        long long sizeOfArray = atoi(argv[2]);
        long long *array = new long long[sizeOfArray];
        
        clock_t runTime;
        
        if(!inputFile.is_open()){
            cout << "ERROR! Input file could not be opened." << endl;
            cout << "Program is terminated. " << endl;
            return -1;
        }
        
        for(long long k=0; k<sizeOfArray ; k++){
            inputFile >> array[k];
        }
        
        runTime = clock();
        
        // long long data will be sorted
        quickSort(array, 0, sizeOfArray-1);
        runTime = clock() - runTime;
        cout << "Run time of Quick Sort Algorithm is: ";
        cout << runTime << " click(" << (float)runTime / CLOCKS_PER_SEC << " seconds)" << endl;
        
        for(long long k=0; k<sizeOfArray ; k++){
            outputFile << array[k] << " ";
        }
        
        delete [] array;
        
    }
    else{
        cout << "ERROR! Wrong number of parameters." << endl;
        cout << "Program is terminated. " << endl;
        return -1;
    }
    
    return 0;
}
