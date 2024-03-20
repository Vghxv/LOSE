#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/uaccess.h>
#include <linux/slab.h> // for kmalloc and kfree

#define PROC_FILENAME "pid"
#define BUFFER_SIZE 128

static ssize_t proc_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    struct task_struct *tsk = NULL;
    if (completed) {
        completed = 0;
        return 0;
    }
    tsk = pid_task(find_vpid(current_pid), PIDTYPE_PID);
    if(tsk) {
        // long state = tsk->__state;

        rv = snprintf(buffer, BUFFER_SIZE,
                      "command = [%s], pid = [%d], state=[%d]\n",
                      tsk->comm, current_pid, tsk->__state);
    }
    else {
        printk(KERN_INFO "Invalid PID %d!", current_pid);
        return 0;
    }
    completed = 1;
    if (copy_to_user(buf, buffer, rv)) {
        rv = -1;
    }
    return rv;
}

static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos){
    char *k_mem;
    k_mem = kmalloc(count, GFP_KERNEL);
    if (copy_from_user(k_mem, usr_buf, count)) {
        printk(KERN_INFO "Error copying from user\n");
        return -1;
    }
    k_mem[count] = '\0';
    
    if(kstrtoint(k_mem, 10, &current_pid)) {
        printk(KERN_INFO "Error converting string to int\n");
        return -1;
    }
    printk(KERN_INFO "Set current PID to %d", current_pid);
    kfree(k_mem);
    return count;
}

static const struct proc_ops task_info_fops = {
    .proc_read = proc_read,
    .proc_write = proc_write,
};

int init_module()
{
    proc_entry = proc_create(PROC_FILENAME, 0, NULL, &task_info_fops);
    if (!proc_entry) {
        printk(KERN_ERR "Failed to create /proc/%s\n", PROC_FILENAME);
        return -ENOMEM;
    }

    printk(KERN_INFO "/proc/%s created\n", PROC_FILENAME);
    return 0;
}

void cleanup_module()
{
    if (proc_entry) {
        remove_proc_entry(PROC_FILENAME, NULL);
        printk(KERN_INFO "/proc/%s removed\n", PROC_FILENAME);
    }
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Report the process information of a given PID");
MODULE_AUTHOR("LOSE");
