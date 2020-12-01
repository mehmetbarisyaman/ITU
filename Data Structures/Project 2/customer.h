//
//  Header.h
//  DATAHW2
//
//  Created by Baris on 10/21/15.
//  Copyright © 2015 Baris. All rights reserved.
//
#define NAME_LENGTH 30
#define TRANS_LENGTH 12


struct trans{
    char *perform = new char[TRANS_LENGTH];
    trans *next;
};

struct customer{
    char *name = new char[NAME_LENGTH];
    char *surname = new char[NAME_LENGTH];
    int *number = new int;
    char *type = new char;
    trans *transactions;
    customer *next;
};


struct File_Record{
    char choice;
    char *name = new char[NAME_LENGTH];
    char *surname = new char[NAME_LENGTH];
    int *number = new int;
    char *type = new char;
    char *perform1 = new char[TRANS_LENGTH];
    char *perform2 = new char[TRANS_LENGTH];
    char *perform3 = new char[TRANS_LENGTH];
};