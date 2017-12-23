#ifndef __MESSAGEBOX_H
#define __MESSAGEBOX_H

#include <linux/ioctl.h> /* needed for the _IOW etc stuff used later */

#define MESSAGEBOX_IOC_MAGIC  'k'

#define MESSAGEBOX_EXCLUDE_READ			_IO(MESSAGEBOX_IOC_MAGIC, 0)
#define MESSAGEBOX_INCLUDE_READ			_IO(MESSAGEBOX_IOC_MAGIC, 1)
#define MESSAGEBOX_CHANGE_UNREAD_LIMIT	_IOW(MESSAGEBOX_IOC_MAGIC, 2, int)
// #define MESSAGEBOX_REMOVE_ALL			      _IOW(MESSAGEBOX_IOC_MAGIC, 3, char*)

#define MESSAGEBOX_IOC_MAXNR 2

#endif /* messagebox_h */
