/*
 Student Name: Mehmet Barış Yaman
 Student Number: 150130136
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <ctime>
#include <string.h>
using namespace std;

long long maxNumber;    // Used in defining array lengths

string algorithm;

string outputFileName;

class Block{
public:
    int xCoordinate;
    int yCoordinate;
    int length;
    char direction;
    int blockNumber;    // Used in printing representation of steps
    Block(int inXCoordinate, int inYCoordinate, int inLength, char inDirection, int inBlockNumber);
};

Block::Block(int inXCoordinate, int inYCoordinate, int inLength, char inDirection, int inBlockNumber){
    // Converting coordinates to array index values
    this->xCoordinate = inXCoordinate-1;
    this->yCoordinate = inYCoordinate-1;
    this->length = inLength;
    this->direction = inDirection;
    this->blockNumber = inBlockNumber;
}

class Node{
public:
    vector<Block>environment;
    char representation[6][6];      // used for printing the steps
    void addBlock(Block newBlock);
    void popBlock();
    void updateRepresentation();
    void represent();
    int nodeNumber;
    int backNumber;
    bool terminate;
    Node();
};

Node::Node(){
    terminate=false;
    backNumber=-1;
}




void Node::addBlock(Block newBlock){
    // Node is the whole environment, thus blocks can be added in generating nodes
    environment.push_back(newBlock);
}

//Needed for dismissing the end of file problem in c++(explained in main function)
void Node::popBlock(){
    environment.pop_back();
}


//Needed for updating the representation, after position changes of blocks
/*
 Blocks are represented with the numbers, emptinesses represented as '.' and the exit gate of the environment is represented with 'E'
 */

void Node::updateRepresentation(){
    for(int j=0; j<6; j++)
        for(int k=0; k<6; k++)
            representation[j][k] = '.';
    for(vector<Block>::iterator it = environment.begin(); it!=environment.end(); it++){
        if(it->direction=='h')
            for(int i=0; i<it->length; i++)
                representation[it->xCoordinate][it->yCoordinate+i] = '0' + it->blockNumber;
        else if(it->direction=='v')
            for(int i=0; i<it->length; i++)
                representation[it->xCoordinate-i][it->yCoordinate] = '0' + it->blockNumber;
    }
    representation[2][5]='E';
}

//Print the node to the console
void Node::represent(){
    for(int a=0; a<6; a++){
        for(int b=0; b<6; b++)
            cout << representation[a][b];
        cout << endl;
    }
    cout << endl;
    cout << endl;
    cout << endl;
}


class Graph{
public:
    int numberOfNodes;      // used in node generation process
    Graph();
    void bfs(Node s);
    void dfs(Node u);
    vector<Node>nodes;
    void addNodes(Node s);
    bool checkIfNodeWasDiscovered(vector<Node>discovered, Node s);  // cycle check
    void createOutput(Node s);
    Node pushNodesForQueue(list<Node>&queue, Node s);
    Node pushNodesForStack(stack<Node>&stack, Node s);
};


Graph::Graph(){
    numberOfNodes=0;
}


void Graph::addNodes(Node s){
    nodes.push_back(s);
}



// check whether there is a cycle

bool Graph::checkIfNodeWasDiscovered(vector<Node>discovered, Node s){
    bool result=false;
    for(vector<Node>::iterator it=discovered.begin(); it!=discovered.end(); it++){
        if(memcmp(it->representation,s.representation, sizeof(it->representation))==0)
            result=true;
    }
    return result;
}

Node Graph::pushNodesForQueue(list<Node>&queue, Node s){
    Node storeNode = s;
    for(vector<Block>::iterator it=s.environment.begin(); it!=s.environment.end(); it++){
        // to make a single move and create a new node
        if(it->direction == 'h'){
            if(s.representation[it->xCoordinate][it->yCoordinate-1]=='.' && it->yCoordinate>0){
                it->yCoordinate--;
                s.updateRepresentation();
                s.backNumber = storeNode.nodeNumber;
                queue.push_back(s);
                this->numberOfNodes++;
                s = storeNode;
            }
            if(s.representation[it->xCoordinate][it->yCoordinate+it->length]=='.' && it->yCoordinate+it->length<=5){
                if(it->blockNumber==0){
                    // for determining if there is a path between the first block and the exit gate
                    for(int i=1; i<6; i++){
                        if(s.representation[it->xCoordinate][it->yCoordinate+it->length+i]=='E'){
                            // Path is found, stop the search
                            s.terminate=true;
                            this->numberOfNodes++;
                            return s;
                        }
                        else if (s.representation[it->xCoordinate][it->yCoordinate+it->length+i]=='.'){
                            continue;
                        }
                        else{
                            break;
                        }
                    }
                }
                it->yCoordinate++;
                s.backNumber = storeNode.nodeNumber;
                queue.push_back(s);
                this->numberOfNodes++;
                s = storeNode;
            }
        }
        else if (it->direction=='v'){
            if(s.representation[it->xCoordinate+1][it->yCoordinate]=='.' && it->xCoordinate<5){
                it->xCoordinate++;
                s.updateRepresentation();
                this->numberOfNodes++;
                s.backNumber = storeNode.nodeNumber;
                queue.push_back(s);
                s = storeNode;
            }
            if(s.representation[it->xCoordinate-it->length][it->yCoordinate]=='.' && it->xCoordinate-it->length>=0){
                it->xCoordinate--;
                s.updateRepresentation();
                this->numberOfNodes++;
                s.backNumber = storeNode.nodeNumber;
                queue.push_back(s);
                s = storeNode;
            }
        }
    }
    return s;
}

Node Graph::pushNodesForStack(stack<Node>&stack, Node s){
    Node storeNode = s;
    for(vector<Block>::iterator it=s.environment.begin(); it!=s.environment.end(); it++){
        // to make a single move and create a new node
        if(it->direction == 'h'){
            if(s.representation[it->xCoordinate][it->yCoordinate-1]=='.' && it->yCoordinate>0){
                it->yCoordinate--;
                this->numberOfNodes++;
                s.updateRepresentation();
                s.backNumber = storeNode.nodeNumber;
                stack.push(s);
                s = storeNode;
            }
            if(s.representation[it->xCoordinate][it->yCoordinate+it->length]=='.' && it->yCoordinate+it->length<=5){
                if(it->blockNumber==0){
                    // for determining if there is a path between the first block and the exit gate
                    for(int i=1; i<6; i++){
                        if(s.representation[it->xCoordinate][it->yCoordinate+it->length+i]=='E'){
                            s.terminate=true;
                            this->numberOfNodes++;
                            return s;
                        }
                        else if (s.representation[it->xCoordinate][it->yCoordinate+it->length+i]=='.'){
                            continue;
                        }
                        else{
                            break;
                        }
                    }
                }
                this->numberOfNodes++;
                it->yCoordinate++;
                s.backNumber = storeNode.nodeNumber;
                stack.push(s);
                s = storeNode;
            }
        }
        else if (it->direction=='v'){
            if(s.representation[it->xCoordinate+1][it->yCoordinate]=='.' && it->xCoordinate<5){
                it->xCoordinate++;
                this->numberOfNodes++;
                s.updateRepresentation();
                s.backNumber = storeNode.nodeNumber;
                stack.push(s);
                s = storeNode;
            }
            if(s.representation[it->xCoordinate-it->length][it->yCoordinate]=='.' && it->xCoordinate-it->length>=0){
                it->xCoordinate--;
                this->numberOfNodes++;
                s.updateRepresentation();
                s.backNumber = storeNode.nodeNumber;
                stack.push(s);
                s = storeNode;
            }
        }
    }
    return s;
}

void Graph::bfs(Node s){
    vector<Node>discovered;
    // First node will be added manually
    list<Node>queue;
    this->numberOfNodes++;
    queue.push_back(s);
    while(!queue.empty()){
        s = queue.front();
        if(!checkIfNodeWasDiscovered(discovered, s)){
            discovered.push_back(s);
            s.nodeNumber = (int)nodes.size();
            addNodes(s);
            // Node is added to the graph
            Node p = pushNodesForQueue(queue, s);
            // Adjacent nodes are pushed and a node is returned if for terminating the search
            if(p.terminate){
                createOutput(p);
                cout << "(BFS) Maximum number of nodes can be kept in memory is: " << numberOfNodes << endl;
                return;
            }
        }
        queue.pop_front();
    }
}

void Graph::dfs(Node u){
    vector<Node>explored;
    // First node will be added manually
    stack<Node>S;
    this->numberOfNodes++;
    S.push(u);
    while(!S.empty()){
        u = S.top();
        S.pop();
        if(!checkIfNodeWasDiscovered(explored, u)){
            explored.push_back(u);
            u.nodeNumber = (int)nodes.size();
            addNodes(u);
            // Node is added to the graph
            Node terminateNode = pushNodesForStack(S, u);
            // Adjacent nodes are pushed and a node is returned if for terminating the search
            if(terminateNode.terminate){
                // Print the solution and write to output file
                createOutput(terminateNode);
                cout << "(DFS) Maximum number of nodes can be kept in memory is: " << numberOfNodes << endl;
                return;
            }
        }
        
    }
}



void Graph::createOutput(Node s){
    ofstream outputFile(outputFileName.c_str());
    vector<Node>resultArray;
    while(s.backNumber>=0){
        resultArray.push_back(s);
        s = nodes[s.backNumber];
    }
    resultArray.push_back(s); // take the last node (starting environment node)
    for(vector<Node>::reverse_iterator it = resultArray.rbegin(); it!=resultArray.rend(); it++){
        it->represent();
        for (vector<Block>::iterator i=it->environment.begin(); i!=it->environment.end(); i++) {
            outputFile << i->xCoordinate <<" " << i->yCoordinate << " " << i->length <<" " << i->direction << endl;
        }
        outputFile << endl;
    }
    cout << "Number of nodes created for " << algorithm << " is: " << resultArray.size() << endl;
}

// Used for determining the size of arrays at first
long long calculateMaxNumberOfPossibleNodes(vector<Block> p){
    long long counter=1;
    for(vector<Block>::iterator it=p.begin(); it!=p.end(); it++)
        counter = counter*(7-it->length);
    return counter;
}




int main(int argc, const char * argv[]) {
    ifstream inputFile(argv[2]);
    outputFileName = argv[3];
    clock_t runTime;
    algorithm = argv[1];
    int input1, input2, input3;
    char input4;
    int i=0;
    Node firstNode;
    if(!inputFile){
        cout << "Input File Error!!!" << endl;
        cout << "Program terminated" << endl;
        return -1;
    }
    while(!inputFile.eof()){
        inputFile >> input1 >> input2 >> input3 >> input4;
        Block newBlock(input1, input2, input3, input4, i++);
        firstNode.addBlock(newBlock);
        if(inputFile.eof())
            firstNode.popBlock();
    }
    maxNumber = calculateMaxNumberOfPossibleNodes(firstNode.environment);
    Graph rushour;
    firstNode.updateRepresentation();   // after blocks are added from the input file
    if(strcmp(argv[1], "bfs")==0){
        runTime = clock();
        rushour.bfs(firstNode);
        runTime = clock() - runTime;
        cout << "Run Time for BFS is: " << (float)runTime / CLOCKS_PER_SEC << " seconds" << endl;
    }
    else if(strcmp(argv[1], "dfs")==0){
        runTime = clock();
        rushour.dfs(firstNode);
        runTime = clock() - runTime;
        cout << "Run Time for DFS is: " << (float)runTime / CLOCKS_PER_SEC << " seconds" << endl;
    }
    return 0;
}



