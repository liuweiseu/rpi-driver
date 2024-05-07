#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

struct file_operations misc_fops = {
  .owner = THIS_MODULE
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

