#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>
#include <linux/types.h>
#include <linux/module.h>

#define PROC_FILENAME "jiffies"
#define BUFFER_SIZE 128

struct proc_dir_entry *proc_entry;

ssize_t jiffies_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

static struct proc_ops proc_ops = {
    .proc_read = jiffies_read,
};

ssize_t jiffies_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    int ret, temp;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    if (completed)
    {
        completed = 0;
        return 0;
    }
    completed = 1;
    ret = sprintf(buffer, "Jiffies: %lu\n", jiffies);
    temp = copy_to_user(usr_buf, buffer, ret);
    return ret;
}

static int __init jiffies_init(void)
{
    proc_entry = proc_create(PROC_FILENAME, 0666, NULL, &proc_ops);
    if (!proc_entry)
    {
        printk(KERN_ERR "Failed to create /proc/%s\n", PROC_FILENAME);
        return -ENOMEM;
    }
    printk(KERN_INFO "/proc/%s created\n", PROC_FILENAME);
    return 0;
}

static void jiffies_exit(void)
{
    remove_proc_entry(PROC_FILENAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_FILENAME);
}

module_init(jiffies_init);
module_exit(jiffies_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel module for /proc/jiffies");
MODULE_AUTHOR("OsClass");