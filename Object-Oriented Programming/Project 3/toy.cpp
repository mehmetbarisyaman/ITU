/*
 * @Author
 * Student Name: MEHMET BARIŞ YAMAN
 * Student ID : 150130136
 * Date: 06/05/2016
 */

#include <stdio.h>
#include "toy.h"

Toy::Toy(string label_in, float weight_in, float length_in, float width_in, bool containsBattery_in):base(label_in, weight_in, length_in, width_in){
    this->containsBattery = containsBattery_in;
}