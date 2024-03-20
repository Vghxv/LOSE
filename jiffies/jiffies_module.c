#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>

#define PROC_FILENAME "jiffies"
#define BUFFER_SIZE 128

static struct proc_dir_entry *proc_entry;

static ssize_t jiffies_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
    char temp[BUFFER_SIZE];
    ssize_t len;
    
    len = snprintf(temp, sizeof(temp), "%lu\n", jiffies);
    
    if (*pos >= len)
        return 0;
    
    if (count > len - *pos)
        count = len - *pos;
    
    if (copy_to_user(buf, temp + *pos, count))
        return -EFAULT;
    
    *pos += count;
    
    return count;
}

static const struct proc_ops jiffies_fops = {
    .proc_read = jiffies_read,
};


int init_module()
{
    proc_entry = proc_create(PROC_FILENAME, 0, NULL, &jiffies_fops);
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
MODULE_DESCRIPTION("Report the current value of jiffies when the /proc/jiffies file is read.");
MODULE_AUTHOR("LOSE");