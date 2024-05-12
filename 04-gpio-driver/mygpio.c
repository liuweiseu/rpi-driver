#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/io.h>

#define GPIO_BASE_ADDR        0x1f000d0000
#define GPIO18_STATUS_OFFSET  (GPIO_BASE_ADDR + 0x090)
#define GPIO18_CTRL_OFFSET    (GPIO_BASE_ADDR + 0x094)

unsigned int *vir_gpio18_status;
unsigned int *vir_gpio18_ctrl;

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
  if(kbuf[0] == 1)
  {
    *vir_gpio18_ctrl |= 0x3 << 14;
    *vir_gpio18_ctrl |= 0x3 << 12;
    printk("output 1.\n");
  }
  else
  {
    *vir_gpio18_ctrl |= 0x3 << 14;
    *vir_gpio18_ctrl &= 0xffffcfff;
    printk("output 0.\n");
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

  // remap gpio18 status register
  vir_gpio18_status = ioremap(GPIO18_STATUS_OFFSET, 4);
  if (vir_gpio18_status == NULL) {
    printk("GPIO18_STATUS ioremap failed.\n");
    return EBUSY;
  } else {
    printk("GPIO18_STATUS ioremap successfully.\n");
    printk("vir_gpio18_status: 0x%08x\n", vir_gpio18_status);
  }
  // remap gpio18 ctrl register
  vir_gpio18_ctrl = ioremap(GPIO18_CTRL_OFFSET, 4);
  if (vir_gpio18_ctrl == NULL) {
    printk("GPIO18_CTRL ioremap failed.\n");
    return EBUSY;
  } else {
    printk("GPIO18_CTRL ioremap successfully.\n");
    printk("vir_gpio18_ctrl :0x%08x\n", vir_gpio18_ctrl);
  }
  return 0;

}

static void __exit misc_exit(void){
  misc_deregister(&misc_dev);
  iounmap(vir_gpio18_ctrl);
  iounmap(vir_gpio18_status);
  printk("misc deregistered successfully!\n");
}

module_init(misc_init);
module_exit(misc_exit);

MODULE_LICENSE("GPLv2");
MODULE_AUTHOR("Wei Liu");
MODULE_DESCRIPTION("This is a test for a simple misc device.");

