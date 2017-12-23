/*
 * @Author
 * Student Name: MEHMET BARIŞ YAMAN
 * Student ID : 150130136
 * Date: 06/05/2016
 */


#ifndef box_h
#define box_h

#include "book.h"
#include "toy.h"

using namespace std;

template <class Type>
class Box: public base{
    int index;              // holds the index of the template objects array index
    Type *objects_array;    // for objects that a box include
    float maximumAllowedWeight;
    
public:
    Box(float=0, float=0, float=0, float=0);
    ~Box();
    Type &operator[] (int);     // []operator overlading is needed
    void add(Type);             // Adding a single objects to the array
    void add(Type*, int);       // Adding multiple objects to the array
    int getIndex(){
        return index;
    }
    float getMaximumAllowedWeight(){
        return maximumAllowedWeight;
    }
    Type* getObjects_array(){
        return objects_array;
    }
    // To provide that this class is not abstract
    void compare(char x){
        return;
    }
};


#endif

