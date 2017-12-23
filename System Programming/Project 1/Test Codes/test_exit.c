#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#define NR_myflag 356

int main(int argc, char *argv[]){
	long int result;
	pid_t f;
	
	int flag = atoi(argv[1]);
	int niceValue = atoi(argv[2]);
	printf("Flag: %d \nNice: %d \n", flag, niceValue);
	
	f=fork();
	if(f==-1){
		
		printf("New process could not created.");
		return EXIT_FAILURE;
	
	}else if(f==0){
		
		pid_t child_pid = getpid();
		printf("Child: My Pid: %d \n", child_pid);
		printf("Child: My Parent's Pid: %d \n", getppid());
		
		sleep(3);
		printf("Child: Terminating\n");
			
	}else{
		printf("Parent: My Pid: %d \n", getpid());
		printf("Parent: My Child's Pid: %d \n", f);
		nice(niceValue);
		sleep(1);
		
		result = syscall(NR_myflag, getpid() ,flag);
		if(result<0){
			//perror("Error");
			switch(errno){
				case EPERM:
					printf("Parent: Operation not permitted - Try to use sudo command.\n");
					break;
				
				case EINVAL:
					printf("Parent: Invalid flag value - Flag values could be 0 or 1.\n");
					break;
					
				case ESRCH:
					printf("Parent: No such process - The given PID does not match any process.\n");
					break;
			}
		}else{
			printf("Parent: Executed successfully \n");
			printf("Parent: Return value of the system call: %ld \n", result);
		}

		printf("Parent: Terminating\n");
		exit(0);
	}
	
	return 0;
}
