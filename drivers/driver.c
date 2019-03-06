#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/semaphore.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>			// copy_to_user(), copy_from_user()

static int major;
struct cdev *kernel_cdev;			// Kernel monitoring of the device

struct device {
	char disk[100];
	struct semaphore sem;
} char_arr;


static int __init char_init(void)
{
	int ret;
	dev_t devno, dev_r;
	printk(KERN_INFO "Initiating init module");

	if ((ret = alloc_chrdev_region(&devno, 0, 1, "char_device")) < 0) {
		printk(KERN_INFO "Major number allocation failed");
		return ret;
	}

	major = MAJOR(devno);
	printk(KERN_INFO "The major number is %d", MAJOR(devno));
	dev_r = MKDEV(major, 0);

	// Allocate file-operations to cdev structure
	kernel_cdev = cdev_alloc();
	kernel_cdev->ops = &fops;
	kernel_cdev->owner = THIS_MODULE;

	if ((ret = cdev_add(kernel_cdev, dev_r, 1)) < 0) {
		printk(KERN_INFO "Unable to inform kernel about cdev");
		return ret;
	}

	// Initialize device semaphore
	sema_init(&char_arr.sem, 1);
	return 0;
}

static void __exit char_exit(void)
{
	printk(KERN_INFO "Initiating cleanup module");
	cdev_del(kernel_cdev);
	unregister_chrdev_region(devno, 1);
}

module_init(char_init);
module_exit(char_exit);