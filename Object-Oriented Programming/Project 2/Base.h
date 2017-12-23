/*
 NAME: MEHMET BARIŞ YAMAN
 NUMBER: 150130136
 DATE: 18/04/2016
 */

class base{                     // Abstract Class
    friend class HetList;       // As in the example from the lecture HetList is a friend of Base class
protected:
    base *next;                 // We make a list with next pointers
    
    int result;                 // The score of each card by comparing an another card (2, 1, 0)
    
    char card;                  // The character obtained from the input file
public:
    
    virtual void compare(char) = 0;     // Virtual Member Function
    
};

// Each kind of cards return different scores comparing with each other
// Thats why each card class have different compare functions
// We should override the compare function from the Base Class

class red: public base {
public:
    void compare(char);
};

class dark_red: public red {        // Derived from Red Class
public:
    void compare(char x);
};

class green: public base {
public:
    void compare(char x);
};

class blue: public base {
public:
    void compare(char x);
};

// Class diagram is provided by introducing the card classes

void red::compare(char x){   // Comparing a red card with another kind of cards
    if(x=='B' || x=='D')
        this->result = 0;
    else if( x=='R')
        this->result = 1;
    else if( x== 'G')
        this->result = 2;
}

void dark_red::compare(char x){  // Comparing a dark red card with another kind of cards
    if(x=='B' || x=='R')
        this->result = 2;
    else if( x=='D')
        this->result = 1;
    else if( x== 'G')
        this->result = 0;
}

void green::compare(char x){     // Comparing a green card with another kind of cards
    if(x=='B' || x=='D')
        this->result = 2;
    else if( x=='G')
        this->result = 1;
    else if( x== 'R')
        this->result = 0;
}

void blue::compare(char x){      // Comparing a blue card with another kind of cards
    if(x=='G' || x=='D')
        this->result = 0;
    else if( x=='B')
        this->result = 1;
    else if( x== 'R')
        this->result = 2;
}

