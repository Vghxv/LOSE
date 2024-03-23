#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>

#define PROC_FILENAME "task_list"

static int __init custom_module_init(void)
{
    struct task_struct *task;
    printk( KERN_INFO "module loaded\n");
    for_each_process(task) {
        printk("command = [%s], pid = [%d], state = [%d]\n", task->comm, task->pid, task_state_to_char(task));
    }
    return 0;
}

void __exit custom_module_exit(void)
{
    printk( KERN_INFO "module removed\n");
}

module_init(custom_module_init);
module_exit(custom_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Task List Module");
MODULE_AUTHOR("😎🥳😑🙂");
