#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#define NR_myflag 356

int main(){
	long int result;
	
	pid_t pid = getpid();
	int flag = 0;
	
	printf("Pid: %d & Flag: %d\n", pid, flag);
	
	
	result = syscall(NR_myflag, pid ,flag);
	if(result<0){
		//perror("Error");
		switch(errno){
			case EPERM:
				printf("Operation not permitted - Try to use sudo command.\n");
				break;
			
			case EINVAL:
				printf("Invalid flag value - Flag values could be 0 or 1.\n");
				break;
				
			case ESRCH:
				printf("No such process - The given PID does not match any process.\n");
				break;
		}
		return EXIT_FAILURE;
	}	
	printf("Executed successfully \n");
	printf("Return value of the system call: %ld \n", result);

	return 0;
}
