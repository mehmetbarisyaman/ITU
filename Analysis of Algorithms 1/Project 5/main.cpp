/*
 Student Name: Mehmet Barış Yaman
 Student ID: 150130136
 */

#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

class node{
public:
    string name;
    char gender;
    int age;
    char color;
    node *parent;
    node *leftChild;
    node *rightChild;
    node(string givenName=" ", char givenGender=' ', int givenAge=0);
    int numberOfAdults;
    int numberOfNonAdults;
};

node::node(string givenName, char givenGender, int givenAge){
    this->name = givenName;
    this->gender = givenGender;
    this->age = givenAge;
    this->parent = this->leftChild = this->rightChild = NULL;
    this->color = 'R';
    numberOfAdults = numberOfNonAdults = 0;
}


class redBlackTree{
public:
    node *root;
    node *nil;
    redBlackTree();
    bool isALeftChild(node *selectedNode);         // if the node is left child or not
    bool isARightChild(node *selectedNode);         // if the node is light child or not
    void adjustTree(node *newNode);
    void rightRotate(node *pointer);
    void leftRotate(node *pointer);
    void violationCaseZero();
    void violationCaseOne(node *insertedNode);
    void violationCaseTwo(node *insertedNode);
    void violationCaseThree(node *insertedNode);
    void insertNode(node *toAdd);
    void preOrderPrint(node *selectedNode);
    void swapColors(node *node1, node *node2);      // for swapping the colors between nodes
    void changeColor(node *selectedNode);           // if black make red or vice versa
    int countAdultNodes(node *);
    int countNonAdultNodes(node *);
    node *osAdultSelect(node *, int x);
    node *osNonAdultSelect(node *, int x);
    void deleteWholeTree(node *);
    ~redBlackTree();
};


redBlackTree::~redBlackTree(){
    deleteWholeTree(this->root);
    delete nil;
}



node * redBlackTree::osAdultSelect(node *selected, int i){
    int k =selected->leftChild->numberOfAdults;
    if(selected->age >=18)
        k++;
    if(i==k){
        if(selected->age<18){
            /*
             If non adult node is accessed in that operation we need to look at the left and right
             child adult numbers and choose one correspondingly. 
             If the number is less than we are looking for we cant access the order. 
             For example, we cant talk about the 5. order in a place that includes less than 5 nodes
             */
            if(selected->leftChild->numberOfAdults>=i)
                return osAdultSelect(selected->leftChild, i);
            if(selected->rightChild->numberOfAdults>=i)
                return osAdultSelect(selected->rightChild, i);
        }
        return selected;
    }
    else if (i < k)
        return osAdultSelect(selected->leftChild, i);
    else
        return osAdultSelect(selected->rightChild, i-k);
}

node * redBlackTree::osNonAdultSelect(node *selected, int i){
    int k=selected->leftChild->numberOfNonAdults;
    if(selected->age <18)
        k++;
    if(i==k){
        // Same event in osAdultSelect function
        if(selected->age>=18){
            if(selected->leftChild->numberOfNonAdults>=i)
                return osNonAdultSelect(selected->leftChild, i);
            if(selected->rightChild->numberOfNonAdults>=i)
                return osNonAdultSelect(selected->rightChild, i);
        }
        return selected;
    }
    
    else if (i < k)
        return osNonAdultSelect(selected->leftChild, i);
    else
        return osNonAdultSelect(selected->rightChild, i-k);
}

int redBlackTree::countAdultNodes(node *traverse){
    if(traverse==nil)   // Dont count the nil nodes
        return 0;
    if(traverse->age>=18)
        traverse->numberOfAdults=1;
    int left = countAdultNodes(traverse->leftChild);
    int right = countAdultNodes(traverse->rightChild);
    traverse->numberOfAdults += left;
    traverse->numberOfAdults += right;
    return traverse->numberOfAdults;
}


// Resembles the countAdultNodes function
int redBlackTree::countNonAdultNodes(node *traverse){
    if(traverse == nil)
        return 0;
    if(traverse->age<18)
        traverse->numberOfNonAdults=1;
    int left = countNonAdultNodes(traverse->leftChild);
    int right = countNonAdultNodes(traverse->rightChild);
    traverse->numberOfNonAdults += left;
    traverse->numberOfNonAdults += right;
    return traverse->numberOfNonAdults;
}

void redBlackTree::changeColor(node *selectedNode){
    if(selectedNode->color=='B')
        selectedNode->color = 'R';
    else
        selectedNode->color = 'B';
}

void redBlackTree::deleteWholeTree(node *p){
    if(p && p!=nil){
        if(p->leftChild){
            deleteWholeTree(p->leftChild);
            p->leftChild = NULL;
        }
        if(p->rightChild){
            deleteWholeTree(p->rightChild);
            p->rightChild =NULL;
        }
        delete p;
    }
}


void redBlackTree::swapColors(node *node1, node *node2){
    node store;
    store.color = node1->color;
    node1->color = node2->color;
    node2->color = store.color;
}

bool redBlackTree::isALeftChild(node *selectedNode){
    return (selectedNode== selectedNode->parent->leftChild) ? true : false;
}

bool redBlackTree::isARightChild(node *selectedNode){
    return (selectedNode== selectedNode->parent->rightChild) ? true : false;
}

void redBlackTree::violationCaseZero(){
    root->color = 'B';
    root->parent = NULL;
}

redBlackTree::redBlackTree(){
    nil = new node();
    nil ->color = 'B';
    root = nil;
}

void redBlackTree::leftRotate(node *pointer){
    node *right = pointer->rightChild;
    pointer->rightChild = right->leftChild;
    if(pointer->rightChild!=nil && pointer->rightChild)
        pointer->rightChild->parent = pointer;
    right ->parent = pointer->parent;
    if(pointer->parent==nil)
        root = right;
    else if(isALeftChild(pointer))
        pointer->parent->leftChild = right;
    else
        pointer->parent->rightChild = right;
    right->leftChild = pointer;
    pointer->parent = right;
}

void redBlackTree::rightRotate(node *pointer){
    node *left = pointer->leftChild;
    pointer->leftChild = left->rightChild;
    if(pointer->leftChild!=nil)
        pointer->leftChild->parent = pointer->parent;
    left->parent = pointer->parent;
    if(pointer->parent==nil)
        root = left;
    else if(isALeftChild(pointer))
        pointer->parent->leftChild = left;
    else
        pointer->parent->rightChild = left;
    left->rightChild = pointer;
    pointer->parent = left;
}

void redBlackTree::preOrderPrint(node *selectedNode){
    if(selectedNode==nil)
        return;
    node *store = selectedNode;
    while(selectedNode->parent){
        cout << "-";
        selectedNode = selectedNode->parent;
        if(selectedNode == nil)
            break;
    }
    selectedNode = store;
    cout << "(" << selectedNode->color << ") " <<selectedNode->name << " " << selectedNode->gender << " " << selectedNode->age <<endl;
    preOrderPrint(selectedNode->leftChild);
    preOrderPrint(selectedNode->rightChild);
}

void redBlackTree::insertNode(node *toAdd){
    node *traverse, *newNode;
    traverse = root;
    bool inserted=false;
    newNode = new node(toAdd->name, toAdd->gender, toAdd->age);
    newNode->parent = nil;
    newNode -> leftChild = nil;
    newNode -> rightChild=nil;
    if(root==nil){
        root = newNode;
        violationCaseZero();
        return;
    }
    while(traverse!=nil && !inserted){
        if(newNode->name < traverse->name){
            if(traverse->leftChild!=nil)
                traverse = traverse->leftChild;
            else{
                traverse->leftChild = newNode;
                inserted=true;
                newNode->parent = traverse;
            }
        }
        else if(newNode->name > traverse->name){
            if(traverse->rightChild!=nil)
                traverse = traverse->rightChild;
            else{
                traverse->rightChild = newNode;
                inserted = true;
                newNode->parent = traverse;
            }
        }
        else
            cout << "Data can not repeat" << endl;
    }
    adjustTree(newNode);
}

// If the uncle node is red
void redBlackTree::violationCaseOne(node *insertedNode){
    changeColor(insertedNode->parent->parent->leftChild);
    changeColor(insertedNode->parent->parent->rightChild);
    changeColor(insertedNode->parent->parent);
}

// If the uncle node is black
void redBlackTree::violationCaseTwo(node *insertedNode){
    if(isARightChild(insertedNode))
        leftRotate(insertedNode->parent);
    else
        rightRotate(insertedNode->parent);
}

// If the uncle node is black
void redBlackTree::violationCaseThree(node *insertedNode){
    swapColors(insertedNode->parent, insertedNode->parent->parent);
    if(isALeftChild(insertedNode))
        rightRotate(insertedNode->parent->parent);
    else
        leftRotate(insertedNode->parent->parent);
}

// A kind of fix function
void redBlackTree::adjustTree(node *newNode){
    node *traverse;
    if(newNode->parent){
        while(newNode ->parent->color=='R'){
            if(isALeftChild(newNode->parent)){
                traverse = newNode->parent->parent->rightChild;
                if(traverse->color == 'R')
                    violationCaseOne(newNode);
                else{
                    if(isARightChild(newNode))
                        violationCaseTwo(newNode);
                    violationCaseThree(newNode);
                }
            }
            else {
                traverse = newNode->parent->parent->leftChild;
                if(traverse->color == 'R')
                    violationCaseOne(newNode);
                else{
                    if(isALeftChild(newNode))
                        violationCaseTwo(newNode);
                    violationCaseThree(newNode);
                }
            }
            
        }
    }
    violationCaseZero();    // If the root is red
}

int main(int argc, const char * argv[]) {
    if(argc!=2){
        cout << "Not enough input arguments!" << endl;
        return -1;
    }
    ifstream inputFile(argv[1]);
    if(!inputFile){
        cout << "Input file error!" << endl;
        return -1;
    }
    redBlackTree tree;
    string takenName;
    char takenGender;
    int takenAge;
    while(!inputFile.eof()){
        inputFile >> takenName >> takenGender >> takenAge;
        node *newNode = new node(takenName, takenGender, takenAge);
        tree.insertNode(newNode);
    }
    tree.countAdultNodes(tree.root);
    tree.countNonAdultNodes(tree.root);
    tree.preOrderPrint(tree.root);
    int adultOrder = 5;
    int nonAdultOrder= 3;
    node *selectedAdult = tree.osAdultSelect(tree.root, adultOrder);
    node *selectedNonAdult = tree.osNonAdultSelect(tree.root, nonAdultOrder);
    cout << adultOrder << "th adult: " << selectedAdult->name << endl;
    cout << nonAdultOrder << "rd non-adult: " << selectedNonAdult->name << endl;
    return 0;
}
