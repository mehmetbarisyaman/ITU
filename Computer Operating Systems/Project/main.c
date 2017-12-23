/*
 STUDENT NAME: Mehmet Barış Yaman
 STUDENT NUMBER: 150130136
 */

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define SEMKEY_A 3
#define SEMKEY_B 2



//Structure of the shared memory space between threads

// increment operation
void sem_signal ( int semid , int val ) {
struct sembuf semaphore ;
semaphore.sem_num =0;
semaphore.sem_op=val ;
semaphore.sem_flg =1;
semop ( semid , &semaphore , 1 ) ;
}


//decrement operation
void sem_wait ( int semid , int val ) {
struct sembuf semaphore ;
semaphore.sem_num =0;
semaphore.sem_op=(-1*val) ;
semaphore.sem_flg =1;
semop ( semid , &semaphore , 1 ) ;
}



typedef struct question{
    char text[30];
    int approvals;
}Question;


typedef struct application{
int exam_score;
int interview_score;
int total_score;
} ApplicationForm;

//Mutex variable is needed for locking a part of the program

pthread_mutex_t psv;

int M;  //The time passed when the Registrar does the exam

int T;  //The number of the WEC Members

int I;  //The time passed when the interviewer makes the interview

int senkronize; //Is used for approving the questions

int acici;  //Used for opening the file just once

Question *sorular;  //Pointer to the array of questions

char dosyaadi[20];  //File name, will get a value from command line

FILE *inputfile;

int semthread;      // For the synchronization of threads
int semprocess;     // For the synchronization of process

void *thread_function(void *arg){
    
    int abc;    //Will be used in a for loop
    int sira;   //Will hold the order of the thread
    int time;   //The time for preapering a question
    int aa;     //Will be used in a for loop
    
    char message[30];   //To get the topic from the input file
    semthread = semget(SEMKEY_A, 1 , 0 ) ;
    pthread_mutex_lock(&psv);   //Only one thread prepares a question each time
    
    if(acici==0){       //The first thread opens the file

        inputfile = fopen(dosyaadi, "r+");
        if(!inputfile){
            printf("File could not be opened\n");
            exit(1);
        }
        acici++;    //The input file is opened only once in a whole program
    }
    if(!feof(inputfile)){

        fscanf(inputfile, "%d", &sira);
        fscanf(inputfile, "%s", message);
        fscanf(inputfile, "%d", &time);
    }

    strcpy(sorular[sira-1].text, message);  //Question is prepared
    sorular[sira-1].approvals = 0;     //Initial approval value is given
    printf("WEC Member %d: A question is prepared on %s \n", sira, message);
    fflush(stdout);
    sleep(time);    //Sleeps the time for preparing the question

    /*  The last thread(when senkronize is equal to T)
     which enters the locked part sends the semaphore signal
        to other threads to wake them up    */

    senkronize++;
    if(senkronize==T){
        for(abc=1; abc<=T-1; abc++)
            sem_signal(semthread , 1);
    }
    pthread_mutex_unlock(&psv);

    /*All threads except the last thread that comes from the locked part have to wait until all threads(WEC members) prepare a question */

    if(senkronize!=T)
        sem_wait(semthread, 1);

    //Approving Questions Part

    for(aa=0; aa<=T-1; aa++){
        if(aa==sira-1)  //WEC Members could not approve their own question
            continue;
        if(strcmp(sorular[aa].text, "")!=0){
            sorular[aa].approvals++;
            if(sorular[aa].approvals==T-1)  //If the question is fully approved
                printf("WEC Member %d: Question %d is ready\n", sira, aa+1);
        }
    }
    sleep(1);   //Sleep for a second(approving time)
    pthread_exit(NULL);
}

int main(int argc, const char * argv[]) {
    srand(time(NULL));
    void *status;
    pthread_attr_t attr;    //For joining all threads with main thread at last
    senkronize=0;   //Used in thread_function

    if(pthread_mutex_init(&psv, NULL)!=0){
        printf("Mutex could not be created\n");
        exit(-1);
    }
    strcpy(dosyaadi, "inputfile.txt");
    semthread = semget (SEMKEY_A , 1 , 0700| IPC_CREAT ) ;
    semctl ( semthread , 0 , SETVAL , 1 ) ;
    semprocess=semget (SEMKEY_B , 1 , 0700| IPC_CREAT ) ;
    semctl ( semprocess , 0 , SETVAL , 1 ) ;

    // The values obtained in the command line are:
    T=atoi(argv[2]);
    M = atoi(argv[3]);
    I = atoi(argv[4]);
    
    acici=0;       //Used in thread_function
    strcpy(dosyaadi,argv[1]);   //Input file name is obtained from command line
    Question rc[T];     //Question array is created
    ApplicationForm applicants[3];  //Assuming there are 3 applicants
    sorular = rc;
    int x;  //Used in for loop related on joining the threads
    int order;  //Used in for loop related on creating the threads
    int order2; //Used in the other for loop
    int created;    //Return value of thread creator function

    pthread_t thread[T];
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr , PTHREAD_CREATE_JOINABLE) ;

    printf("The Registrar started taking applications \n");
    printf("The Registrar finished taking applications \n");
    sleep(3);

    int f = fork();
    if(f==-1){
        printf("Process could not be created. \n");
        exit(-1);
    }

    if(f==0){   // WEC Members Process (Child Process)
        printf("The Written Exams Committee started preparing questions.\n");
        semprocess = semget(SEMKEY_B , 1 , 0 ) ;
        for(order=0; order<T; order++){
            created = pthread_create(&thread[order], &attr, thread_function, (void *)order);
            if(created){
                printf("Thread could not be created since the return value is %d\n", created);
                exit(-1);
            }
        }
        //All threads will join the main thread
        for(x=0; x<T; x++) {
            if(pthread_join(thread[x], &status)){
                printf("ERROR in joining the threads\n");
                exit(-1);
            }
        }

        printf("The Written Exams Committee finished preparing questions.\n");
        sem_signal(semprocess, 1);
    }
    else{
        wait(NULL);
        exit(0);
    }

    sem_wait(semprocess, 1);
    printf("The Registrar started the written exam.\n");
    for(order2=0; order2<=2; order2++){
        applicants[order2].exam_score = rand() %50;
        printf("Written exam score of applicant %d is %d.\n", order2 + 1, applicants[order2].exam_score);
    }
    sleep(M);
    printf("The Registrar finished the written exam.\n");
    f = fork();
    if(f==-1){
        printf("Process could not be created. \n");
        exit(-1);
    }
    if(f==0){  // Interviewer (Child Process)

        semprocess = semget(SEMKEY_B , 1 , 0 ) ;
        printf("The Interviewer started interviews. \n");
        for(order2 =0; order2<=2; order2++){
            applicants[order2].interview_score = rand() %50;
            printf("Interview score of applicant %d is %d.\n", order2+1, applicants[order2].interview_score);
        }
        sleep(I);
        printf("The Interviewer finished interviews. \n");
        sem_signal(semprocess, 1);
    }
    
    else{
    wait(NULL);
    exit(0);
    }
    
    sem_wait(semprocess, 1);
    printf("The Registrar started calculating total scores. \n");
    for(order2=0; order2<=2; order2++){
        applicants[order2].total_score = applicants[order2].exam_score + applicants[order2].interview_score;
        printf("Total score of applicant %d is %d.\n", order2+1, applicants[order2].total_score);
    }
    sleep(3);
    printf("The Registrar finished calculating total scores. \n");

    //Allocated resources will be removed
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&psv);
    pthread_exit(NULL);
    semctl( semprocess , 0 , IPC_RMID , 0 ) ;
    semctl ( semthread , 0 , IPC_RMID , 0 ) ;

    return 0;
}