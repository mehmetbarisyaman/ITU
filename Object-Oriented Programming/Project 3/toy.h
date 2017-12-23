/*
 * @Author
 * Student Name: MEHMET BARIŞ YAMAN
 * Student ID : 150130136
 * Date: 06/05/2016
 */

#ifndef toy_h
#define toy_h

#include "book.h"
#include <string>


using namespace std;

class Toy: public base{
    bool containsBattery;
public:
    
    // We need to use inline functions in order to prevent the linker error
    
    Toy(string="", float=0, float=0, float=0, bool=false);
    inline bool getContainsBattery(){
        return containsBattery;
    }
    inline void setContainsBattery(bool containsBattery_in){
        this->containsBattery = containsBattery_in;
    }
    
    // To provide that this class is not abstract
    void compare(char x){
        return;
    }
};




#endif
