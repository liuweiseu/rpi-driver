#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>

#include <linux/cdev.h>
#include <linux/device.h>

#define DEVICE_CLASS_NAME "chrdev_class"
#define DEVICE_NAME "chrdev"
#define DEVICE_SNAME "schrdev"
#define DEVICE_ANAME "achrdev"
#define DEVICE_NUM  1
static int major_num,minor_num;

int chrdev_open(struct inode *inode, struct file *file){
  printk("chrdev is open. \n");
  return 0;
}

struct cdev cdev;
struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = chrdev_open
};

struct class *cls;
struct device *devfp;

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
  
  // init cdev
  cdev.owner = THIS_MODULE;
  cdev_init(&cdev, &fops);
  cdev_add(&cdev, dev, DEVICE_NUM);
  cls = class_create(DEVICE_CLASS_NAME);
  devfp = device_create(cls, NULL, dev, NULL, DEVICE_NAME);

  return 0;
}

static __exit void hello_exit(void)
{
  unregister_chrdev_region(MKDEV(major_num, minor_num), DEVICE_NUM);
  cdev_del(&cdev);
  device_destroy(cls, MKDEV(major_num, minor_num));
  class_destroy(cls);
  printk("char dev unregistered successfully!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wei Liu");
MODULE_DESCRIPTION("This is for test.");

