#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/list.h>

struct number_node {
    int white;
    int red;
    int green;
    int blue;
    struct list_head list;
};

// assgin color to a node
void AppendNode(struct list_head *list, int white, int red, int green, int blue){
    struct number_node* new_node = kmalloc(sizeof(struct number_node), GFP_KERNEL);
    new_node->white = white;
    new_node->red = red;
    new_node->green = green;
    new_node->blue = blue;
    list_add_tail(&new_node->list, list);
}

static LIST_HEAD(color_list);

static int __init custom_module_init(void)
{
    // create a new node list and allocate memory for it
    struct number_node *ptr;
    AppendNode(&color_list, 255, 0, 0, 0);
    AppendNode(&color_list, 0, 255, 0, 0);
    AppendNode(&color_list, 0, 0, 255, 0);
    printk(KERN_INFO "module loaded\n");
    printk(KERN_INFO "Color List:\n");
    list_for_each_entry(ptr, &color_list, list)
    {
        printk(KERN_CONT "White: %d, Red: %d, Green: %d, Blue: %d\n", ptr->white, ptr->red, ptr->green, ptr->blue);
    }
    return 0;
}

void __exit custom_module_exit(void)
{
    struct color_node *ptr, *next;
    list_for_each_entry_safe(ptr, next, &color_list, list)
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