//
//  toy.h
//  oophw
//
//  Created by Baris on 4/29/16.
//  Copyright © 2016 Baris. All rights reserved.
//

#ifndef toy_h
#define toy_h

#include "book.h"
#include <string>


using namespace std;

class Toy: public base{
    bool containsBattery;
public:
    inline Toy(string="", float=0, float=0, float=0, bool=false);
    inline bool getContainsBattery(){
        return containsBattery;
    }
    inline void setContainsBattery(bool containsBattery_in){
        this->containsBattery = containsBattery_in;
    }
    inline void compare(char x){
        return;
    }
};


Toy::Toy(string label_in, float weight_in, float length_in, float width_in, bool containsBattery_in):base(label_in, weight_in, length_in, width_in){
    this->containsBattery = containsBattery_in;
}

#endif
