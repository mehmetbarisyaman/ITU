/* Mehmet Barış Yaman
    150130136
*/

#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <ctime>
#include <math.h>



using namespace std;

class logs{
public:
    long long attackedPosition;
    long long gameId;
    long long attackerPosition;
    char clanOfAttacker;
};

class players{
public:
    long long ID;
    long long CP;
};


class numbers{
public:
    long long ID;
    long long number;
    long long digit;        // Used in Counting Sort
};

long long numberOfInputs;

//Parsing Numbers and selecting a digit before Counting Sort

void parseNumbers(numbers * array, int orderOfDigit){
    long long moder = pow(10, orderOfDigit);
    long long dividor = moder/10;
    for(long long i=0; i<numberOfInputs; i++)
        array[i].digit = (array[i].number % moder)/dividor;
    
}

//Used in Radix Sort

long long findMax(numbers *array){
    numbers maxNumber = array[0];
    for(long long x=0; x<numberOfInputs; x++){
        if(maxNumber.digit < array[x].digit)
            maxNumber = array[x];
    }
    return maxNumber.digit;
}

void countingSort(numbers *A, long long k){
    long long *C = new long long[k+1];
    numbers *B = new numbers[numberOfInputs+1];
    for(long long i=0; i<=k; i++)
        C[i] = 0;
    for (long long j=0; j<numberOfInputs; j++)
        C[A[j].digit]++;
    
    for (long long i=1; i<=k; i++)
        C[i] = C[i] + C[i-1];
    
    for(long long j = numberOfInputs-1; j>=0; j--){
        B[C[A[j].digit]] = A[j];
        C[A[j].digit] = C[A[j].digit] -1;
    }
    for(long long i=1; i<=numberOfInputs; i++){
        A[i-1] = B[i];
    }
    delete [] C;
    delete [] B;
}




void radixSort(numbers *array, int digits){
    for (int x=1; x<=digits; x++){
        parseNumbers(array, x);
        long long maxNumber = findMax(array);
        countingSort(array, maxNumber);
    }
    return;
}



void exchange(players *array, long long firstIndex, long long secondIndex){
    players temp = array[firstIndex];
    array[firstIndex] = array[secondIndex];
    array[secondIndex] = temp;
}


void maxHeapify(players *A, long long i){
    i = i+1;
    long long l = 2*i;
    long long r = 2*i+1;
    long long largest=i;
    if (l<= numberOfInputs && A[l-1].CP>A[i-1].CP)
        largest = l;
    else
        largest = i;
    if(r<=numberOfInputs&& A[r-1].CP>A[largest-1].CP)
        largest = r;
    if(largest != i){
        exchange(A, i-1, largest-1);
        maxHeapify(A, largest-1);
    }
}


void buildMaxHeap(players *A){
    for (long long i = numberOfInputs/2-1; i>=0; i--){
        maxHeapify(A, i);
    }
    
}

void heapSort(players *A){
    buildMaxHeap(A);
    for (long long i = numberOfInputs-1 ; i>=1; i--){
        exchange(A, i, 0);
        numberOfInputs--;       // Heap size is decremented
        maxHeapify(A, 0);
    }
}




void updateCPs(players *attackerArray, players *attackedArray, logs gameLog){
    if(gameLog.attackerPosition ==0){
        attackerArray[0].CP += attackedArray[gameLog.attackedPosition].CP/2;
        attackedArray[gameLog.attackedPosition].CP -= (int)attackedArray[gameLog.attackedPosition].CP/2;
    }
    else if(gameLog.attackerPosition<=7 && gameLog.attackerPosition>0){
        attackerArray[gameLog.attackerPosition].CP += 500;
        attackedArray[gameLog.attackedPosition].CP -= 500;
    }
    else{
        attackerArray[gameLog.attackerPosition].CP += (abs((int)log2(gameLog.attackerPosition+1) - (int)log2(gameLog.attackedPosition+1))+1) * 30;
        attackedArray[gameLog.attackedPosition].CP -=120;
    }
    if(attackedArray[gameLog.attackedPosition].CP < 0)
        attackedArray[gameLog.attackedPosition].CP = 0;
}


long long calculateTotalCPs(players *array, char whichClan){
    long long total = 0;
    for(long long index = 0; index <numberOfInputs; index++)
        total += array[index].CP;
    cout << "Total CP's of Clan " << whichClan << " is: " << total << endl;
    return total;
}

// Reversing the sorted array elements
void reverse(players *array){
    players *tempArray = new players[numberOfInputs];
    for(long long i=0 ;i<numberOfInputs;i++){
        tempArray[numberOfInputs-1-i] = array[i];
    }
    for(long long i=0; i<numberOfInputs; i++){
        array[i] = tempArray[i];
    }
    delete[] tempArray;
}

int main(int argc, const char * argv[]) {
    string algorithmType;       // First input given in command
    clock_t runTime;

    algorithmType = argv[0];
    
    
    
    if(argc==2 && algorithmType == "./150130136_AoA1_P3_1"){
        numberOfInputs = atol(argv[1]);
        
        ifstream inputFile("numbers_to_sort.txt");
        ofstream outputFile("sorted_numbers.txt");
        
        if(!inputFile.is_open()||!outputFile.is_open()){
            cout << "File Error!" << endl;
            cout << "Program is terminated." << endl;
            return -1;
        }
        
        numbers *array = new numbers[numberOfInputs];
        
        //Loading array from input file data
        for(long long i=0; i<numberOfInputs; i++)
            inputFile >> array[i].ID >> array[i].number;
        
        runTime = clock();
        radixSort(array, 10);
        runTime = clock() - runTime;
        cout << "Run time for Radix Sort with " << numberOfInputs << " inputs is " << runTime << " click ( " << ((float)runTime) / CLOCKS_PER_SEC << " seconds )" << endl;
        
        // Write sorted data into the output file
        for(long long i=0; i<numberOfInputs; i++){
            outputFile << array[i].ID << " " << array[i].number << endl ;
        }
        delete [] array;
    }
    else if(argc==3 && algorithmType == "./150130136_AoA1_P3_2" && atol(argv[1])==1){
        numberOfInputs = atol(argv[2]);
        ifstream inputFile("ClanA.txt");
        ofstream outputFile("sorted.txt");
        
        // Storing array size
        long long numberHolder = numberOfInputs;
        if(!inputFile.is_open()||!outputFile.is_open()){
            cout << "File Error!" << endl;
            cout << "Program is terminated." << endl;
            return -1;
        }
        players *array = new players[numberOfInputs];
        for(long long i=0; i<numberOfInputs; i++)
            inputFile >> array[i].ID >> array[i].CP;
        runTime = clock();
        heapSort(array);
        runTime = clock()- runTime;
        cout << "Run time for Heap Sort with " << numberOfInputs << " inputs is " << runTime << " click ( " << ((float)runTime) / CLOCKS_PER_SEC << " seconds )" << endl;
        
        /*
         Since Heap size is decremented in Heap Sort algortihm we need to store the array size in order to write the data in the output file correctly.
         */
        numberOfInputs = numberHolder;
        for(long long i=0; i<numberOfInputs; i++){
            outputFile << array[i].ID << " " << array[i].CP << endl ;
        }
        delete [] array;
        
    }
    else if(argc ==2 && algorithmType == "./150130136_AoA1_P3_2" && atol(argv[1])==2){
        numberOfInputs = 10000;
        long long stored = numberOfInputs;
        ifstream clanAInput("ClanA.txt");
        ifstream clanBInput("ClanB.txt");
        ifstream gameLogInput("gamelogs.txt");
        ofstream outputFile("A_result.txt");
        if(!clanAInput.is_open()||!clanBInput.is_open()||!gameLogInput.is_open()||!outputFile.is_open()){
            cout << "File Error!" << endl;
            cout << "Program is terminated." << endl;
            return -1;
        }
        players *arrayA = new players[numberOfInputs];
        players *arrayB = new players[numberOfInputs];
        for(long long i=0; i<numberOfInputs; i++){
            clanAInput >> arrayA[i].ID >> arrayA[i].CP;
            clanBInput >> arrayB[i].ID >> arrayB[i].CP;
        }
        logs readLogs;
        while(!gameLogInput.eof()){
            gameLogInput >> readLogs.gameId >> readLogs.clanOfAttacker >> readLogs.attackerPosition >> readLogs.attackedPosition;
            heapSort(arrayA);
            numberOfInputs = stored;        //For the next heap sort
            heapSort(arrayB);
            numberOfInputs = stored;
            
            // By reverse function what we are making is
            // Sorting smallest to largest -> Sorting largest to smallest
            
            reverse(arrayA);
            reverse(arrayB);
            if(readLogs.clanOfAttacker=='A')
                updateCPs(arrayA, arrayB, readLogs);
            else if(readLogs.clanOfAttacker=='B')
                updateCPs(arrayB, arrayA, readLogs);
            else
                cout << "Wrong clan of attackers" << endl;
            if(readLogs.gameId==999)       // To prevent one more unnecessary loop
                break;
        }
        long long totalA = calculateTotalCPs(arrayA, 'A');
        long long totalB = calculateTotalCPs(arrayB, 'B');
        if(totalA>totalB)
            cout << "Clan A wins!!!" << endl;
        else
            cout << "Clan B wins!!!" << endl;
        reverse(arrayA);
        for(long long l = 0; l<numberOfInputs; l++)
            outputFile << arrayA[l].ID << " " << arrayA[l].CP << endl;
        delete [] arrayA;
        delete [] arrayB;
    }
    else{
        cerr << "Not acceptable terminal inputs!" << endl;
        cerr << "Please check again. Program terminated" << endl;
        return -1;
    }
    return 0;
}
