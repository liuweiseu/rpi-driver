#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

void gpio_release(struct device *dev)
{
  printk("gpio release.\n");
}

struct resource gpio_resource[] = {
  {
    .start  = 0x1f000d0000 + 0x094,
    .end    = 0x1f000d0000 + 0x098,
    .flags  = IORESOURCE_MEM,
    .name   = "gpio18_ctrl"
  }
};

struct platform_device gpio_device = {
	.name = "gpio_test",
	.id = -1,
	.num_resources = ARRAY_SIZE(gpio_resource),
	.resource = gpio_resource,
  .dev = {
    .release = gpio_release
  }
};

static __init int device_init(void)
{
  printk("device init.\n");
  return platform_device_register(&gpio_device);
}

static __exit void device_exit(void)
{
  printk("Byebye!\n");
  platform_device_unregister(&gpio_device);
}

module_init(device_init);
module_exit(device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wei Liu");
MODULE_DESCRIPTION("This is for test.");

