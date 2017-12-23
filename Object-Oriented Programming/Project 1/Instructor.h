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
#define MAXCOURSES 10
#define MAXCOURSENAME 10
#include <string>

using namespace std;

//The class for each instructor object

/* We have to define Addressbook and InstructorList Class as a friend, because we will access
 the Instructor members from other classes to execute the functions */

class Instructor{
    friend class InstructorList;
    friend class Addressbook;
    
private:
    
    // Its better and simple to select string Data Type for each Instructor class member
    
    string title;
    string first_name;
    string last_name;
    string telephone_number;
    string room_number;
    string user_name;
    string email_address;
    string instructor_courses[MAXCOURSES];  // We can use string array to hold the courses
    
public:
    
    // We create Instructor objects with constructors to give the value of class members
    // Default values of an Instructor are given
    
    Instructor(string = "", string =  "", string = "", string = "", string = "", string = "", string = "", int = 0, string * = NULL);
    
    //For the Constructor which works with partial information
    
    Instructor(string, string , string = "", string = "", string = "");
    
    void print_info();
    
    /* There is no need to use Deconstructors since there is no dynamically created members for this object */
    
    void settitle(const string title_in){
        title = title_in;
    }
    string gettitle() const{
        return title;
    }
    void setfirst_name(const string first_name_in){
        first_name = first_name_in;
    }
    string getfirst_name() const{
        return first_name;
    }
    void setlast_name(const string last_name_in){
        last_name = last_name_in;
    }
    string getlast_name()const{
        return last_name;
    }
    void settelephone_number(const string telephone_number_in){
        telephone_number = telephone_number_in;
    }
    string gettelephone_number()const{
        return telephone_number;
    }
    void setroom_number(const string room_number_in){
        room_number = room_number_in;
    }
    string getroom_number()const{
        return room_number;
    }
    void setuser_name(const string user_name_in){
        user_name = user_name_in;
    }
    string getuser_name()const{
        return user_name;
    }
    void setemail_address(const string email_address_in){
        email_address = email_address_in;
    }
    string getemail_address()const{
        return email_address;
    }
    
    // Setter and Getter Methods are defined
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
    for (int n = 0; n <course_array_length; n++)  // In order to take the value of courses
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
    
    // In order to print the info of each instructors clearly
    
    while(this->instructor_courses[n]!=""){ //There is no need to show the newly created space
        cout << instructor_courses[n];
        n++;
        if(this->instructor_courses[n]!="")
            cout << ", ";
    }
    cout << " ]" << endl;
}
 
