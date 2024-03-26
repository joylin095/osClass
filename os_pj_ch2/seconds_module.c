#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/param.h>

#define PROC_FILENAME "seconds"
#define BUFFER_SIZE 128

unsigned long start_time, now_time, elapsed_time;

struct proc_dir_entry *proc_entry;

ssize_t seconds_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

static struct proc_ops proc_ops = {
    .proc_read = seconds_read,
};

ssize_t seconds_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
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
    now_time = jiffies;
    elapsed_time = now_time - start_time;
    ret = sprintf(buffer, "Time Elapsed: %lu\n", elapsed_time / HZ);
    temp = copy_to_user(usr_buf, buffer, ret);
    return ret;
}

static int __init seconds_init(void)
{
    proc_entry = proc_create(PROC_FILENAME, 0666, NULL, &proc_ops);
    if (!proc_entry)
    {
        printk(KERN_ERR "Failed to create /proc/%s\n", PROC_FILENAME);
        return -ENOMEM;
    }
    start_time = jiffies;
    printk(KERN_INFO "/proc/%s created\n", PROC_FILENAME);
    return 0;
}

static void seconds_exit(void)
{
    remove_proc_entry(PROC_FILENAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_FILENAME);
}

module_init(seconds_init);
module_exit(seconds_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel module for /proc/seconds");
MODULE_AUTHOR("OsClass");