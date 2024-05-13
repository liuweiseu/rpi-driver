#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/mod_devicetable.h>
#include <linux/ioport.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/io.h>

//#define REQUEST_MEM 

struct resource *gpio_resource;
struct resource *gpio_resource_tmp;

unsigned int *virtual_addr;

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
  .name = "mymisc",
  .fops = &misc_fops
};

int gpio_probe(struct platform_device *pdev)
{
  int ret = 0;
  printk("gpio probe.\n");
  // method 1
  printk("device name: %s\n", pdev->resource[0].name);
  // method 2
  gpio_resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
  if(gpio_resource == NULL) {
    printk("platform get resource failed.\n");
    return -EBUSY;
  }
  printk("start: 0x%llx\n", gpio_resource->start);
#ifdef REQUEST_MEM
  gpio_resource_tmp = request_mem_region(gpio_resource->start, gpio_resource->end - gpio_resource->start + 1, "gpio18_ctrl");
  if(gpio_resource_tmp == NULL) {
    printk("request mem region failed.\n");
    goto err;
  }
#endif

  // get virtual address
  virtual_addr = ioremap(gpio_resource->start, gpio_resource->end - gpio_resource->start + 1);
  if (virtual_addr == NULL) {
    printk("ioremap failed.\n");
    return -EBUSY;
  }

  //register misc device
  ret = misc_register(&misc_dev);
  if (ret < 0) {
    printk("misc register failed.\n");
    return ret;
  }
  return 0;

#ifdef REQUEST_MEM
err:
  release_mem_region(gpio_resource->start, gpio_resource->end - gpio_resource->start + 1);
  return -EBUSY;
#endif
}

int gpio_remove(struct platform_device *pdev)
{
  printk("gpio remove.\n");
  return 0;
}

struct platform_device_id pf_id_table = {
  .name = "gpio_test"
};

struct platform_driver pf_driver = {
	.probe = gpio_probe,
	.remove = gpio_remove,
  .driver = {
    .name = "gpio_test_inv",
    .owner = THIS_MODULE
  },
  .id_table = &pf_id_table
};

static __init int gpio_driver_init(void)
{
  printk("driver init.\n");
  int ret;
  ret = platform_driver_register(&pf_driver);
  if (ret < 0) {
    printk("platform driver register failed.\n");
    return ret;
  }
  return 0;
}

static __exit void gpio_driver_exit(void)
{
  printk("Byebye!\n");
  platform_driver_unregister(&pf_driver);
  misc_deregister(&misc_dev);
  iounmap(virtual_addr);
}

module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wei Liu");
MODULE_DESCRIPTION("This is for test.");

