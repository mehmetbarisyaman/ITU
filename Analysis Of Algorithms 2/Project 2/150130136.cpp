/*
 Student Name: Mehmet Barış Yaman
 Student ID: 150130136
 */


#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class node{
public:
    string data;                // For converting a word to a POS tag
    string traceBack;           // For handling the whole string
};


class Sentence{
public:
    vector<string>words;        // Including all the words in a sentence
    void addWord(string);
    void makeEmpty();
};

void Sentence::addWord(string inputWord){
    words.push_back(inputWord);
}
void Sentence::makeEmpty(){
    this->words.clear();
}

// Converting a word to a POS tag
string posTagOfWord(string word){
    string result;
    // Sentences begin with a capital letter, therefore that should also be considered
    if(word=="that" || word=="this" || word=="a" || word=="the" || word=="That" || word=="This" || word=="A" || word=="The"){
        result = "DT";
    }
    else if(word=="book"|| word=="flight" || word=="cat" || word=="mat" || word=="I" || word=="you" || word=="they" || word=="Book"|| word=="Flight" || word=="Cat" || word=="Mat" || word=="i" || word=="You" || word=="They"){
        result = "NN";
    }
    else if(word=="booked" || word=="included" || word=="preferred" || word=="sat" || word=="Booked" || word=="Included" || word=="Preferred" || word=="Sat"){
        result = "VR";
    }
    else if(word=="from" || word=="to" || word=="on" || word=="near" || word=="through"||word=="From" || word=="To" || word=="On" || word=="Near" || word=="Through"){
        result = "PR";
    }
    else if(word=="big" || word=="heavy" || word=="beautiful" || word=="cheap"||word=="Big" || word=="Heavy" || word=="Beautiful" || word=="Cheap"){
        result = "AD";
    }
    else{
        result = "wrong syntax";
    }
    return result;
}

// For tag set transformation
string tagSetTransformation(string arg1, string arg2){
    string resultTagSet;
    if(arg1=="NN" && arg2=="NN"){
        resultTagSet = "NP";
    }
    else if(arg1=="AD" && arg2=="NN"){
        resultTagSet = "NP";
    }
    else if(arg1=="AD" && arg2=="NP"){
        resultTagSet = "NP";
    }
    else if(arg1=="DT" && arg2=="NP"){
        resultTagSet = "NP";
    }
    else if(arg1=="DT" && arg2=="NN"){
        resultTagSet = "NP";
    }
    else if(arg1=="PR" && arg2=="NN"){
        resultTagSet = "PP";
    }
    else if(arg1=="PR" && arg2=="NP"){
        resultTagSet = "PP";
    }
    else if(arg1=="VR" && arg2=="PP"){
        resultTagSet = "VP";
    }
    else if(arg1=="NN" && arg2=="VR"){
        resultTagSet = "VP";
    }
    else if(arg1=="NP" && arg2=="VR"){
        resultTagSet = "VP";
    }
    else if(arg1=="VR" && arg2=="NP"){
        resultTagSet = "VP";
    }
    else if(arg1=="NP" && arg2=="VP"){
        resultTagSet = "S";
    }
    else if(arg1=="NN" && arg2=="VP"){
        resultTagSet = "S";
    }
    else{
        resultTagSet = "no rule";
    }
    return resultTagSet;
}


class Parser{
public:
    string extractParseTree(vector<node>&, int);
    void obtainNodeVector(Sentence, vector<node>&);
};

// Converting a string vector into a node vector
void Parser::obtainNodeVector(Sentence s, vector<node>&ap){
    size_t sentenceSize = s.words.size();
    for(int i=0; i<sentenceSize;i++){
        node newNode;
        newNode.data = posTagOfWord(s.words[i]);
        newNode.traceBack = newNode.data + "(" + s.words[i] + ")";          // parsed data
        ap.push_back(newNode);
    }
}

string Parser::extractParseTree(vector<node>& s, int counter){
    // transform pos tag sets
    string resultPos = tagSetTransformation(s[counter-2].data, s[counter-1].data);
    if(resultPos!="no rule"){
        // If the rule is accepted, new node has to be created
        node parent;
        parent.data = resultPos;
        parent.traceBack = parent.data + "[" + s[counter-2].traceBack + s[counter-1].traceBack + "]";
        if(resultPos=="S"){
            if(s.size()==2)     // that means the sentence is wholly parsed
                return parent.traceBack;
            else{
                if(counter==2) // If counter gets below to 2, then segmentation fault occurs, since -1 indexes may be wanted to be accessed
                    return "SYNTAX ERROR";
                // That means more sentences
                return extractParseTree(s, counter-1);
            }
        }
        // Change the word vector accordingly
        // Pos tag transformation operands are deleted from the vector
        s[counter-2] = parent;
        s.erase(s.begin()+counter-1, s.begin()+counter);
        return extractParseTree(s, (int)s.size());
    }
    else{
        // No related pos tag transformation rule
        if(counter==2)
            return "SYNTAX ERROR";
        // Shifts left to look at the other pos tags
        return extractParseTree(s, counter-1);
    }
}


int main(int argc, const char * argv[]) {
    ifstream inputFile(argv[1]);
    ofstream outputFile("output.txt");
    string inputString;         // used in taking an input
    Parser parser;
    vector<Sentence>sentences;  // holds all the sentences in an input file
    Sentence inputSentence;
    while(!inputFile.eof()){
        inputFile >> inputString;
        // If the word begins with a capital letter that means new input sentence is started to be read
        if(isupper(inputString[0])&&inputSentence.words.size()){
            sentences.push_back(inputSentence);
            inputSentence.makeEmpty();          // get ready for the next sentence
        }
        inputSentence.addWord(inputString);
    }
    sentences.push_back(inputSentence);         // pushing the last sentence that is not included
    for(int i=0; i<sentences.size(); i++){
        vector<node>used;
        parser.obtainNodeVector(sentences[i], used);    // vector 'used' carries nodes (input sequences with POS tag set)
        string result = parser.extractParseTree(used, (int)sentences[i].words.size());
        cout << result << endl;                 // Printing the outputs
        outputFile << result << endl;           // Writing to the output file
    }
    return 0;
}
