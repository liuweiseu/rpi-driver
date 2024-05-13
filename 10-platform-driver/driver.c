#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/device.h>

int gpio_probe(struct platform_device *pdev)
{
  printk("gpio probe.\n");
  return 0;
}

int gpio_remove(struct platform_device *pdev)
{
  printk("gpio remove.\n");
  return 0;
}

struct platform_driver pf_driver = {
	.probe = gpio_probe,
	.remove = gpio_remove,
  .driver = {
    .name = "gpio_test",
    .owner = THIS_MODULE
  }
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
}

module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wei Liu");
MODULE_DESCRIPTION("This is for test.");

