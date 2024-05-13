#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>

#define DEVICE_SNAME "schrdev"
#define DEVICE_ANAME "achrdev"
#define DEVICE_NUM  1
static int major_num,minor_num;

module_param(major_num, int, S_IRUSR);
module_param(minor_num, int, S_IRUSR);

static __init int hello_init(void)
{
  printk("char dev register init.\n");
  dev_t dev;
  int ret;
  if(major_num) {
    printk("use static registration method.\n");
    dev = MKDEV(major_num, minor_num);
    ret = register_chrdev_region(dev, DEVICE_NUM, DEVICE_SNAME);
    if(ret < 0) {
      printk("register_chrdev_region failed.\n");
      return ret;
    }
  }
  else {
    printk("use dynamic registration method.\n");
    ret = alloc_chrdev_region(&dev, 0, DEVICE_NUM, DEVICE_ANAME);
    if(ret < 0) {
      printk("alloc_chrdev_region failed.\n");
      return ret;
    }
    major_num = MAJOR(dev);
    minor_num = MINOR(dev);
  }
  printk("char dev registered successfully!\n");
  printk("major_num: %d, minor_num: %d\n", major_num, minor_num);
  
  return 0;
}

static __exit void hello_exit(void)
{
  unregister_chrdev_region(MKDEV(major_num, minor_num), DEVICE_NUM);
  printk("char dev unregistered successfully!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPLv2");
MODULE_AUTHOR("Wei Liu");
MODULE_DESCRIPTION("This is for test.");

