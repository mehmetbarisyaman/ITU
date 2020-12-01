//
//  box.h
//  oophw
//
//  Created by Baris on 4/29/16.
//  Copyright © 2016 Baris. All rights reserved.
//weight, length, width, maximumAllowedWeight



#ifndef box_h
#define box_h

#include "book.h"
#include "toy.h"

using namespace std;

template <class Type>
class Box: public base{
    int index;
    Type *objects_array;
    float maximumAllowedWeight;
    float totalWeight;
public:
    inline Box(float=0, float=0, float=0, float=0);
    ~Box();
    inline Type &operator[] (int);
    void add(Type);
    inline void add(Type*, int);
    inline int getIndex(){
        return index;
    }
    inline float getTotalWeight(){
        return totalWeight;
    }
    inline float getMaximumAllowedWeight(){
        return maximumAllowedWeight;
    }
    inline Type* getObjects_array(){
        return objects_array;
    }
    inline void setTotalWeight(float totalWeight_in){
        totalWeight = totalWeight_in;
    }
    inline void compare(char x){
        return;
    }
};


#endif

