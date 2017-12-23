/*
 * @Author
 * Student Name: MEHMET BARIŞ YAMAN
 * Student ID : 150130136
 * Date: 06/05/2016
 */



#include <iostream>
#include "box.h"


template<class Type>
Box<Type>::Box(float weight_in, float length_in, float width_in, float maximumAllowedWeight_in):base("", weight_in, length_in, width_in){
    this->index = 0;
    this->maximumAllowedWeight = maximumAllowedWeight_in;
    objects_array=NULL;
};


template<class Type>
Type& Box<Type>::operator[](int i){
    return objects_array[i];
}



template <class Type>
void Box<Type>::add(Type tool){
    
    // Loading the variables
    float tool_width = tool.getWidth();
    float tool_length = tool.getLength();
    float box_width = getWidth();
    float box_length = getLength();
    float tool_weight = tool.getTotalWeight();
    float box_maximumAllowedWeight = getMaximumAllowedWeight();
    float box_totalWeight = getTotalWeight();
    
    // If the dimensions of the book fits
    if((box_width>= tool_width && box_length >=tool_length) || (box_length>= tool_width && box_width>=tool_length)){
        
        // If the total weight with the additional weight is not greater than the maximum allowed weight
        
        if(box_totalWeight + tool_weight < box_maximumAllowedWeight){
            
            // Adding operation should increase the array size dynamically
            if(objects_array){
                Type *temp = new Type[index+1];
                for(int i=0; i<index; i++)
                    temp[i] = objects_array[i];
                objects_array = new Type[index+1];
                for(int i=0; i<index; i++)
                    objects_array[i] = temp[i];
                delete []temp;
            }
            else    // if we add the first object
                objects_array = new Type;
            
            objects_array[index++]= tool;
            setTotalWeight(box_totalWeight + tool_weight);
        }
        else{
            throw string("  The total weight of the contained objects including the box may not exceed the maximum allowed weight for the box!");
            return;
        }
    }
    else{
        throw string("  The dimensions of the contained object should be equal or smaller than those of the box!");
        return;
    }
}

// Adding multiple elements
template <class Type>
void Box<Type>::add(Type* abc, int counter){
    Type temp;
    for(int i=0; i<counter; i++){
        temp = abc[i];
        add(temp);
    }
}

inline ostream& operator << (ostream& out, Book &z){
    out << z.getLabel()<< " # " << z.getLength() << "x" <<z.getWidth() << " " << z.getWeight() << "kg ";
    out << endl;
    return out;
}

inline ostream& operator << (ostream& out, Toy &z){
    int x=z.getWeight();
    if(x==0)
        throw string("Index out of bounds!");
    else
        out << "Toy Label: " << z.getLabel()<< " # " << z.getLength() << "x" <<z.getWidth() << " " << z.getWeight() << "kg ";
    if(z.getContainsBattery())
        out << "Contains Battery";
    else
        out << "No Battery";
    out << endl;
    return out;
}


inline ostream& operator <<(ostream& out, Box<Toy> &z)
{
    Toy* array = z.getObjects_array();
    out << "***** Box<Toy> *****" << endl << "Box item count: " << z.getIndex()<< endl << "Size: " << z.getLength() << "x" << z.getWidth() << " Box Weight:" << z.getWeight()<< "kg " << " Total/Maximum Allowed Weight:" << z.getTotalWeight()<< "kg" << "/" << z.getMaximumAllowedWeight() << "kg "<< endl << "Items:" << endl;
    for(int i=0; i<z.getIndex(); i++)
        out << i+1  << ":" << array[i];
    out << "********************" << endl;
    return out;
};


inline ostream& operator <<(ostream& out, Box<Book> &z)
{
    Book* array = z.getObjects_array();
    out << "***** Box<Book> *****" << endl << "Box item count: " << z.getIndex() << endl << "Size: " <<    z.getLength() << "x" << z.getWidth() << " Box Weight:" << z.getWeight()<< "kg " << " Total/Maximum Allowed Weight:" << z.getTotalWeight()<< "kg" << "/" << z.getMaximumAllowedWeight() << "kg" << endl << "Items:" << endl;
    for(int i=0; i<z.getIndex(); i++)
        out << i+1 << ":" << "Book Label:"<< array[i];
    out << "********************" << endl;
    return out;
};

inline ostream& operator <<(ostream& out, Box<Box<Toy> > &z)  
{
    Box<Toy> *x = z.getObjects_array();
    out << "***** Box<Box<Toy>> *****" << endl << "Box item count: " << z.getIndex() << endl << "Size: " <<    z.getLength() << "x" << z.getWidth() << " Box Weight:" << z.getWeight()<< "kg " << " Total/Maximum Allowed Weight:" << z.getTotalWeight()<< "kg" << "/" << z.getMaximumAllowedWeight()<< "kg " << endl << "Items:" << endl;
    for(int i=0; i<z.getIndex(); i++)
        out << i+1 << ":" << x[i];
    out << "********************" << endl;
    return out;
};


// Removing all allocated space
template<class Type>
Box<Type>::~Box(){
    Type *temp = new Type[index+1];
    for(int i=0; i<index; i++)
        temp[i] = objects_array[i];
    delete [] temp;
}
