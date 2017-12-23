#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<sys/ioctl.h>

#include "messagebox_ioctl.h"

int main(){
    int fd, retval;
    char ch, write_buf[100];
    int tmp = 0;

    fd = open("/dev/messagebox0", O_RDWR);
		if(fd == -1 ){
			printf("could not open /dev/messagebox0\n");
			return 0;
		}

    while (1){
        printf("Settings\nSet exclue read mode (a)\nSet include read mode (b)\nChange unread limit (c)\nExit (e)\n");
        scanf(" %c", &ch);

        switch (ch) {
          case 'a':
            retval = ioctl(fd, MESSAGEBOX_EXCLUDE_READ);
            if(retval != 0 ){
              printf("ioctl error\n");
              return retval;
            }
            printf("OK!\n");
            break;

          case 'b':
            retval = ioctl(fd, MESSAGEBOX_INCLUDE_READ);
            if(retval != 0 ){
              printf("ioctl error\n");
              return retval;
            }
            printf("OK!\n");
            break;

          case 'c':
            printf("Enter new limit: ");
            scanf("%d", &tmp);
            retval = ioctl(fd, MESSAGEBOX_CHANGE_UNREAD_LIMIT, tmp);
            if(retval != 0 ){
              printf("ioctl  error\n");
              return retval;
            }
            printf("OK!\n");
            break;

          case 'e':
            printf("Bye!\n");
            return 0;

          default:
            printf("Wrong input!\n");

        }
    }

    close(fd);
    return 0;
}
