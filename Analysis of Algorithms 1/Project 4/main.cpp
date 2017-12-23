//
//  main.cpp
//  Spell Checker
//
//  Created by Baris on 12/3/16.
//  Copyright © 2016 Mehmet Barış Yaman. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>

using namespace std;


int N;  // holds the number of input words



class myHash{
public:
    int _tableSize;
    int collision;
    myHash(int tableSize);
    void insert(string s);
    int hashFunc(string s, int index);
    bool retrieve(string s);
    bool remove(string s);
    string returnAMember(int index);
    ~myHash();
private:
    string *myHashTable;
};



myHash::myHash(int tableSize){
    collision=0;
    _tableSize = tableSize;
    myHashTable = new string[_tableSize];
    for(int j=0; j<tableSize; j++){
        myHashTable[j] = "-";       // Loaded with empty string
    }
}

myHash::~myHash(){
    delete [] myHashTable;
}

int myHash:: hashFunc(string s, int index){
    long long int result = 1;
    for (int i=0; i<s.length(); ++i)
    {
        // Mod calculated each time since the value can be too large to store
        result = (result * (int(s[i])))%N;
    }
    return int(result)+index;   // Linear probing is provided
}

// To write the table data to output file in a simpler way
string myHash::returnAMember(int index){
    return myHashTable[index];
}


void myHash::insert(string s){
    int index;
    bool isInserted =  false;
    int scopeCollision=0;       // used for printing collision output
    for(int k=0; k<_tableSize; k++){
        index = hashFunc(s, k);
        if(index>=_tableSize)
            index = index % _tableSize;     // Return the beginning for search if no empty slot
        if(k==0)
            cout << "h(x) result is " << index << " for: " << s << endl;
        // Deleted and empty slots are same
        if(myHashTable[index]=="-" || myHashTable[index]=="*"){
            myHashTable[index] = s;
            cout<<"INSERT: The word '"<<s<<"' is put in the cell number "<<index<<endl;
            isInserted = true;
            break;
        }
        else if(myHashTable[index]==s){
            cout<<"WARNING: The word '"<<s<<"' is already in the dictionary!"<<endl;
            isInserted=true;    // To prevent the warning output (below) is printed
            break;
        }
        else{
            scopeCollision++;
            collision++;
        }
        
    }
    if(!isInserted){
        cout<<"WARNING: There are no empty space in the hash table for the word: "<<s<<endl;
    }
    cout << "COLLISIONS :" << scopeCollision << endl;
    cout << "-----------------------------------------------------------------"<<endl;
}

bool myHash::remove(string s){
    int index;
    bool isDeleted=false;
    int scopeCollision=0;
    for(int i = 0; i<_tableSize; i++){
        index = hashFunc(s, i);
        if(i==0)
            cout << "h(x) result is " << index << " for: " << s << endl;
        if(index>=_tableSize)
            index = index % _tableSize;
        if(myHashTable[index]==s){
            myHashTable[index]="*";     // Lazy deletion
            cout<<"REMOVE: The word '"<<s<<"' is removed from the dictionary."<<endl;
            isDeleted=true;;
            break;
        }
        else if(myHashTable[index]=="-")
            break;
        else{
            collision++;
            scopeCollision++;
        }
    }
    if(!isDeleted){
        cout<<"WARNING: The word '"<<s<<"' couldn't be found in the dictionary"<<endl;
    }
    cout << "COLLISIONS :" << scopeCollision << endl;
    cout << "-----------------------------------------------------------------"<<endl;
    return isDeleted;
}


void spellChecker(myHash *hash,string *hashTable, string word){
    long finishPoint = word.length();
    int index=0;        // holds the index of character changed in the string
    string store = word;    // the word needs to be stored for return
    while(index!=finishPoint){
        for(int i=97; i<=122; i++){
            word[index] = char(i);      // changing the character
            for(int k=0; k<N; k++){     // Searching the updated word
                int counter = hash->hashFunc(word, k);
                if(counter>=hash->_tableSize)
                    counter = counter % hash->_tableSize;
                if(hashTable[counter]==word){
                    cout << hashTable[counter] << " ";
                }
            }
        }
        index++;                    // Change the index of character
        word=store;
        if(index==finishPoint)      // If last character is changed and searched, stop
            break;
    }
    cout << endl;
}

bool myHash::retrieve(string s){
    bool isRetrieved=false;
    int index;
    int scopeCollision=0;
    for(int i=0; i<_tableSize; i++){
        index = hashFunc(s, i);
        if(i==0)
            cout << "h(x) result is " << index << " for: " << s << endl;
        if(index>=_tableSize)
            index = index % _tableSize;
        if(myHashTable[index]==s){
            isRetrieved=true;
            cout<<"RETRIEVE: The word '"<<s<<"' found in the dictionary with index: "<<index<<endl;
            break;
        }   // Deleted and empty slots are not same for this operation
        if(myHashTable[index]=="-"){
            break;
        }
        collision++;
        scopeCollision++;
    }
    if(!isRetrieved){
        cout<<"WARNING: The word '"<<s<<"' couldn't be found in the dictionary"<<endl;
        cout << "Looking for possible suggestions." << endl;
        cout << "SUGGESTIONS for " << s << ": ";
        // myHashTable is sent for hash function in the spell checker
        spellChecker(this, myHashTable, s);
        
    }
    cout << "COLLISIONS :" << scopeCollision << endl;
    cout << "-----------------------------------------------------------------"<<endl;
    return isRetrieved;
}


int main(int argc, const char * argv[]) {
    N = atoi(argv[2]);
    string inputFileName = argv[1];
    ifstream inputFile(inputFileName);
    ofstream outputFile("150130136_output.txt");
    if(argc!=3){
        cout << "ERROR!! Not acceptable terminal inputs" << endl;
        return -1;
    }
    if (!inputFile.is_open() || !outputFile.is_open()){
        cout << "ERROR!! File could not be opened" << endl;
        return -1;
    }
    myHash *hash = new myHash(N);
    string taken;
    while(!inputFile.eof()){
        inputFile >> taken;
        //Checking the first character of the line in the input file
        if(taken[0]=='i'){
            taken = taken.substr(7, taken.length());
            hash->insert(taken);
        }
        else if(taken[0]=='d'){
            taken = taken.substr(7, taken.length());
            hash->remove(taken);
        }
        else if(taken[0]=='r'){
            taken = taken.substr(9, taken.length());
            hash->retrieve(taken);
        }
    }
    inputFile.close();
    for(int x=0; x<hash->_tableSize; x++){
        outputFile << x << ":" <<hash->returnAMember(x) << endl;
    }
    cout << "-----------------------------------" << endl;
    cout << "TOTAL COLLISIONS: " << hash->collision << endl;
    cout << "Final form of hash table is saved as 150130136_output.txt" << endl;
    delete hash;
    return 0;
}
