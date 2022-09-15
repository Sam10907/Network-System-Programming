#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#define MAJOR_NUM 60
#define MODULE_NAME "DEMO"

static ssize_t drv_read(struct file *filp, char *buf, size_t count, loff_t *ppos)
{
    printk("device read\n");
    return count;
}
static ssize_t drv_write(struct file *filp, const char *buf, size_t count, loff_t *ppos)
{
    printk("device write\n");
    return count;
}
static int drv_open(struct inode *inode, struct file *filp)
{
    printk("device open\n");
    return 0;
}
static int drv_release(struct inode *inode, struct file *filp)
{
    printk("device close\n");
    return 0;
}
struct file_operations drv_fops =
{
    read: drv_read,
    write: drv_write,
    open: drv_open,
    release: drv_release,
};
static int demo_init(void) 
{
    if (register_chrdev(MAJOR_NUM, "demo", &drv_fops) < 0) {
        printk("<1> s: can't get major %s %d\n", MODULE_NAME, MAJOR_NUM);return (-
        EBUSY);
    }
    printk("<1> s: started %s\n", MODULE_NAME);
    return 0;
}
static void demo_exit(void) 
{
    unregister_chrdev(MAJOR_NUM, "demo");
    printk("<1> s: removed %s\n", MODULE_NAME);
}
module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL v2");