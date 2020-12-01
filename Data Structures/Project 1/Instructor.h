/*
Name: Mehmet Barış Yaman
Number: 150130136
*/

//We need to define all maximum space for the strings

#define MAXTITLE 25
#define MAXFIRSTNAME 30
#define MAXLASTNAME 40
#define MAXTELEPHONENUMBER 20
#define MAXROOMNUMBER 20
#define MAXUSERNAME 20
#define MAXEMAILADDRESS 30
#define MAXCOURSENAME 10
#define MAXCOURSES 10
#include <string>
using namespace std;

//We need a struct to hold the name of the courses given by the instructors


//The class for each instructor object

class Instructor{
    friend class InstructorList;
    friend class Addressbook;
    
private:
    string title;
    string first_name;
    string last_name;
    string telephone_number;
    string room_number;
    string user_name;
    string email_address;
    string instructor_courses[MAXCOURSES];
public:
    Instructor(string = "", string =  "", string = "", string = "", string = "", string = "", string = "", int = 0, string * = NULL);
    void print_info();
};


Instructor::Instructor(string newtitle, string newfirstname, string newlastname, string newtelnumber, string newroomnumber, string newusername, string newemail, int course_array_length, string newcourses[MAXCOURSES] ){
    this->title = newtitle;
    this->first_name = newfirstname;
    this->last_name = newlastname;
    this->telephone_number = newtelnumber;
    this->room_number = newroomnumber;
    this->user_name = newusername;
    this->email_address = newemail;
    if(newcourses)
    for (int n = 0; n <course_array_length; n++)
        this->instructor_courses[n] = newcourses[n];
    
 }

void Instructor::print_info(){
    cout << "Title: " << this->title << endl;
    cout << "First Name: " << this->first_name << endl;
    cout << "Surname: " << this->last_name << endl;
    cout << "Telephone Number: " << this->telephone_number << endl;
    cout << "Room Number: " << this->room_number << endl;
    cout << "User Name: " << this->user_name << endl;
    cout << "Email: " << this->email_address << endl;
    cout << "Courses: [ ";
    int n=0;
    while(this->instructor_courses[n]!=""){
        cout << instructor_courses[n];
        n++;
        if(this->instructor_courses[n]!="")
            cout << ", ";
    }
    cout << " ]" << endl;
}
 
