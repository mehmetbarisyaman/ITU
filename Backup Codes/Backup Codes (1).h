//
//  book.h
//  oophw
//
//  Created by Baris on 4/29/16.
//  Copyright © 2016 Baris. All rights reserved.
//

#ifndef book_h
#define book_h

using namespace std;

class base{
protected:
    string label;
    float weight;
    float length;
    float width;
public:
    virtual void compare(char) = 0;
    inline base(string="", float=0, float=0, float=0);
    inline string getLabel(){
        return this->label;
    }
    inline void setLabel(string label_in){
        this->label = label_in;
    }
    inline float getWeight(){
        return this->weight;
    }
    inline void setWeight(float weight_in){
        weight = weight_in;
    }
    inline void setLength(float length_in){
        length = length_in;
    }
    inline float getLength(){
        return this->length;
    }
    inline void setWidth(float width_in){
        width = width_in;
    }
    inline float getWidth(){
        return this->width;
    }
};

base::base(string label_in, float weight_in, float length_in, float width_in){
    this->label = label_in;
    this->weight = weight_in;
    this->length = length_in;
    this->width = width_in;
}


class Book: public base{

public:
    inline Book(string="", float=0, float=0, float=0);
    void compare(char x){
        return;
    }
};


Book::Book(string label_in, float weight_in, float length_in, float width_in):base(label_in, weight_in, length_in, width_in){
    
}



#endif