/*
 * @Author
 * Student Name: Mehmet Barış Yaman
 * Student ID : 150130136
 * Date:11/12/2015
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include "struct.h"

typedef Queue baris;
baris deste; //deck of the cards

typedef Stack temporaryspace;
temporaryspace yer;
temporaryspace eslesen;

using namespace std;

DataType kartlar[52]; // This is the array for shuffling the cards

bool karistirildi; // That tells if the cards are shuffled or not

int sira; // This is the number that the fortune teller says

int shufflepoint; //This is for shuffling the cards

void fortune_teller(){
    if(::karistirildi==false){
        if(::shufflepoint==51){
            ::karistirildi=true; // Finishing the shuffling
            for(int y=0; y<52; y++){
                deste.enqueue(kartlar[y].cesit, kartlar[y].number);
                deste.writetofile(&kartlar[y], 1, 0);
            }
        }
        swap(kartlar[::shufflepoint], kartlar[rand()%52]);
        ::shufflepoint++;
         return fortune_teller();
    }
    
    //Final state of the game
    
    if(deste.isempty()){
        int toplam = 0;
        while(!eslesen.isempty()){
            DataType h = eslesen.pop();
            deste.writetofile(&h, 3, 0);
            if(h.number==1||h.number==11||h.number==12||h.number==13)
                h.number=10;
            toplam = toplam + h.number;
        }
        deste.writetofile(&kartlar[1], 4, toplam);
        return;
    }
    else{
        
        //Making the temporary place empty after comparing the 13.sequence
        
        if(::sira>=14){
            while(!yer.isempty())
                yer.pop();
            ::sira = 1;
            return fortune_teller();
        }
        DataType g = deste.dequeue();
        
        //If no match occurs
        
        if(::sira!=g.number){
            yer.push(g.cesit, g.number);
            ::sira++;
            deste.writetofile(&g, 2, 0);
            fortune_teller();
        }
        
        //If a match occurs (taking the cards back from temporary space)
        
        else{
            while(!yer.isempty()){
                DataType x = yer.pop();
                deste.enqueue(x.cesit, x.number);
            }
            eslesen.push(g.cesit, g.number);
            deste.writetofile(&g, 2, 1);
            ::sira = 1;
            fortune_teller();
        }
    }
}

//Creating the deck before shuffling

void desteolustur(){
    for(int dizisirasi=0; dizisirasi<=51; dizisirasi++){
        if(dizisirasi<=12){
            kartlar[dizisirasi].cesit = 'H';
            kartlar[dizisirasi].number = dizisirasi+1;
        }
        else if(dizisirasi>12&&dizisirasi<=25){
            kartlar[dizisirasi].cesit = 'D';
            kartlar[dizisirasi].number = dizisirasi-12;
        }
        else if(dizisirasi>25&&dizisirasi<=38){
            kartlar[dizisirasi].cesit = 'S';
            kartlar[dizisirasi].number = dizisirasi-25;
        }
        else if(dizisirasi>38){
            kartlar[dizisirasi].cesit = 'C';
            kartlar[dizisirasi].number = dizisirasi-38;
        }
    }
}

//This function is for shuffling the cards

void swap(Node * a, Node * b){
    int x = rand()%2;
    if(x==1){
        Node * temp;
        temp = a;
        a = b;
        b = temp;
    }
}

int main() {
    deste.create();
    yer.create();
    eslesen.create();
    desteolustur();
    srand(time(NULL));
    
    //Giving the first values to global variables
    
    ::karistirildi = false;
    ::sira = 1;
    ::shufflepoint=0;
    
    fortune_teller();
    deste.close();
    eslesen.close();
    yer.close();
    return 0;
}
