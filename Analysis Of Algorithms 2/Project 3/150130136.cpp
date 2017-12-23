/*
 Student Name: Mehmet Barış Yaman
 Student ID: 150130136
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>           // for the "infinite" value

using namespace std;

class Publication{
public:
    string type;
    string definition;      // full name of a publication such as "WorkProc1"
    int reviewerLeft;       // will be the value of edge to Sink node
    Publication(string);
    vector<string>reviewers;
};

// Type of a publication and the value of the edge are determined
Publication::Publication(string pubType){
    this->definition = pubType;
    if(pubType[0]=='J'){
        this->type = "journal";
        this->reviewerLeft = 2;
    }
    else if(pubType[0]=='W'){
        this->type = "workshop";
        this->reviewerLeft = 2;
    }
    else if(pubType[0]=='C'){
        this->type = "conference";
        this->reviewerLeft = 3;
    }
    else{
        cerr << "Invalid publication Type!" << endl;
        return;
    }
}


// Class members duty is same as the class "Publication"
class Reviewer{
public:
    string definition;
    string title;
    int timeLeft;
    Reviewer(string, int);
};

// Title of the reviewer is determined
Reviewer::Reviewer(string titType, int time){
    this->definition = titType;
    this->timeLeft =time;
    if(titType[0]=='P')
        this->title = "professor";
    else if(titType[3]=='P')
        this->title = "assistant professor";
    else if(titType[0]=='A')
        this->title = "research assistant";
    else{
        cerr << "Invalid title!" << endl;
        return;
    }
}


// Whole network
class PaperNetwork{
public:
    int numberOfNodes;
    int numberOfReviewers;
    int numberOfPublications;
    int **matrix;                       // adjacency matrix
    vector<Reviewer>reviewerVector;
    vector<Publication>publicationVector;
    void readFile(string, string);      // to form the graph
    void formGraph();
    ~PaperNetwork();
    bool bfs(int *);          // Breadth-First Search
    void adjustFlow();            // Ford-Fulkerson Algorithm
    void augment(int , int, int);
    void writeResults();
};


void PaperNetwork::readFile(string input1, string input2){
    ifstream inputReviewer(input1.c_str());         // Reviewer file
    ifstream inputPublication(input2.c_str());      // Publication file
    inputReviewer >> numberOfReviewers;
    inputPublication >> numberOfPublications;
    // Also one for Source and one for Sink node
    numberOfNodes = numberOfPublications + numberOfReviewers + 2;
    // Allocating space for the adjacency matrix
    matrix = new int*[numberOfNodes];
    for(int x=0; x<numberOfNodes; x++)
        matrix[x] = new int[numberOfNodes];
    // Initializing the adjacency matrix
    for(int r=0; r<numberOfNodes; r++)
        for(int c=0; c<numberOfNodes; c++)
            matrix[r][c] = 0;
    // will be used for reading the file
    string in1;
    int in2;
    // Adding reviewers to the review vector
    for(int r=0; r<numberOfReviewers; r++){
        inputReviewer >> in1 >> in2;
        Reviewer newReviewer(in1, in2);         // type of the reviewer is determined by constructor
        reviewerVector.push_back(newReviewer);
    }
    // Adding publications to the publication vector
    for (int r=0; r<numberOfPublications; r++) {
        inputPublication >> in1;
        Publication newPublication(in1);        // type of the publication is determined by constructor
        publicationVector.push_back(newPublication);
    }
    inputReviewer.close();
    inputPublication.close();
}

void PaperNetwork::formGraph(){
    /*Forming the edges from reviewer nodes to publication nodes
     All the edge values between reviewer nodes and publication nodes is 1 since all reviewers can review only one publication in a single time slot*/
    for(int i=0; i<numberOfPublications; i++){
        Publication aPub = publicationVector[i];
        if(aPub.type=="journal"){
            for(int j=0; j<numberOfReviewers; j++){
                Reviewer aRev = reviewerVector[j];
                // Journal can be only reviewed by professors
                if(aRev.title == "professor")
                    matrix[j+1][i+numberOfReviewers+1] = 1;
            }
        }
        else if(aPub.type == "conference"){
            for(int j=0; j<numberOfReviewers; j++){
                Reviewer aRev = reviewerVector[j];
                // Conference proceeding can be reviewed by professors and assistant professors
                if(aRev.title == "professor" || aRev.title == "assistant professor")
                    matrix[j+1][i+numberOfReviewers+1] = 1;
            }
        }
        else{
            for(int j=0; j<numberOfReviewers; j++){
                // All reviewers can review workshop proceeding
                Reviewer aRev = reviewerVector[j];
                matrix[j+1][i+numberOfReviewers+1] = 1;
            }
        }
    }
    // Forming the edges from Source node to reviewer nodes
    for(int i=0; i<numberOfReviewers; i++)
        matrix[0][i+1] = reviewerVector[i].timeLeft;
    // Forming the edges from publication nodes to Sink node
    for(int i=0; i<numberOfPublications; i++)
        matrix[i+numberOfReviewers+1][numberOfNodes -1] = publicationVector[i].reviewerLeft;
}

bool PaperNetwork::bfs(int *traverseList){
    queue<int>que;
    int s;
    // Maintain the Discovered array
    int *discovered = new int[numberOfNodes];
    // Mark other nodes as unvisited
    for (int i=0; i<numberOfNodes; i++) {
        discovered[i] = -5;
    }
    // Initialize the current node as first node
    s = 0;
    // Initialize the first layer
    que.push(s);
    discovered[s] = -1;             // means no other node is added before s
    while(!que.empty()){
        s = que.front();
        que.pop();
        for(int i=0; i<numberOfNodes; i++){
            if(discovered[i] == -5 && matrix[s][i]>0){
                que.push(i);
                // set discovered index true
                discovered[i] = s;
            }
        }
    }
    // if any path is found
    if(discovered[numberOfNodes-1]!=-5){
        for(int i=0; i<numberOfNodes; i++)
            traverseList[i] = discovered[i];
        delete [] discovered;
        return true;
    }
    // If no path is found
    else{
        delete[] discovered;
        return false;
    }
}

void PaperNetwork::augment(int r, int c, int f){
    matrix[r][c]-=f;
}

// Ford-Fulkerson Implementation
void PaperNetwork::adjustFlow(){
    int *traverseList;
    traverseList = new int[numberOfNodes];
    int s;              // current node
    int parent;
    int publicationIndex=0;
    while(bfs(traverseList)){
        s = numberOfNodes -1;       // initialize s as the last node
        while(s!=0){
            // If the selected path node is a Reviewer
            if(s>0 && s <= numberOfReviewers){
                reviewerVector[s-1].timeLeft--;
                publicationVector[publicationIndex].reviewers.push_back(reviewerVector[s-1].definition);
            }
            // If the selected path node is a Publication
            else if(s>numberOfReviewers && s < numberOfNodes-1){
                publicationVector[s - numberOfReviewers -1].reviewerLeft--;
                publicationIndex = s - numberOfReviewers -1;
            }
            parent = traverseList[s];
            s = traverseList[s];
        }
        s = numberOfNodes -1;
        // The edge between a publication and a reviewer node is always 1. Therefore
        // there is no need to calculate the flow since it will always be equal to 1
        while (s!=0) {
            parent = traverseList[s];
            augment(parent, s, 1);          // decrease the edge, 1 is the flow
            s = parent;                     // traverse the parent node
        }
    }
    delete [] traverseList;

}

void PaperNetwork::writeResults(){
    ofstream output("output.txt");
    for(int i=0; i<numberOfPublications; i++){
        output << publicationVector[i].definition << " is assigned to ";
        for(int j=0; j<publicationVector[i].reviewers.size(); j++){
            output << publicationVector[i].reviewers[j];
            if(j<publicationVector[i].reviewers.size()-1)
                output << ", ";
        }
        output << endl;
    }
    bool feasible = true;
    // If any Publication node has any review
    for(int i=0; i<publicationVector.size(); i++){
        if(publicationVector[i].reviewerLeft>0){
            feasible = false;
        }
        else if (publicationVector[i].reviewers.size()==0){
            output << publicationVector[i].definition << " is NOT assigned." << endl;
        }
    }
    if(feasible){
        output << "FEASIBLE ASSIGNMENT IS FOUND FOR ALL NODES!" << endl;
    }
    else{
        output << "ASSIGNMENT IS NOT FEASIBLE!" << endl;
    }
    output.close();
    
}

PaperNetwork::~PaperNetwork(){
    for(int i=0; i<numberOfNodes; i++){
        // Clear node list first
        delete []matrix[i];
    }
    // Clear the node array
    delete [] matrix;
}

int main(int argc, const char * argv[]) {
    PaperNetwork reviewNetwork;
    reviewNetwork.readFile(argv[1], argv[2]);
    reviewNetwork.formGraph();
    reviewNetwork.adjustFlow();
    reviewNetwork.writeResults();
    return 0;
}
