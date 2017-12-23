#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>	/* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/cdev.h>

#include <asm/switch_to.h>		/* cli(), *_flags */
#include <asm/uaccess.h>	/* copy_*_user */

#include "messagebox_ioctl.h"

#define MESSAGEBOX_MAJOR 0
#define MESSAGEBOX_NR_DEVS 2
#define MESSAGEBOX_UNREAD_LIMIT 3
#define MESSAGEBOX_READ_ALL 0

int messagebox_major = MESSAGEBOX_MAJOR;
int messagebox_minor = 0;
int messagebox_unread = 0;
int messagebox_unread_limit = MESSAGEBOX_UNREAD_LIMIT;
int messagebox_nr_devs = MESSAGEBOX_NR_DEVS;
int messagebox_read_all = MESSAGEBOX_READ_ALL;

module_param(messagebox_major, int, S_IRUGO);
module_param(messagebox_minor, int, S_IRUGO);
module_param(messagebox_unread, int, S_IRUGO);
module_param(messagebox_unread_limit, int, S_IRUGO);
module_param(messagebox_nr_devs, int, S_IRUGO);
module_param(messagebox_read_all, int, S_IRUGO);

MODULE_AUTHOR("Goksel Coban, Baris Yaman");
MODULE_LICENSE("Dual BSD/GPL");

struct message{
	char* data;
	unsigned int reciever;
	unsigned int sender;
	unsigned int size_of_data;
	int is_read;
	struct message* next;
};

struct messagebox_dev {
    struct message* start;
    struct message* end;
		int unread;
		int unread_limit;
		int read_all;
    struct semaphore sem;
    struct cdev cdev;
};

struct messagebox_dev *messagebox_devices;


int message_trim(struct message *msg)
{
		printk(KERN_WARNING "message_trim\n");
    if (msg->data) {
        kfree(msg->data);
    }

    msg->data = NULL;
    msg->sender = 0;
    msg->reciever = 0;
    msg->size_of_data = 0;
    msg->is_read = 0;
    msg->next = NULL;
    return 0;
}


int messagebox_trim(struct messagebox_dev *dev)
{
		printk(KERN_WARNING "messagebox_trim\n");
    while(dev->start){
			struct message* temp = dev->start->next;
			message_trim(dev->start);
			kfree(dev->start);
			dev->start = temp;
		}

		dev->unread = 0;
		dev->unread_limit = 0;
		dev->read_all = 0;
    dev->start = NULL;
    dev->end = NULL;
    return 0;
}


int messagebox_open(struct inode *inode, struct file *filp)
{
		printk(KERN_WARNING "messagebox_open\n");
		struct messagebox_dev *dev;

    dev = container_of(inode->i_cdev, struct messagebox_dev, cdev);
    filp->private_data = dev;

    return 0;
}


int messagebox_release(struct inode *inode, struct file *filp)
{
		printk(KERN_WARNING "messagebox_release\n");
    return 0;
}

ssize_t messagebox_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
		printk(KERN_WARNING "messagebox_read\n");
		printk(KERN_NOTICE "read start: %d \n", (*f_pos));

    struct messagebox_dev *dev = filp->private_data;
    ssize_t retval = 0;

		if (down_interruptible(&dev->sem))
        return -ERESTARTSYS;

    if (dev->start == NULL)
        goto out;

		unsigned int counter = 0;
		unsigned int read = 0;
		struct message* iterator = dev->start;

		while( iterator ){
			printk(KERN_WARNING "iterator %s\n", iterator->data);
			if( counter >= (*f_pos) && ((!iterator->is_read) ||  dev->read_all)){
				if (copy_to_user(buf, iterator->data, iterator->size_of_data)) {
				     retval = -EFAULT;
			      goto out;
			  }
				if(iterator->is_read == 0 ){
					iterator->is_read = 1;
					dev->unread -= 1;
				}

				printk(KERN_WARNING "read end: %s\n", iterator->data);
				read = iterator->size_of_data;
				break;
			}

			counter += iterator->size_of_data;
			iterator = iterator->next;
		}

		*f_pos += read;
    retval = read;

  out:
		if(retval==0){
			*f_pos = 0;
		}
    up(&dev->sem);
		printk(KERN_NOTICE "retun val: %d\n", retval);
    return retval;
}

ssize_t messagebox_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
		printk(KERN_NOTICE "write start: %d \n", (*f_pos));
    struct messagebox_dev *dev = filp->private_data;
    ssize_t retval = -ENOMEM;

		if (down_interruptible(&dev->sem))
        return -ERESTARTSYS;

		printk(KERN_WARNING "messagebox_write unread %d limit %d\n", dev->unread, dev->unread_limit);
		if( dev->unread >= dev->unread_limit ){
			retval = -ENOMEM;
			goto out;
		}

		if (*f_pos >= count) {
			retval = 0;
			goto out;
		}

		struct message* new_message;
		new_message = kmalloc(sizeof(struct message) , GFP_KERNEL);
		if(!new_message){
			retval = -ENOMEM;
			goto out;
		}

		new_message->size_of_data = count;
		new_message->is_read = 0;
		new_message->sender = 0;
		new_message->reciever = 0;
		new_message->next = NULL;
		new_message->data = kmalloc(count * sizeof(char), GFP_KERNEL);
		if(!new_message->data){
			retval = -ENOMEM;
			goto out;
		}

		if (copy_from_user(new_message->data, buf, count)) {
			retval = -EFAULT;
			goto out;
		}

		if(dev->start){
			dev->end->next = new_message;
			dev->end = dev->end->next;
		}else{
			dev->start = new_message;
			dev->end = new_message;
		}
		dev->unread += 1;
		*f_pos += count;
    retval = count;
  out:
		printk(KERN_NOTICE "write out fpos: %d \n", (*f_pos));
		*f_pos = 0;
    up(&dev->sem);
    return retval;
}

long messagebox_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct messagebox_dev *dev = filp->private_data;

	int err = 0;
	int retval = 0;

	/*
	 * extract the type and number bitfields, and don't decode
	 * wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
	 */
	if (_IOC_TYPE(cmd) != MESSAGEBOX_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > MESSAGEBOX_IOC_MAXNR) return -ENOTTY;

	/*
	 * the direction is a bitmask, and VERIFY_WRITE catches R/W
	 * transfers. `Type' is user-oriented, while
	 * access_ok is kernel-oriented, so the concept of "read" and
	 * "write" is reversed
	 */
	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	if (err) return -EFAULT;

	switch(cmd) {
		case MESSAGEBOX_EXCLUDE_READ:
			if (! capable (CAP_SYS_ADMIN))
				return -EPERM;
			dev->read_all= 0;
			break;

		case MESSAGEBOX_INCLUDE_READ:
			if (! capable (CAP_SYS_ADMIN))
				return -EPERM;
			dev->read_all= 1;
			break;

		case MESSAGEBOX_CHANGE_UNREAD_LIMIT:
			if (! capable (CAP_SYS_ADMIN))
				return -EPERM;
			dev->unread_limit = arg;
			break;

		// case MESSAGEBOX_REMOVE_ALL:
		// 	if (! capable (CAP_SYS_ADMIN))
		// 		return -EPERM;
		// 	retval = __get_user(dev->unread_limit, (char __user *)arg);
		// 	break;

	  default:  /* redundant, as cmd was checked against MAXNR */
		return -ENOTTY;
	}
	return retval;
}

struct file_operations messagebox_fops = {
    .owner =    THIS_MODULE,
    .read =     messagebox_read,
    .write =    messagebox_write,
    .unlocked_ioctl =  messagebox_ioctl,
    .open =     messagebox_open,
    .release =  messagebox_release,
};


void messagebox_cleanup_module(void)
{
		printk(KERN_WARNING "messagebox_cleanup_module\n");
		int i;
    dev_t devno = MKDEV(messagebox_major, messagebox_minor);

    if (messagebox_devices) {
        for (i = 0; i < messagebox_nr_devs; i++) {
            messagebox_trim(messagebox_devices + i);
            cdev_del(&messagebox_devices[i].cdev);
        }
    kfree(messagebox_devices);
    }

    unregister_chrdev_region(devno, messagebox_nr_devs);
}


int messagebox_init_module(void)
{
		printk(KERN_WARNING "messagebox_init_module\n");
    int result, i;
    int err;
    dev_t devno = 0;
    struct messagebox_dev *dev;

    if (messagebox_major) {
        devno = MKDEV(messagebox_major, messagebox_minor);
        result = register_chrdev_region(devno, messagebox_nr_devs, "messagebox");
    } else {
        result = alloc_chrdev_region(&devno, messagebox_minor, messagebox_nr_devs, "messagebox");
        messagebox_major = MAJOR(devno);
    }

    if (result < 0) {
        printk(KERN_WARNING "messagebox: can't get major %d\n", messagebox_major);
        return result;
    }

    messagebox_devices = kmalloc(messagebox_nr_devs * sizeof(struct messagebox_dev), GFP_KERNEL);
    if (!messagebox_devices) {
        result = -ENOMEM;
        goto fail;
    }
    memset(messagebox_devices, 0, messagebox_nr_devs * sizeof(struct messagebox_dev));

    /* Initialize each device. */
    for (i = 0; i < messagebox_nr_devs; i++) {
        dev = &messagebox_devices[i];
        dev->start = NULL;
        dev->end = NULL;
				dev->unread = messagebox_unread;
				dev->unread_limit = messagebox_unread_limit;
				dev->read_all = messagebox_read_all;
        sema_init(&dev->sem,1);
        devno = MKDEV(messagebox_major, messagebox_minor + i);
        cdev_init(&dev->cdev, &messagebox_fops);
        dev->cdev.owner = THIS_MODULE;
        dev->cdev.ops = &messagebox_fops;
        err = cdev_add(&dev->cdev, devno, 1);
        if (err)
            printk(KERN_NOTICE "Error %d adding messagebox%d", err, i);
    }

    return 0;

  fail:
    messagebox_cleanup_module();
    return result;
}

module_init(messagebox_init_module);
module_exit(messagebox_cleanup_module);
