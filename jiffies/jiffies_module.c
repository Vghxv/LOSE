#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/seq_file.h>

#define PROC_JIFFIES "jiffies"
#define PROC_SECS "seconds"

static struct proc_dir_entry *proc_entry_jiffies;
static struct proc_dir_entry *proc_entry_secs;
static unsigned long start_jiffies;

// this function logs the current value of jiffies to the kernel log
static int print_jiffies(struct seq_file *m, void *v)
{
    seq_printf(m, "current value of jiffies = %lu\n", jiffies);
    return 0;
}

// this function is called when the /proc/jiffies file is read
static int jiffies_open(struct inode *inode, struct file *file)
{
    return single_open(file, print_jiffies, NULL);
}


static int print_seconds(struct seq_file *m, void *v)
{
    seq_printf(m, "current value of seconds since the module was loaded = %lu\n", (jiffies - start_jiffies) / HZ);
    return 0;
}

// this function is called when the /proc/seconds file is read
static int seconds_open(struct inode *inode, struct file *file)
{
    return single_open(file, print_seconds, NULL);
}

// __init macro tells the kernel that this function is only used at initialization time
static int __init custom_module_init(void) 
{
    // put it inside the init_module function to limit the scope of the struct proc_ops  
    static const struct proc_ops jiffies_fops = {
        .proc_open = jiffies_open,
        .proc_read = seq_read,
        .proc_lseek = seq_lseek,
        .proc_release = single_release,
    };

    static const struct proc_ops seconds_fops = {
        .proc_open = seconds_open,
        .proc_read = seq_read,
        .proc_lseek = seq_lseek,
        .proc_release = single_release,
    };

    start_jiffies = jiffies;
    proc_entry_jiffies = proc_create(PROC_JIFFIES, 0, NULL, &jiffies_fops);
    if (!proc_entry_jiffies) {
        printk(KERN_ERR "Failed to create /proc/%s\n", PROC_JIFFIES);
        return -ENOMEM;
    }
    printk(KERN_INFO "/proc/%s created\n", PROC_JIFFIES);

    proc_entry_secs = proc_create(PROC_SECS, 0, NULL, &seconds_fops);
    if (!proc_entry_secs) {
        printk(KERN_ERR "Failed to create /proc/%s\n", PROC_SECS);
        return -ENOMEM;
    }
    printk(KERN_INFO "/proc/%s created\n", PROC_SECS);
    
    return 0;
}

// __exit macro tells the kernel that this function is only used at exit time
static void __exit custom_module_exit(void) 
{
    if (proc_entry_jiffies) {
        remove_proc_entry(PROC_JIFFIES, NULL);
        printk(KERN_INFO "/proc/%s removed\n", PROC_JIFFIES);
    }

    if (proc_entry_secs) {
        remove_proc_entry(PROC_SECS, NULL);
        printk(KERN_INFO "/proc/%s removed\n", PROC_SECS);
    }
}

module_init(custom_module_init);
module_exit(custom_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Jiffies Module");
MODULE_AUTHOR("😎");