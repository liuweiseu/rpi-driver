#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

int misc_open(struct inode *inode, struct file *file){
  printk("Hello, misc is open. \n");
  return 0;
}

int misc_release(struct inode *inode, struct file *file){
  printk("Bye, misc is close.\n");
  return 0;
}

ssize_t misc_read(struct file *file, char __user *ubuf, size_t size, loff_t *loff_t)
{
  //char kbuf[64] = {0};
  //strcpy(kbuf, "Hello, this is the data from kernel buf.");
  char kbuf[64] = "Hello, this is data from kernel buf.";
  if(copy_to_user(ubuf, kbuf, strlen(kbuf)))
  {
     printk("read error.\n");
     return -1;
  }
  printk("msic read.\n");
  return 0;
}

ssize_t misc_write(struct file *file, const char __user *ubuf, size_t size, loff_t *loff_t)
{
  char kbuf[64] = {0};
  if (copy_from_user(kbuf, ubuf, size))
  {
    printk("write error.\n");
    return -1;
  }
  printk("misc write.\n");
  printk("data from user: %s\n", kbuf);
  return 0;
}


struct file_operations misc_fops = {
  .owner = THIS_MODULE,
  .open = misc_open,
  .release = misc_release,
  .read = misc_read,
  .write = misc_write
};

struct miscdevice misc_dev = {
  .minor = MISC_DYNAMIC_MINOR,
  .name = "hello_misc",
  .fops = &misc_fops
};

static int __init misc_init(void){
  int ret;
  ret = misc_register(&misc_dev);
  if(ret < 0)
  {
    printk("misc registeration failed!\n");
  }
  printk("misc registered successfully!\n");
  return 0;
}

static void __exit misc_exit(void){
  misc_deregister(&misc_dev);
  printk("misc deregistered successfully!\n");
}

module_init(misc_init);
module_exit(misc_exit);

MODULE_LICENSE("GPLv2");
MODULE_AUTHOR("Wei Liu");
MODULE_DESCRIPTION("This is a test for a simple misc device.");

