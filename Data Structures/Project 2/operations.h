//
//  operations.h
//  DATAHW2
//
//  Created by Baris on 10/21/15.
//  Copyright © 2015 Baris. All rights reserved.
//

#include "customer.h"
struct operations{
    customer *head;
    int fileindex;
    FILE *input;
    FILE *output;
    void create();
    void close();
    void insert(customer *);
    customer *create_customer(File_Record* );
    void search(char *, char *);
    void proceed();
    void remove(File_Record *);
    void print();
    void writetofile(customer *);
    void makeEmpty();
    bool printyazimi;
    File_Record readfromfile();
    int order;
};