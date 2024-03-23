#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

#define PROC_FILENAME "task_list_dfs"

void print_with_depth_color(const char *command, int pid, int state, int depth){
    const char *color_code;

    switch (depth % 7) {
        case 0:
            color_code = "\033[31m"; // Red
            break;
        case 1:
            color_code = "\033[32m"; // Green
            break;
        case 2:
            color_code = "\033[33m"; // Yellow
            break;
        case 3:
            color_code = "\033[34m"; // Blue
            break;
        case 4:
            color_code = "\033[35m"; // Magenta
            break;
        case 5:
            color_code = "\033[36m"; // Cyan
            break;
        case 6:
            color_code = "\033[37m"; // White
            break;
        default:
            color_code = "\033[0m"; // Default color
    }

    printk("[%scommand = [%s], pid = [%d], state = [%d], depth = [%d]\033[0m]\n", color_code, command, pid, state, depth);
}

void dfs(struct task_struct *task, int depth) {
    struct list_head *list;
    struct task_struct *child;

    print_with_depth_color(task->comm, task->pid, task_state_to_char(task), depth);
    list_for_each(list, &task->children) {
        child = list_entry(list, struct task_struct, sibling);
        dfs(child, depth + 1);
    }
    return;
}

static int __init custom_module_init(void)
{
    struct task_struct *task;
    printk(KERN_INFO "module loaded\n");
    task = &init_task;
    dfs(task, 0);
    printk("command = [%s], pid = [%d], state = [%d]\n", task->comm, task->pid, task->__state);
    return 0;
}

static void __exit custom_module_exit(void)
{
    printk(KERN_INFO "module removed\n");
}

module_init(custom_module_init);
module_exit(custom_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Task List DFS Module");
MODULE_AUTHOR("😎🥳😑🙂");
