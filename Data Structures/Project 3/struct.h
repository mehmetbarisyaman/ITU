/*
 * @Author
 * Student Name: Mehmet Barış Yaman
 * Student ID : 150130136
 * Date:11/12/2015
 */

//The data type, with that we implement cards is the node struct

struct Node{
    char cesit;
    int number;
    Node *next;
};

//I used stacks for implementing the temporary card space and the cards which are matched

typedef Node DataType;

struct Stack{
    Node *head;
    void create();
    void close();
    void push(char, int);
    DataType pop();
    bool isempty();
};

//This queue struct is for the deck of the cards

struct Queue{
    FILE *output;
    int kartsirasi;
    int kartsirasi2;
    int kartsirasi3;
    Node * front;
    Node * back;
    void writetofile(Node *, int, int);
    void create();
    void close();
    void enqueue(char, int);
    DataType dequeue();
    bool isempty();
    
};
