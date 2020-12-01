//
//  operations.cpp
//  DATAHW2
//
//  Created by Baris on 10/21/15.
//  Copyright © 2015 Baris. All rights reserved.
//

#include <stdio.h>
#include "operations.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;

void operations::insert(customer *peace){
    customer *traverse, *behind;
    traverse = head;
    behind = NULL;
    if(head == NULL){
        head = peace;
    }
    else if(strcmp(peace->type, head->type)>0){
        peace->next = head;
        head = peace;
    }
    else{
    while ( traverse && (strcmp(peace->type, traverse->type)<=0) ) {
        behind = traverse;
        traverse = traverse->next;
    }
    if(!traverse)
        behind->next = peace;
    else{
        peace->next = traverse;
        behind->next = peace;
    }}
    if(printyazimi ==true){
    fwrite("New customer is added\n\n", sizeof("New customer is added\n\n"), 1, output);
        order ++;
    }
}

void operations::remove(File_Record *record){
    customer *traverse=head, *behind=NULL;
    customer *xnode;
    trans *tr;
    while(traverse && !(strcmp(record->name, traverse->name)==0)){
        behind = traverse;
        traverse = traverse->next;
    }
    if(!traverse)
        fwrite("Record could not be found\n\n", sizeof("Record could not be found\n\n"), 1, output);
    else{
        behind->next = traverse->next;
        tr = traverse->transactions;
        for(int x=1; x<=*traverse->number; x++) {
                traverse->transactions = tr->next;
                delete tr;
                tr = traverse->transactions;
        }
        delete traverse;
        record->type = NULL;
        record->number = NULL;
        xnode=create_customer(record);
        writetofile(xnode);
        order--;
        fwrite("is removed\n\n", sizeof("is removed\n\n"), 1, output);
    }
}

void operations::search(char *tosearchname, char *tosearchsurname){
    customer *traverse=head, *xnode=NULL;
    for(int x=1; x<=order; x++){
        if(strncmp(tosearchname, traverse->name, strlen(tosearchname))==0){
            if(strncmp(tosearchsurname, traverse->surname, strlen(tosearchsurname))==0){
                xnode = traverse;
            }
        }
        traverse = traverse->next;
    }
    if(!xnode){
        fwrite(tosearchname, sizeof(tosearchname), 1, output);
        fwrite(" ", 1, 1, output);
        fwrite(tosearchsurname, sizeof(tosearchsurname), 1, output);
        fwrite(" could not  be found", sizeof(" could not be found"), 1, output);
        fwrite("\n\n", sizeof("\n\n"), 1, output);
    }
    else{
        writetofile(xnode);
        fwrite("\n\n", sizeof("\n\n"), 1, output);
    }
}

customer *operations::create_customer(File_Record *nrptr){
    customer *newcustomer;
    trans *tr;
    newcustomer = new customer;
    tr = new trans;
    strcpy(newcustomer->name, nrptr->name);
    strcpy(newcustomer->surname, nrptr->surname);
    newcustomer->number = nrptr->number;
    newcustomer->type = nrptr->type;
    newcustomer->transactions=tr;
    if(newcustomer->number){
        if(*newcustomer->number == 1){
            strcpy(tr->perform, nrptr->perform1);
        }
        else if(*newcustomer->number == 2){
            strcpy(tr->perform, nrptr->perform1);
            tr->next = new trans;
            tr = tr->next;
            strcpy(tr->perform, nrptr->perform2);
        }
        else if(*newcustomer->number == 3){
            strcpy(tr->perform, nrptr->perform1);
            tr->next = new trans;
            tr = tr->next;
            strcpy(tr->perform, nrptr->perform2);
            tr->next = new trans;
            tr = tr->next;
            strcpy(tr->perform, nrptr->perform3);
        }}
    return newcustomer;
}

void operations::create(){
    if ( !(input = fopen( "input_file.txt", "r+" ) ) ) {
        if ( !(input = fopen( "input_file.txt", "w+" ) ) ) {
            cerr << "File could not be opened." << endl;
            cerr << "Will work in memory only." << endl;
            return;
        } }
    if ( !(output = fopen( "output_file.txt", "w+" ) ) ) {
        if ( !(output = fopen( "output_file.txt", "r+" ) ) ) {
            cerr << "File could not be opened." << endl;
            cerr << "Will work in memory only." << endl;
            return;
        } }
    fseek(input, 0, SEEK_SET);
    fseek(output, 0, SEEK_SET);
    head = NULL;
    order = 0;
    printyazimi = true;
}

void operations::close(){
    makeEmpty();
    fwrite("Program exists", sizeof("Program exists"), 1, output);
    fclose(input);
    fclose(output);
}

void operations::print(){
    customer *traverse;
    traverse = head;
    for(int x=1; x<=order; x++){
        writetofile(traverse);
        fwrite("\n", sizeof("\n"), 1, output);
            traverse=traverse->next;
    }
    fwrite("\n", sizeof("\n"), 1, output);
}

void operations::proceed(){
    customer *traverse, *xtraverse;
    trans *tr;
    traverse = head;
    tr = traverse->transactions;
    if(*traverse->number == 1){
        head = head->next;
        delete traverse;
        order--;
    }
    else if(*traverse->number > 1){
        head->transactions = head->transactions->next;
        delete tr;
        (*head->number)--;
        xtraverse = head;
        head = head->next;
        printyazimi = false;
        insert(xtraverse);
    }
    fwrite("Next transaction is processed\n\n", sizeof("Next transaction is processed\n\n"), 1, output);
    printyazimi = true;
    }

void operations::writetofile(customer *traverse){
    trans *tr;
    File_Record *record;
    record = new File_Record;
    tr = traverse->transactions;
    record->choice = '\0';
    strcpy(record->name, traverse->name);
    strcpy(record->surname, traverse->surname);
    record->type = traverse->type;
    record->number = traverse->number;
    fwrite(record->name, sizeof(record->name), 1, output);
    fwrite(" ", 1, 1, output);
    fwrite(record->surname, sizeof(record->surname), 1, output);
    fwrite(" ", 1, 1, output);
    if(record->type){
        fwrite(record->type, sizeof(record->type),1, output);
        fwrite(" ", 1, 1, output);
    }
    if(record->number){
    if(*record->number == 0){
        return;
    }
    else if(*record->number == 1){
        strcpy(record->perform1, tr->perform);
        fwrite(record->perform1, sizeof(record->perform1), 1, output);
        fwrite(" ", 1, 1, output);
    }
    else if(*record->number == 2){
        strcpy(record->perform1, tr->perform);
        fwrite(record->perform1, sizeof(record->perform1), 1, output);
        fwrite(" ", 1, 1, output);
        if(tr->next){
            tr = tr->next;
            strcpy(record->perform2, tr->perform);
            fwrite(record->perform2, sizeof(record->perform2), 1, output);
            fwrite(" ", 1, 1, output);
        }
    }
    else if(*record->number == 3){
        strcpy(record->perform1, tr->perform);
        fwrite(record->perform1, sizeof(record->perform1), 1, output);
        fwrite(" ", 1, 1, output);
        if(tr->next){
            tr = tr->next;
            strcpy(record->perform2, tr->perform);
            fwrite(record->perform2, sizeof(record->perform2), 1, output);
            fwrite(" ", 1, 1, output);
        }
        if(tr->next){
            tr = tr->next;
            strcpy(record->perform3, tr->perform);
            fwrite(record->perform3, sizeof(record->perform3), 1, output);
        }
    }
    }
        delete record;
}


void operations::makeEmpty(){
    customer *p;
    trans *q;
    for(int x=1; x<=order; x++) {
        p = head;
        head = head->next;
        q = p->transactions;
        for(int x=1; x<=*p->number; x++){
            p->transactions = p->transactions->next;
            delete q;
            q = p->transactions;
        }
        delete p;
    }
}

File_Record operations::readfromfile(){
    fseek(input, fileindex, SEEK_SET);
    char access[90];
    int z=0;
    bool durdur;
    durdur = false;
    File_Record xrecord;
    fread(access, 90, 1, input);
    xrecord.choice = access[0];
    for(int x=2; x<=90 ; x++){
        if(access[x]==';' || access[x]==' '){
            z=x;
            break;
        }
        if(access[x] == '\n'){
            durdur = true;
            z=x;
            z--;
            break;
        }
        xrecord.name[x-2] = access[x];
    }
    if(durdur == false)
    for(int x=z+1; x<=90 ; x++){
        if(access[x]==';' || access[x]==' '){
            z=x;
            break;
        }
        if(access[x] == '\n'|| access[x] == '\r'){
            durdur = true;
            z=x;
            break;
        }
        xrecord.surname[x-z-1] = access[x];
    }
    if(durdur == false){
        *xrecord.type = access[z+1];
        if(access[z+2]=='\r'){
            durdur = true;
            z= z+2;
        }
    }
    if(durdur == false){
        *xrecord.number = atoi(&access[z+3]);
    }
    if(*xrecord.number == 1){
        for(int x=z+5; x<=90 ; x++){
            if(access[x]==';'||access[x] == '\r' || access[x]==' ' || access[x] == '\n'){
                z=x;
                break;
            }
            xrecord.perform1[x-z-5] = access[x];
        }
        strcpy(xrecord.perform2, "");
        strcpy(xrecord.perform3, "");
    }
    else if(*xrecord.number ==2){
        for(int x=z+5; x<=90 ; x++){
            if(access[x]==';'||access[x] == '\r' || access[x]==' ' || access[x] == '\n'){
                z=x;
                break;
            }
            xrecord.perform1[x-z-5] = access[x];
        }
        for(int x=z+1; x<=90 ; x++){
            if(access[x]==';'||access[x] == '\r' || access[x]==' ' || access[x] == '\n'){
                z=x;
                break;
            }
            xrecord.perform2[x-z-1] = access[x];
        }
        strcpy(xrecord.perform3, "");
    }
    else if(*xrecord.number ==3){
        for(int x=z+5; x<=90 ; x++){
            if(access[x]==';'||access[x] == '\r' || access[x]==' ' || access[x] == '\n'){
                z=x;
                break;
            }
            xrecord.perform1[x-z-5] = access[x];
        }
        for(int x=z+1; x<=90 ; x++){
            if(access[x]==';'||access[x] == '\r' || access[x]==' ' || access[x] == '\n'){
                z=x;
                break;
            }
            xrecord.perform2[x-z-1] = access[x];
        }
        for(int x=z+1; x<=90 ; x++){
            if(access[x]==';'||access[x] == '\r' || access[x]==' ' || access[x] == '\n'){
                z=x;
                break;
            }
            xrecord.perform3[x-z-1] = access[x];
        }
    }
    fileindex = fileindex + z+2;
    return xrecord;
}
