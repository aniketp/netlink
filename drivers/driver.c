#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/semaphore.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>			// copy_to_user(), copy_from_user()

static int major;
dev_t devno, dev_r;
struct cdev *kernel_cdev;			// Kernel monitoring of the device

struct device_ {
	char disk[100];
	struct semaphore sem;
} char_arr;

static int char_open(struct inode *inode, struct file *filp)
{
	printk(KERN_NOTICE "Inside open() function");
	
	// Disallow multiple processes to open() char device simultaneously
	if (down_interruptible(&char_arr.sem)) {
		printk(KERN_INFO "Could not hold the device semaphore");
		return -1;
	}
	return 0;
}

static int char_release(struct inode *inode, struct file* filp)
{
	printk(KERN_NOTICE "Inside release() function\n");
	printk(KERN_INFO "Releasing semaphore");
	up(&char_arr.sem);
	return 0;
}

static ssize_t char_read(struct file *filp, char *buff, size_t count, loff_t *offset)
{
	unsigned long ret;
	printk(KERN_NOTICE "Inside read() function");
	ret = copy_to_user(buff, char_arr.disk, count);
	return ret;
}

static ssize_t char_write(struct file *filp, const char *buff, size_t count, loff_t *offset)
{
	unsigned long ret;
	printk(KERN_NOTICE "Inside write() function");
	ret = copy_from_user(char_arr.disk, buff, count);
	return count;
}

// Pointers to various file-operations on 'char_device'
struct file_operations fops = {
	read:	 char_read,
	write:	 char_write,
	open:	 char_open,
	release: char_release
};


static int __init char_init(void)
{
	int ret;

	// Allocate file-operations to cdev structure
	kernel_cdev = cdev_alloc();
	kernel_cdev->ops = &fops;
	kernel_cdev->owner = THIS_MODULE;
	printk(KERN_INFO "Initiating init module");

	if ((ret = alloc_chrdev_region(&devno, 0, 1, "char_device")) < 0) {
		printk(KERN_INFO "Major number allocation failed");
		return ret;
	}

	major = MAJOR(devno);
	dev_r = MKDEV(major, 0);
	// Initialize device semaphore
	sema_init(&char_arr.sem, 1);
	printk("The major number is %d", MAJOR(devno));

	if ((ret = cdev_add(kernel_cdev, dev_r, 1)) < 0) {
		printk(KERN_INFO "Unable to inform kernel about cdev");
		return ret;
	}
	return 0;
}

static void __exit char_exit(void)
{
	printk(KERN_INFO "Initiating cleanup module");
	cdev_del(kernel_cdev);
	unregister_chrdev_region(major, 1);
}

MODULE_LICENSE("GPL");
module_init(char_init);
module_exit(char_exit);