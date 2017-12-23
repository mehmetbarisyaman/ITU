/*
 NAME: MEHMET BARIŞ YAMAN
 NUMBER: 150130136
 DATE: 18/04/2016
*/

#include <vector>   // For the vector that holds the scores of each player
#include "Base.h"
#include <fstream>  // For file operations

using namespace std;

class HetList{
    
    base *head;     // Head of the heterogenous list
    
    //Scores of each player
    
    int lose;
    int win;
    int tie;
    
    FILE *input_file;
    
    string player_name; // Holds the player names such as 'Player 1' or 'Player 2'
    
    int fileindex;      // That holds the index of file

public:
    
    vector<int> scores; // That holds the scores
    
    
    // Setter and Getter Methods
    
    void setName(string name_in){
        player_name = name_in;
    }
    string getName(){
        return player_name;
    }
    int getIndex(){
        return fileindex;
    }
    void setIndex(int z){
        fileindex=z;
    }
    int getTie(){
        return tie;
    }
    void setTie(int z){
        tie=z;
    }
    int getWin(){
        return win;
    }
    void setWin(int z){
        win=z;
    }
    int getLose(){
        return lose;
    }
    void setLose(int z){
        lose=z;
    }
    
    HetList(string, int);
    
    void insert(base *);        // For inserting a card to the list
    
    ~HetList();
    
    void print();               // Printing the cards that players choose(first two lines of  the output)
    
    void count(HetList *);      // To allocate the players win, lose and tie numbers
};

// count() function takes an other player as an arguement because we can determine the players win, lose
// and tie numbers by comparing the other players cards

void HetList::count(HetList *player){
    
    // Pointers that hold the cards of players

    base *temp1 = this->head;
    base *temp2 = player->head;
    
    while(temp1 && temp2){
        
        temp1->compare(temp2->card);    // Returns the score by comparing two cards
        
        if(temp1->result==2)
            this->win++;
        else if(temp1->result==1)
            this->tie++;
        else if(temp1->result==0)
            this->lose++;
        
        scores.push_back(temp1->result);    // Pushing score to the vector
        
        temp1 = temp1->next;            // Pointing to the next card of the player
        temp2 = temp2->next;
    }
}

HetList::HetList(string name, int x){
    
    fileindex=x;                    // fileindex provides the synchronization of the file reading
    tie = win = lose = 0;           // Initial value of the numbers
    player_name = name;             // Name of the player is taken by an arguement
    
    input_file = fopen("deck.txt", "r");
    char holder;                    // Temporarily holds the first character in the file
    fseek(input_file, fileindex, SEEK_SET);
    fread(&holder, sizeof(char), 1, input_file);

    if(holder=='R')
        this->head = new red;
    else if(holder=='D')
        this->head = new dark_red;
    else if(holder=='B')
        this->head = new blue;
    else if(holder=='G')
        this->head = new green;
    
    head->next = NULL;
    head->card = holder;
    
    fileindex++;                    // fileindex should increment when the file pointer increments
    
    red *temp = new red;          // A temporary pointer for reading cards from the input file
    
// Since Abstract class objects could not be created another class is used for temporary pointer(not base)
    
    while(!feof(input_file)){
        fread(&temp->card, sizeof(char), 1, input_file);
        
        this-fileindex++;
        
        if(temp->card=='\n')        // Stop reading if the line is ended
            break;
        
        if(!feof(input_file))
            if(temp->card=='R'||temp->card=='D'||temp->card=='G'||temp->card=='B')
                insert(temp);
    }
    
    fclose(input_file);             // Each player opens the file and closes
    delete temp;
    
}

HetList::~HetList(){                // All allocated space for the list must be given back
    base * temp = head;
    while(head){
        temp = head;
        head = head->next;          // We can change the place of head while destructing the list
        delete temp;
    }
}

void HetList::print(){              // To print the first two lines of the output
    cout << player_name << ": ";
    base * temp = head;
    while(temp){
        cout << temp->card << " ";
        temp = temp->next;
    }
    cout << endl;
}


void HetList::insert(base *t){      // Inserting a card to the HetList
    base *new_base;
    
    if(t->card=='R')
        new_base = new red;
    else if(t->card=='D')
        new_base = new dark_red;
    else if(t->card=='B')
        new_base = new blue;
    else if(t->card=='G')
        new_base = new green;
    
    *new_base = *t;
    new_base->next = NULL;          // The data is given to the new card pointer
    
    base *traverse, *tail;          // Two pointers are needed for adding a new card
    tail = head;
    traverse = head->next;
    
    while(traverse){
        tail = traverse;
        traverse = traverse->next;
    }
    tail->next = new_base;          // New card is placed at the end of the list
}


