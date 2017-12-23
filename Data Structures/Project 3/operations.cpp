/*
 * @Author
 * Student Name: Mehmet Barış Yaman
 * Student ID : 150130136
 * Date:11/12/2015
 */

#include <stdio.h>
#include <string.h>
#include <iostream>
#include "struct.h"

using namespace std;

void Stack::create(){
    head = NULL;
}

void Stack::close(){
    Node * p;
    while(head){
        p=head;
        head = head->next;
        delete p;
    }
}

void Stack::push(char newchar, int newnumber){
    Node *newnode = new Node;
    newnode->number = newnumber;
    newnode->cesit = newchar;
    newnode->next = head;
    head = newnode;
}

DataType Stack::pop(){
    Node * topnode;
    DataType temp;
    topnode = head;
    head = head->next;
    temp.cesit = topnode->cesit;
    temp.number = topnode->number;
    delete topnode;
    return temp;
}

bool Stack::isempty(){
    return head==NULL;
}

//The output file, queue structure and kartsirasi values (for writing to the output file) will be created

void Queue::create(){
    front = NULL;
    back = NULL;
    if ( !(output = fopen( "output_file.txt", "w+" ) ) ) {
        if ( !(output = fopen( "output_file.txt", "r+" ) ) ) {
            cerr << "File could not be opened." << endl;
            cerr << "Will work in memory only." << endl;
            return;
        } }
    fseek(output, 0, SEEK_SET);
    kartsirasi=1;
    kartsirasi2=0;
    kartsirasi3=0;
}

void Queue::close(){
    Node * p;
    while(front){
        p=front;
        front = front->next;
        delete p;
    }
}

void Queue::enqueue(char newchar, int newnumber){
    Node * newnode = new Node;
    newnode->number = newnumber;
    newnode->cesit = newchar;
    newnode->next = NULL;
    if(isempty()){
        back = newnode;
        front = back;
    }
    else{
        back->next = newnode;
        back = newnode;
    }
}

DataType Queue::dequeue(){
    Node * topnode;
    DataType temp;
    topnode = front;
    front = front->next;
    temp.number = topnode->number;
    temp.cesit = topnode->cesit;
    delete topnode;
    return temp;
}

bool Queue::isempty(){
    return front == NULL;
}

void Queue::writetofile(Node *kart, int durum, int match){

    //This state is for writing the shuffled deck to the output file
    //The match value is not for this state
    //kartsirasi values are for this state
    
    if(durum==1){
        if(kartsirasi==1){
            fwrite("Shuffled deck:", sizeof("Shuffled deck:"), 1, output);
            fwrite("\n", sizeof("\n"), 1, output);
        }
        fprintf(output, "%d", kartsirasi);
        kartsirasi++;
        fwrite(":", 1, 1, output);
        fwrite(&kart->cesit, sizeof(kart->cesit), 1, output);
        fwrite(":", 1, 1, output);
        if(kart->number==1){
            fwrite("A", 1, 1, output);
        }
        else if(kart->number>=11){
            if(kart->number==11)
                 fwrite("J", 1, 1, output);
            else if(kart->number==12)
                fwrite("Q", 1, 1, output);
            else if(kart->number==13)
                fwrite("K", 1, 1, output);
        }
        else
            fprintf(output, "%d", kart->number);
        fwrite("\n", sizeof("\n"), 1, output);
    }
    
    //This state is for writing the shuffled deck and the matching value to the output file
    //The match value tells whether match occurs or not
    //kartsirasi2 values are for this state
    
    if(durum==2){
        if(kartsirasi2==0){
            fwrite("Fortune-telling steps:", sizeof("Fortune-telling steps:"), 1, output);
            fwrite("\n", sizeof("\n"), 1, output);
            kartsirasi2++;
        }
        fprintf(output, "%d", kartsirasi2);
        kartsirasi2++;
        fwrite(":", 1, 1, output);
        fwrite(&kart->cesit, sizeof(kart->cesit), 1, output);
        fwrite(":", 1, 1, output);
        if(kart->number==1){
            fwrite("A", 1, 1, output);
        }
        else if(kart->number>=11){
            if(kart->number==11)
                fwrite("J", 1, 1, output);
            else if(kart->number==12)
                fwrite("Q", 1, 1, output);
            else if(kart->number==13)
                fwrite("K", 1, 1, output);
        }
        else
            fprintf(output, "%d", kart->number);
        fwrite(":", 1, 1, output);
        if(match==1){
            fwrite("match", sizeof("match"), 1, output);
            kartsirasi2 = 1;
        }
        else if(match==0){
            fwrite("unmatch", sizeof("unmatch"), 1, output);
            if(kartsirasi2==14)
                kartsirasi2=1;
        }
        fwrite("\n", sizeof("\n"), 1, output);
    }
    
    //This state is for writing only the cards that are matched before the summation to the output file
    //The match value is not valid for this state
    //kartsirasi3 values are for this state
    
    if(durum==3){
        if(kartsirasi3==0){
            fwrite("Fortune Results:", sizeof("Fortune Results:"), 1, output);
            fwrite("\n", sizeof("\n"), 1, output);
            kartsirasi3++;
        }
        fwrite(&kart->cesit, sizeof(kart->cesit), 1, output);
        fwrite(":", 1, 1, output);
        if(kart->number==1){
            fwrite("A", 1, 1, output);
        }
        else if(kart->number>=11){
            if(kart->number==11)
                fwrite("J", 1, 1, output);
            else if(kart->number==12)
                fwrite("Q", 1, 1, output);
            else if(kart->number==13)
                fwrite("K", 1, 1, output);
        }
        else
            fprintf(output, "%d", kart->number);
        fwrite("\n", sizeof("\n"), 1, output);
    }
    
    //This state is for writing the summation of the cards and the result of the game
    //The match value is the summation result in this state
    //kartsirasi3 values are for this state
    
    if(durum==4){
        fwrite("Sum: ", sizeof("Sum: "), 1, output);
        fprintf(output, "%d", match);
        fwrite("\n", sizeof("\n"), 1, output);
        if(match>=50)
            fwrite("Fortune will come true", sizeof("Fortune will come true"), 1, output);
        else if(match <50)
            fwrite("Bad luck!", sizeof("Bad luck!"), 1, output);
        fwrite("\n", sizeof("\n"), 1, output);
    }
}
