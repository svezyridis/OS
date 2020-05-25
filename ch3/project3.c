#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/list.h>

#define PROC_NAME "process list"

struct task_struct *task, *tsk;
struct list_head *list;
/* This function is called when the module is loaded. */

static int proc_init(void)
{
	printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
	/*for_each_process(task)
	{
		printk(KERN_INFO "%s %ld %d \n", task->comm, task->state, task->pid);
	}*/
	task = &init_task;
	list_for_each(list, &task->children)
	{
		tsk = list_entry(list, struct task_struct, sibling);
		printk(KERN_INFO "%s %ld %d \n", tsk->comm, tsk->state, tsk->pid);
	}
	return 0;
}

/* This function is called when the module is removed. */
static void proc_exit(void)
{

	printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/* Macros for registering module entry and exit points. */
module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("process information");
MODULE_AUTHOR("SGG");