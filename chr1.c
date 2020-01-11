#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/string.h>
#include <linux/unistd.h>

#define DEVICE_NAME "echo.dev"

MODULE_AUTHOR("x0r19x91");
MODULE_VERSION("1.0");
MODULE_LICENSE("GPL");

int major, open_count;

static int device_open(struct inode *, struct file *);
static int device_close(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static char msg[] = "[*] Hello from Kernel!\n";
static struct class* dev_class;
static struct device* dev;
static char buffer[0x1000];

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_close
};

static int __init
mod_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "[!] echo.dev: Error Registering Device!\n");
        return -EPERM;
    }

    dev_class = class_create(THIS_MODULE, "chrdev");
    if (IS_ERR(dev_class)) {
        printk(KERN_ALERT "[!] echo.dev: Error Creating Device Class!\n");
        unregister_chrdev(major, DEVICE_NAME);
        return PTR_ERR(dev_class);
    }

    dev = device_create(dev_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(dev)) {
        printk(KERN_ALERT "[!] echo.dev: Error Creating Device!\n");
        class_destroy(dev_class);
        unregister_chrdev(major, DEVICE_NAME);
        return PTR_ERR(dev);
    }
    strcpy(buffer, msg);
    return 0;
}

static void __exit
mod_exit(void)
{
    device_destroy(dev_class, MKDEV(major, 0));
    class_unregister(dev_class);
    class_destroy(dev_class);
    unregister_chrdev(major, DEVICE_NAME);
}

static int
device_open(struct inode* inode, struct file* file)
{
    if (open_count)
        return -EBUSY;
    open_count++;
    try_module_get(THIS_MODULE);
    return 0;
}

static int
device_close(struct inode *inode, struct file *file)
{
    if (open_count > 0)
        module_put(THIS_MODULE);
    open_count--;
    return 0;
}

static ssize_t
device_read(struct file* file, char* buf, size_t size, loff_t* off)
{
    static char* ptr = buffer;
    static loff_t pos = 0;

    if (! *ptr) {
        ptr = buffer;
        return 0;
    }

    while (size && *ptr) {
        size--;
        put_user(*ptr++, buf++);
        pos++;
    }

    return (ssize_t) pos;
}

static ssize_t
device_write(struct file *file, const char *buf, size_t len, loff_t * off)
{
    ssize_t min_len = 0xfff;

    if (len < min_len)
        min_len = len;

    copy_from_user(buffer, buf, min_len);
    buffer[min_len] = 0;

    return min_len;
}

module_init(mod_init);
module_exit(mod_exit);
