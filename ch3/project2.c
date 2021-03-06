#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>


#define BUFFER_SIZE 128

#define PROC_NAME "pid"

/**
 * Function prototypes
 */
static ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);
static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos);
long id = -1;
static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
    .write = proc_write};

/* This function is called when the module is loaded. */
static int proc_init(void)
{

	// creates the /proc/pid entry
	// the following function call is a wrapper for
	// proc_create_data() passing NULL as the last argument
	proc_create(PROC_NAME, 0, NULL, &proc_ops);

	printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
	return 0;
}

/* This function is called when the module is removed. */
static void proc_exit(void)
{

	// removes the /proc/hello entry
	remove_proc_entry(PROC_NAME, NULL);

	printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos)
{
	char *k_mem;
	//alocate kernel memory
	k_mem = kmalloc(count, GFP_KERNEL);
	//copies user space usr_buf to kenrel memory
	copy_from_user(k_mem, usr_buf, count);
	printk(KERN_INFO "%s\n", k_mem);
	kstrtol(k_mem, 10, &id);
	kfree(k_mem);
	return count;
}

/**
 * This function is called each time the /proc/hello is read.
 * 
 * This function is called repeatedly until it returns 0, so
 * there must be logic that ensures it ultimately returns 0
 * once it has collected the data that is to go into the 
 * corresponding /proc file.
 *
 * params:
 *
 * file:
 * buf: buffer in user space
 * count:
 * pos:
 */
static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
	int rv = 0;
	char buffer[BUFFER_SIZE];
	static int completed = 0;
	struct pid *process = find_vpid((int)id);
	struct task_struct *task = pid_task(process, PIDTYPE_PID);
	if (task == NULL)
		return 0;

	if (completed)
	{
		completed = 0;
		return 0;
	}

	completed = 1;

	rv = sprintf(buffer, "command= %s pid=%ld state=%ld \n", task->comm, id, task->state);

	// copies the contents of buffer to userspace usr_buf
	copy_to_user(usr_buf, buffer, rv);

	return rv;
}

/* Macros for registering module entry and exit points. */
module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("process information");
MODULE_AUTHOR("SGG");