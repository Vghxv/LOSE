#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/uaccess.h>

#define PROC_FILENAME "task_list_dfs"
#define BUFFER_SIZE 128

void dfs(struct task_struct *task, int depth) {
    struct list_head *list;
    struct task_struct *child;

    printk("command = [%s], pid = [%d], state = [%d], depth = [%d]\n", task->comm, task->pid, task->__state, depth);
    list_for_each(list, &task->children) {
        child = list_entry(list, struct task_struct, sibling);
        dfs(child, depth + 1);
    }

    return;
}


int init_module()
{
    struct task_struct *task;
    printk(KERN_INFO "/proc/%s created\n", PROC_FILENAME);
    task = &init_task;
    dfs(task, 0);   
    printk("command = [%s], pid = [%d], state = [%d]\n", task->comm, task->pid, task->__state);
    return 0;
}

void cleanup_module()
{
    printk(KERN_INFO "/proc/%s removed\n", PROC_FILENAME);
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Task List DFS Module");
MODULE_AUTHOR("LOSE");
