#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE 128
#define PROC_NAME "pid"

static int current_pid;

static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);
static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos);

static struct proc_ops proc_ops = {
    .proc_read = proc_read,
    .proc_write = proc_write,
};

static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    struct task_struct *tsk = NULL;
    if (completed)
    {
        completed = 0;
        return 0;
    }
    tsk = pid_task(find_vpid(current_pid), PIDTYPE_PID);
    if (tsk)
    {
        rv = sprintf(buffer, "command = [%s], pid = [%d], state = [%d]\n", tsk->comm, current_pid, tsk->__state);
        completed = 1;
    }
    else
    {
        printk(KERN_INFO "Invalid PID %d!", current_pid);
        return 0;
    }
    if (copy_to_user(usr_buf, buffer, rv))
    {
        rv = -1;
    }
    return rv;
}

static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos)
{
    char *k_mem;
    int temp;
    k_mem = kmalloc(count, GFP_KERNEL);
    if (copy_from_user(k_mem, usr_buf, count))
    {
        printk(KERN_INFO "Error copying from user\n");
        return -1;
    }
    k_mem[count] = '\0';
    temp = kstrtoint(k_mem, 10, &current_pid);
    printk(KERN_INFO "Set current PID to %d", current_pid);
    kfree(k_mem);
    return count;
}

static int proc_init(void)
{
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

static void proc_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL"); // MIT is not allowed here, because this module calls pid_task(), which is GPL only
MODULE_DESCRIPTION("Linux Kernel Module for Task Information");
MODULE_AUTHOR("osClass");