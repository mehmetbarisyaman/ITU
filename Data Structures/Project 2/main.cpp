//
//  main.cpp
//  DATAHW2
//
//  Created by Baris on 10/21/15.
//  Copyright © 2015 Baris. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <ctype.h>
#include "operations.h"

typedef operations defter;
defter book;

using namespace std;

void add_customer(File_Record *);
void print_customer();
void search_customer(File_Record *);
void procees_transactions();
bool perform_operation(File_Record *);
void remove_customer(File_Record *);

int main() {
    book.create();
    bool end = false;
    File_Record record;
    book.fileindex=0;
    while(!end){
        record = book.readfromfile();
        end = perform_operation(&record);
    }
    book.close();
    return EXIT_SUCCESS;
}

bool perform_operation(File_Record *record){
    bool terminate=false;
    switch (record->choice) {
        case 'P': case'p':
            print_customer();
            break;
        case 'S': case 's':
            search_customer(record);
            break;
        case 'A': case 'a':
            add_customer(record);
            break;
        case 'V': case 'v':
            procees_transactions();
            break;
        case 'R': case 'r':
            remove_customer(record);
            break;
        case 'E': case 'e':
            terminate=true;
            break;
        default:
            break;
    }
    return terminate;
}

void print_customer(){
    book.print();
}

void search_customer(File_Record *record){
    book.search(record->name, record->surname);
}

void add_customer(File_Record *record){
    customer *pnode;
    pnode = book.create_customer(record);
    book.insert(pnode);
}

void procees_transactions(){
    book.proceed();
}

void remove_customer(File_Record *record){
    book.remove(record);
}


