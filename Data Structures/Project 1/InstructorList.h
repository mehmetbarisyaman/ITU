/* 
Name: Mehmet Barış Yaman
Number: 150130136
*/

#include "Instructor.h"

using namespace std;

//Instructor List Class

class InstructorList{
    friend class Addressbook;
private:
    int array_size;
    Instructor *ogretmenler;
public:
    void operator=(const InstructorList&);
    void delete_instructor();
    void list_instructors();
    void search_firstname();
    void search_lastname();
    void search_telephonenumber();
    void search_roomnumber();
    InstructorList();
    ~InstructorList();
};

InstructorList::InstructorList(){
    this->array_size = 3;
    this->ogretmenler = new Instructor[array_size];
    string HaticeKose_courses[10];
    HaticeKose_courses[0] = "BLG 252E";
    string ZehraCataltepe_courses[10];
    ZehraCataltepe_courses[0] = "BLG 335E";
    string TolgaOvatman_courses[10];
    TolgaOvatman_courses[0] = "BLG 311E";
    TolgaOvatman_courses[1] = "BLG 411E";
    Instructor *HaticeKose = new Instructor("Assist. Prof. Dr.", "Hatice", "Kose", "2853593", "3315", "hbkose", "hatice.kose@itu.edu.tr", 1, HaticeKose_courses);
    Instructor *ZehraCataltepe = new Instructor("Prof. Dr.", "Zehra", "Cataltepe", "2853551", "3301", "cataltepe", "cataltepe@itu.edu.tr", 1, ZehraCataltepe_courses);
    Instructor *TolgaOvatman = new Instructor("Yrd. Doç. Dr.", "Tolga", "Ovatman", "2856703", "5309", "ovatman", "ovatman@itu.edu.tr", 2, TolgaOvatman_courses);
    ogretmenler[0] = *HaticeKose;
    ogretmenler[1] = *ZehraCataltepe;
    ogretmenler[2] = *TolgaOvatman;
}



void InstructorList::operator=(const InstructorList &inList){
    array_size = inList.array_size +1;
    ogretmenler = new Instructor[array_size];
    for( int x=0; x<inList.array_size; x++){
        ogretmenler[x] = inList.ogretmenler[x];
    }
}



void InstructorList::delete_instructor(){
    char delete_firstname [MAXFIRSTNAME];
    char delete_lastname [MAXLASTNAME];
    bool deleted = false;
    cout << "Enter the first name of the record to be deleted: ";
    cin >> setw(MAXFIRSTNAME) >> delete_firstname;
    cout << "Enter the last name of the record to be deleted: ";
    cin >> setw(MAXLASTNAME) >> delete_lastname;
    for(int n = 0; n< array_size; n++){
        if(ogretmenler[n].first_name == delete_firstname && ogretmenler[n].last_name == delete_lastname){
            ogretmenler[n].title = "";
            ogretmenler[n].first_name =  "";
            ogretmenler[n].last_name =  "";
            ogretmenler[n].telephone_number =  "";
            ogretmenler[n].room_number =  "";
            ogretmenler[n].user_name =  "";
            ogretmenler[n].email_address =  "";
            deleted = true;
            for(int i = 0; i<MAXCOURSES; i++)
                for(int j = 0; j<10; j++)
                    ogretmenler[n].instructor_courses[i] = "";
        }
            
    }
    if(deleted)
        cout << "Record has been deleted!" << endl;
    else
        cout << "No record found" << endl;
    
}


void InstructorList::list_instructors(){
    cout << "----------List of all Instructors in Computer Engineering of ITU----------" << endl;
    for(int n = 0; n< array_size; n++){
        if(ogretmenler[n].title == "")
            continue;
        ogretmenler[n].print_info();
        cout << "--------------------------------------" << endl;
    }
}

void InstructorList::search_firstname(){
    char search_name[MAXFIRSTNAME];
    cout << "Enter the first name: ";
    cin >> setw(MAXFIRSTNAME) >> search_name;
    cout << "----------Search Results----------" << endl;
    for(int n = 0; n< array_size; n++){
        if(ogretmenler[n].first_name == search_name)
            ogretmenler[n].print_info();
    }
}

void InstructorList::search_lastname(){
    char search_name[MAXLASTNAME];
    cout << "Enter the last name: ";
    cin >> setw(MAXLASTNAME) >> search_name;
    cout << "----------Search Results----------" << endl;
    for(int n = 0; n< array_size; n++){
        if(ogretmenler[n].last_name == search_name)
            ogretmenler[n].print_info();
    }
}

void InstructorList::search_telephonenumber(){
    char search_number[MAXTELEPHONENUMBER];
    cout << "Enter the telephone number: ";
    cin >> setw(MAXTELEPHONENUMBER) >> search_number;
    cout << "----------Search Results----------" << endl;
    for(int n = 0; n< array_size; n++){
        if(ogretmenler[n].telephone_number == search_number)
            ogretmenler[n].print_info();
    }
}

void InstructorList::search_roomnumber(){
    char search_number[MAXROOMNUMBER];
    cout << "Enter the room number: ";
    cin >> setw(MAXROOMNUMBER) >> search_number;
    cout << "----------Search Results----------" << endl;
    for(int n = 0; n< array_size; n++){
        if(ogretmenler[n].room_number == search_number)
            ogretmenler[n].print_info();
    }
}

InstructorList::~InstructorList(){
    delete [] ogretmenler;
}