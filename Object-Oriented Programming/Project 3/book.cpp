/*
 * @Author
 * Student Name: MEHMET BARIŞ YAMAN
 * Student ID : 150130136
 * Date: 06/05/2016
 */

#include "book.h"
#include <stdio.h>


base::base(string label_in, float weight_in, float length_in, float width_in){
    this->label = label_in;
    this->weight = weight_in;
    this->length = length_in;
    this->width = width_in;
    this->totalWeight = weight_in;
}


Book::Book(string label_in, float weight_in, float length_in, float width_in):base(label_in, weight_in, length_in, width_in){
    
}