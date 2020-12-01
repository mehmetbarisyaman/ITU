/*
Name: Mehmet Barış Yaman
Number: 150130136
*/

#include <iostream>
#include <iomanip>
#include "InstructorList.h"

using namespace std;

//Addressbook Class

class Addressbook{
private:
    InstructorList *instructor_array;
    InstructorList tempList;
    
public:
    void print_menu();
    bool perform_operation(int);
    void add_operation();
    Addressbook();
    ~Addressbook();
};


Addressbook::Addressbook(){
    instructor_array = new InstructorList;
}

void Addressbook::add_operation(){
    char arg1[MAXTITLE];
    char arg2[MAXFIRSTNAME];
    char arg3[MAXLASTNAME];
    char arg4[MAXTELEPHONENUMBER];
    char arg5[MAXROOMNUMBER];
    char arg6[MAXUSERNAME];
    char arg7[MAXEMAILADDRESS];
    char arg8[MAXCOURSENAME];
    int how_many_courses;
    cout << "Enter the title: " << endl;
    cin.ignore(1000, '\n');
    cin.getline(arg1, MAXTITLE);
    cout << "Enter the First Name: " << endl;
    cin >> setw(MAXFIRSTNAME) >> arg2;
    for(int n = 0; n<= instructor_array->array_size; n++){
        if(instructor_array->ogretmenler[n].first_name== arg2){
            cout << "Same Instructions can not be added" << endl;
            return;
        }
    }
    cout << "Enter the Last Name: " << endl;
    cin >> setw(MAXLASTNAME) >> arg3;
    cout << "Enter the Telephone Number: " << endl;
    cin >> setw(MAXTELEPHONENUMBER) >> arg4;
    cout << "Enter the Room Number: " << endl;
    cin >> setw(MAXROOMNUMBER) >> arg5;
    cout << "Enter the User Name: " << endl;
    cin >> setw(MAXUSERNAME) >> arg6;
    cout << "Enter the E-Mail: " << endl;
    cin >> setw(MAXEMAILADDRESS) >> arg7;
    cout << "How many courses are given by the instructor? :";
    cin >> how_many_courses;
    string new_instructor_courses[MAXCOURSES];
    if(how_many_courses!=0)
        cout << "Enter the Courses: " << endl;
    for(int x = -1; x<how_many_courses; x++){
        cin.getline(arg8, MAXCOURSENAME);
        new_instructor_courses[x] = arg8;
    }
    Instructor *newinstructor = new Instructor(arg1, arg2, arg3, arg4, arg5, arg6, arg7, how_many_courses, new_instructor_courses);
    int counter = -5;
    for(int x=0; x<instructor_array->array_size; x++)
        if(instructor_array->ogretmenler[x].first_name == ""){
            counter = x;
            break;
        }
    if(counter!=-5)
        instructor_array->ogretmenler[counter]=* newinstructor;
    else{
        tempList =  *this->instructor_array;
        *this->instructor_array = tempList;
        for(int x=0; x<instructor_array->array_size; x++)
            if(instructor_array->ogretmenler[x].first_name == ""){
                counter = x;
                break;
            }
        instructor_array->ogretmenler[counter]=* newinstructor;
    }
    cout << "New instructor is added!" << endl;

}

void Addressbook::print_menu(){
    cout << "1. Add a new instructor" << endl;
    cout << "2. Delete an instructor" << endl;
    cout << "3. List all instructors" << endl;
    cout << "4. Search by Name" << endl;
    cout << "5. Search by Surname" << endl;
    cout << "6. Search by Telephone Number" << endl;
    cout << "7. Search by Room Number" << endl;
    cout << "8. Exit" << endl;
    cout << "Enter the number for operation you want to perform: ";
}


bool Addressbook::perform_operation(int secenek){
    char durdur;
    switch(secenek){
        case 1:
            add_operation();
            break;
        case 2:
            instructor_array->delete_instructor();
            break;
        case 3:
            instructor_array->list_instructors();
            break;
        case 4:
            instructor_array->search_firstname();
            break;
        case 5:
            instructor_array->search_lastname();
            break;
        case 6:
            instructor_array->search_telephonenumber();
            break;
        case 7:
            instructor_array->search_roomnumber();
            break;
        default:
            cout << "Invalid option" << endl;
            break;
    }
    cout << "Do you want to perform another operation?(Y/N): ";
    cin >> durdur;
    if(durdur=='y'||durdur=='Y')
        return false;
    else if(durdur=='N'||durdur=='n')
        return true;
    else{
        cout << "Invalid option, termination stops" << endl;
        return true;
    }
}

Addressbook::~Addressbook(){
    delete instructor_array;
}

int main(){
    bool terminate= false;
    Addressbook defter;
    int choice;
    
    while(terminate!=true){
        defter.print_menu();
        cin >> choice;
        if(choice == 8)
            terminate = true;
        else
            terminate = defter.perform_operation(choice);
    }
    return 0;
}