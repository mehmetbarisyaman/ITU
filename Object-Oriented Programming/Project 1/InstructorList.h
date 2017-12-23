/* 
Name: Mehmet Barış Yaman
Number: 150130136
*/

#include "Instructor.h"

using namespace std;

//Instructor List Class

class InstructorList{
    
    friend class Addressbook;  // The class Addressbook should access the members of this class
    
private:
    
    int array_size;     // Size of the instructor array will be used to implement the operations
    
    Instructor *ogretmenler;    // Pointer of the Instructor array class that will be dynamically created
    
public:
    
    /* The class Addressbook will have the adding function since the assignment between InstructorList objects will be needed to expand the arrays */
    
    void operator=(const InstructorList&);  // In order to expand the Instructor Array size
    void delete_instructor();
    void list_instructors();
    void search_firstname();
    void search_lastname();
    void search_telephonenumber();
    void search_roomnumber();
    InstructorList();
    ~InstructorList();      // It is necessary for dynamically created members of this class
};

InstructorList::InstructorList(){
    
    this->array_size = 3;       // First value is given by hand
    
    this->ogretmenler = new Instructor[array_size];
    
    // It will be better and simple to use string arrays for courses from each Instructors
    
    string HaticeKose_courses[10];
    HaticeKose_courses[0] = "BLG 252E";
    string ZehraCataltepe_courses[10];
    ZehraCataltepe_courses[0] = "BLG 335E";
    string TolgaOvatman_courses[10];
    TolgaOvatman_courses[0] = "BLG 311E";
    TolgaOvatman_courses[1] = "BLG 411E";
    
    // The arrays for each instructors are defined and necessary parts are filled

    Instructor *HaticeKose = new Instructor("Assist. Prof. Dr.", "Hatice", "Kose", "(+90 212) 2853593", "3315", "hbkose", "hatice.kose@itu.edu.tr", 1, HaticeKose_courses);
    Instructor *ZehraCataltepe = new Instructor("Prof. Dr.", "Zehra", "Cataltepe", "(+90 212) 2853551", "3301", "cataltepe", "cataltepe@itu.edu.tr", 1, ZehraCataltepe_courses);
    Instructor *TolgaOvatman = new Instructor("Yrd. Doç. Dr.", "Tolga", "Ovatman", "(+90 212) 2856703", "5309", "ovatman", "ovatman@itu.edu.tr", 2, TolgaOvatman_courses);
    
    // Instructors are dynamically created and members value are given by constructors
    
    ogretmenler[0] = *HaticeKose;
    ogretmenler[1] = *ZehraCataltepe;
    ogretmenler[2] = *TolgaOvatman;
    
    /* We dont need the space for each dynamically created Instructors anymore since we pass those objects to the array */
    
    delete HaticeKose;
    delete TolgaOvatman;
    delete ZehraCataltepe;
}

// Overloading the assignment operator is needed for expanding the array size

void InstructorList::operator=(const InstructorList &inList){
    
    // We can expand the array by multiplying the array size by 2 etc.
    // My selection is expanding the array size by 1 in each assignment
    /* Since there will be 2 assignments ( to tempList and to new array in the class Addressbook ) the array size will increment by 2 */
    
    array_size = inList.array_size +1;      // Array size will increment by 1 for each assignment
    ogretmenler = new Instructor[array_size];  // The array with new size is created
    for( int x=0; x<inList.array_size; x++){
        ogretmenler[x] = inList.ogretmenler[x];  // Array values are passed
    }
}



void InstructorList::delete_instructor(){
    char delete_firstname [MAXFIRSTNAME];
    char delete_lastname [MAXLASTNAME];
    bool deleted = false;
    cout << "Enter the first name of the record to be deleted: ";
    cin.ignore(1000, '\n');     // To prevent the crash dumping with space characters
    cin.getline(delete_firstname, MAXFIRSTNAME);
    cout << "Enter the last name of the record to be deleted: ";
    cin >> setw(MAXLASTNAME) >> delete_lastname;
    
    // Deleting operation
    
    for(int n = 0; n< array_size; n++){
        
        // First and last names are same as given inputs for deleting
        
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
        if(ogretmenler[n].title == "")  // There is no need to show empty array members
            continue;
        ogretmenler[n].print_info();    // print info function is used (from Instructor class)
        cout << "--------------------------------------" << endl;
    }
}

// The interface of the searching Instructor functions are almost same

void InstructorList::search_firstname(){
    char search_name[MAXFIRSTNAME];
    cout << "Enter the first name: ";
    cin.ignore(1000, '\n');
    cin.getline(search_name, MAXFIRSTNAME);
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
    cin.ignore(1000, '\n');
    cin.getline(search_number, MAXTELEPHONENUMBER);
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
    
    delete [] ogretmenler;      // To release the dynamically created array space
    
}