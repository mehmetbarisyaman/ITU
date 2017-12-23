/*
 NAME: MEHMET BARIŞ YAMAN
 NUMBER: 150130136
 DATE: 18/04/2016
 */

#include <iostream>
#include "HetList.h"

using namespace std;

void displayResults(int w1, int t1, int l1, vector<int>&v1, HetList *peace){
    
    // The win, lose and tie numbers are obtained from getter methods
    
    int w2 = peace->getWin();   // Win number of the second player
    int t2 = peace->getTie();   // Tie number of the second player
    int l2 = peace->getLose();  // Lose number of the second player
    
    // Iterator is needed for displaying the scores of the cards
    
    cout << "Score1: ";
    
    vector<int>::iterator i;
    for(i = v1.begin(); i!=v1.end(); i++)
        cout << *i << ' ';
    cout << endl;
    
    cout << "Score2: ";
    for(i = peace->scores.begin(); i!=peace->scores.end(); i++)
        cout << *i << ' ';
    cout << endl;
    
    // Displaying the total scores
    
    cout << "Total Score1: ";
    cout << "Lose:" << l1 << " ";
    cout << "Tie:" << t1 << " ";
    cout << "Win:" << w1 << " " << endl;
    cout << "Total Score2: ";
    cout << "Lose:" << l2 << " ";
    cout << "Tie:" << t2 << " ";
    cout << "Win:" << w2 << " " << endl;
    
    // Displaying the final result
    
    if(w1 > w2)
        cout << "Winner: Player1" << endl;
    else if(w1==w2)
        cout << "Winner: Nobody" << endl;
    else
        cout << "Winner: Player2" << endl;
}



int main(int argc, const char * argv[]) {
    HetList *Player1 = new HetList("Player1", 0);
    int index = Player1->getIndex();            // To pass the file index
    HetList *Player2 = new HetList("Player2", index);
    
    // Two player objects are created dynamically
    
    Player1->print();
    Player2->print();
    Player1->count(Player2);
    Player2->count(Player1);
    
    // Win, lose and tie numbers are determined for each player
    
    int p1 = Player1->getWin();
    int p2 = Player1->getTie();
    int p3 = Player1->getLose();
    
    // Score arguements of the player are passed to variables for displayResults function
    
    displayResults(p1, p2, p3, Player1->scores, Player2);
    
    return 0;
}
