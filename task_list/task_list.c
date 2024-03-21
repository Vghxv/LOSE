#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/uaccess.h>

// for kmalloc and kfree
// #include <linux/slab.h>

#define PROC_FILENAME "task_list"
#define BUFFER_SIZE 128

int init_module()
{
    struct task_struct *task;

    printk(KERN_INFO "/proc/%s created\n", PROC_FILENAME);
 
    for_each_process(task) {
        printk("command = [%s], pid = [%d], state = [%d]\n", task->comm, task->pid, task->__state);
    }

    return 0;
}

void cleanup_module()
{
    printk(KERN_INFO "/proc/%s removed\n", PROC_FILENAME);
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Task List Module");
MODULE_AUTHOR("LOSE");
