/*
 * @Author
 * Student Name: MEHMET BARIŞ YAMAN
 * Student ID : 150130136
 * Date: 06/05/2016
 */

#ifndef book_h
#define book_h
#include <string>

using namespace std;

/*
 In order to avoid code repetition we need to create an additional base class 
 and include the same memebers and methods in all classes
 */

class base{
    
    // To provide maximum data hiding we need to define all variables as private
    string label;
    float weight;
    float length;
    float width;
    float totalWeight;
public:
    virtual void compare(char) = 0;     // base class is an abstract class
    
    // To prevent the linker error we need to provide that those methods are defined once
    
    // Thats why I have used inline methods here
    
    base(string="", float=0, float=0, float=0);
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
    inline void setTotalWeight(float totalWeight_in){
        totalWeight = totalWeight_in;
    }
    inline float getTotalWeight(){
        return totalWeight;
    }
};




class Book: public base{

public:
    Book(string="", float=0, float=0, float=0);
    
    // To provide that this class is not abstract
    void compare(char x){
        return;
    }
};






#endif