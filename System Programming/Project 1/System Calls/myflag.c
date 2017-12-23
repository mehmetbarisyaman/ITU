#include<asm/current.h>
#include<asm/errno.h>
#include<linux/kernel.h>
#include<linux/linkage.h>
#include<linux/sched.h>
#include<linux/syscalls.h>

asmlinkage long set_myFlag(pid_t pid, int flag){
	struct task_struct *callerProcess;
	
	// Check root privileges
 	if(!capable(CAP_SYS_ADMIN))
		// Operation not permitted
 		return -EPERM;
 		
 	// Check is flag valid	
  	if(flag != 0 && flag != 1)
		// Invalid argument
  		return -EINVAL;
  		
  	// Find a process with matching pid
  	callerProcess = find_task_by_vpid(pid);
  	
  	// Check is there a matmatching process
  	if(callerProcess == NULL)
		// No such process
  		return -ESRCH;
  	
  	// Set myFlag
  	callerProcess->myFlag = flag;
  	
  	// Executed successfully
  	return 0;
}
