#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>
#include <linux/time.h>

#define PROC_FILENAME "seconds"
#define BUFFER_SIZE 128

static struct proc_dir_entry *proc_entry;
static unsigned long start_jiffies;

static ssize_t seconds_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
    unsigned long elapsed_seconds;
    unsigned long elapsed_jiffies = jiffies - start_jiffies;
    unsigned long seconds;
    char temp[BUFFER_SIZE];
    ssize_t len;

    elapsed_seconds = elapsed_jiffies / HZ;

    seconds = jiffies_to_msecs(elapsed_jiffies) / 1000;

    len = snprintf(temp, sizeof(temp), "%lu\n", seconds);

    if (*pos >= len)
        return 0;
    
    if (count > len - *pos)
        count = len - *pos;
    
    if (copy_to_user(buf, temp + *pos, count))
        return -EFAULT;
    
    *pos += count;
    
    return count;

}

static const struct proc_ops seconds_fops = {
    .proc_read = seconds_read,
};

int init_module()
{
    proc_entry = proc_create(PROC_FILENAME, 0, NULL, &seconds_fops);
    if (!proc_entry) {
        printk(KERN_ERR "Failed to create /proc/%s\n", PROC_FILENAME);
        return -ENOMEM;
    }

    start_jiffies = jiffies;

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
MODULE_DESCRIPTION("Report the number of seconds since the /proc/seconds file was read.");
MODULE_AUTHOR("LOSE");
