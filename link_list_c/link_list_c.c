#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/list.h>

struct number_node {
    int number;
    struct list_head list;
};

static int NUM = 100;
module_param(NUM, int, 0644);

static LIST_HEAD(Collatz_Conjecture_list);
// assgin color to a node
void AppendNode(struct list_head *list, int number){
    struct number_node* new_node = kmalloc(sizeof(struct number_node), GFP_KERNEL);
    new_node->number = number;
    list_add_tail(&new_node->list, list);
}

void CollatzConjecture(int n){

    while( n != 1 ) {
        AppendNode(&Collatz_Conjecture_list, n);
        if ( n % 2 == 0 ) {
            n /= 2;
        } 
        else {
            n = 3 * n + 1;
        }
    }
    AppendNode(&Collatz_Conjecture_list, 1);
}
static int __init custom_module_init(void)
{
    struct number_node *ptr;
    printk(KERN_INFO "module loaded\n");
    printk(KERN_INFO "Collatz Conjecture List:\n");
    CollatzConjecture(NUM);
    list_for_each_entry(ptr, &Collatz_Conjecture_list, list)
    {
        printk(KERN_CONT "%d ", ptr->number);
    }
    printk(KERN_CONT "\n");
    return 0;
}

void __exit custom_module_exit(void)
{
    struct number_node *ptr, *next;
    list_for_each_entry_safe(ptr, next, &Collatz_Conjecture_list, list)
    {
        list_del(&ptr->list);
        kfree(ptr);
    }
    printk( KERN_INFO "module removed\n");
}

module_init(custom_module_init);
module_exit(custom_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Link List Module");
MODULE_AUTHOR("😎🙂");