
#include <stdio.h>
#include "record.h"
struct File{
	FILE *phonebook;
	void create();
	void close();
	void add(Phone_Record *);
	int search(char[]);
	void remove(int );
	void update(int , Phone_Record *);
};