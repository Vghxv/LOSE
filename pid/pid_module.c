#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/uaccess.h>
#include <linux/seq_file.h>
#include <linux/slab.h> // for kmalloc and kfree

#define PROC_FILENAME "pid"

static struct proc_dir_entry *proc_entry;

static int current_pid = 0;

// this function returns the state of the task
static ssize_t pid_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos){
    char *k_mem;
    k_mem = kmalloc(count, GFP_KERNEL);
    if (copy_from_user(k_mem, usr_buf, count)) {
        printk(KERN_INFO "Error copying from user\n");
        return -EFAULT;
    }
    k_mem[count] = '\0';
    
    if(kstrtoint(k_mem, 10, &current_pid)) {
        printk(KERN_INFO "Error converting string to int\n");
        return -EFAULT;
    }
    printk(KERN_INFO "Set current PID to %d", current_pid);
    kfree(k_mem);
    return count;
}

// this function logs the command, pid and state of the task
static int task_report(struct seq_file *m , void *v)
{
    struct task_struct *task;
    task = get_pid_task(find_vpid(current_pid), PIDTYPE_PID);
    if (task == NULL) {
        printk(KERN_INFO "No task found with PID %d\n", current_pid);
        return 0;
    }
    seq_printf(m, "command = [%s], pid = [%d], state = [%d]\n", task->comm, task->pid, task_state_to_char(task));
    return 0;
}

// this function is called when the /proc file is opened
static int pid_open(struct inode *inode, struct file *file)
{    
    return single_open(file, task_report, NULL);
}

// __init macro tells the kernel that this function is only used at initialization time
static int __init custom_module_init(void) 
{
    // put it inside the init_module function to limit the scope of the struct proc_ops  
    static const struct proc_ops pid_fops = {
        .proc_open = pid_open,
        .proc_read = seq_read,
        .proc_lseek = seq_lseek,
        .proc_release = single_release,
        .proc_write = pid_write
    };

    proc_entry = proc_create(PROC_FILENAME, 0, NULL, &pid_fops);
    if (!proc_entry) {
        printk(KERN_ERR "Failed to create /proc/%s\n", PROC_FILENAME);
        return -ENOMEM;
    }
    printk(KERN_INFO "/proc/%s created\n", PROC_FILENAME);
    
    return 0;
}

// __exit macro tells the kernel that this function is only used at exit time
static void __exit custom_module_exit(void) 
{
    if (proc_entry) {
        remove_proc_entry(PROC_FILENAME, NULL);
        printk(KERN_INFO "/proc/%s removed\n", PROC_FILENAME);
    }
}


module_init(custom_module_init);
module_exit(custom_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("PID Module");
MODULE_AUTHOR("😎🥳");