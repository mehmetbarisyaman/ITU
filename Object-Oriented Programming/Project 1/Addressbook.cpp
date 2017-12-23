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
    InstructorList tempList;        // Will be used for the assignment between to InstructorList class
    
public:
    
    void print_menu();
    bool perform_operation(int);     // For performing the menu operations
    void add_operation();   
    Addressbook();
    ~Addressbook();     // To release the dynamically allocated spaces
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
    cin.ignore(1000, '\n');     // To prevent the input crash with space characters
    cin.getline(arg1, MAXTITLE);
    cout << "Enter the First Name: " << endl;
    cin.getline(arg2, MAXFIRSTNAME);
    
    // Controlling whether same Instructors are added or not
    
    for(int n = 0; n< instructor_array->array_size; n++){
        if(instructor_array->ogretmenler[n].first_name== arg2){
            cout << "Same Instructions can not be added" << endl;
            return;
        }
    }
    
    // Taking the input from user
    
    cout << "Enter the Last Name: " << endl;
    cin.getline(arg3, MAXLASTNAME);
    cout << "Enter the Telephone Number: " << endl;
    cin.getline(arg4, MAXTELEPHONENUMBER);
    cout << "Enter the Room Number: " << endl;
    cin.getline(arg5, MAXROOMNUMBER);
    cout << "Enter the User Name: " << endl;
    cin.getline(arg6, MAXUSERNAME);
    cout << "Enter the E-Mail: " << endl;
    cin.getline(arg7, MAXEMAILADDRESS);
    cout << "How many courses are given by the instructor? :";
    cin >> how_many_courses;
    string new_instructor_courses[MAXCOURSES];
    
    if(how_many_courses!=0)     // If the Instructor does not give any course
        cout << "Enter the Courses: " << endl;
    
    cin.ignore(1000, '\n');
    for(int x = 0; x<how_many_courses; x++){
        cin.getline(arg8, MAXCOURSENAME);
        new_instructor_courses[x] = arg8;
    }
    
    Instructor *newinstructor = new Instructor(arg1, arg2, arg3, arg4, arg5, arg6, arg7, how_many_courses, new_instructor_courses);
    
    // Instructor object instance is dynamically created
    
    int counter = -5;      // To decide whether there is an empty space in Instructor array or not
    
    for(int x=0; x<instructor_array->array_size; x++)
        if(instructor_array->ogretmenler[x].first_name == ""){
            counter = x;
            break;
        }
    
    if(counter!=-5)     // This means there is an empty space in the Instructor array
        instructor_array->ogretmenler[counter]=* newinstructor;
    
    else{       // If there is no empty space in the Instructor array
        
        tempList =  *this->instructor_array;
        *this->instructor_array = tempList;
        
        // Array size is incremented by two (one for each assignment)
        
        for(int x=0; x<instructor_array->array_size; x++)
            if(instructor_array->ogretmenler[x].first_name == ""){
                counter = x;
                break;
            }
        instructor_array->ogretmenler[counter]=* newinstructor;
        
        // New instance is passed to the new space of the array
        
    }
    
    delete newinstructor;       // No need for newinstructor instance anymore
    
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
    
    char durdur;        // Will be used for termination

    switch(secenek){        // Choice of the user in the main menu
        case 1:
            add_operation();       // Since add operation is held in the Addressbook class
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
        return true;        // Terminating the program
    else{
        cout << "Invalid option, program terminates" << endl;
        return true;
    }
}

Addressbook::~Addressbook(){
    
    delete instructor_array;        // Deleting the dynamically allocated space

}

int main(){
    bool terminate= false;      // For terminating the whole program
    Addressbook defter;
    int choice;             // It will hold the user selection from the menu
    
    while(terminate!=true){
        defter.print_menu();
        cin >> choice;
        if(choice == 8)
            terminate = true;       // Terminating the program (If the selection is exit)
        else
            terminate = defter.perform_operation(choice);   // Running the program again
    }
    return 0;
}