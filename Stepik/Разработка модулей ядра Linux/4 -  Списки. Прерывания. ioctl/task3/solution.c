#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <asm/irq.h>

static int dev_id = 0;
static int counter = 0;
static const int irq_num = 8;

static irqreturn_t handler(int irq, void* dev_id)
{
  ++counter;
  return IRQ_HANDLED;
}

static ssize_t my_sys_show(struct kobject* kobj, struct kobj_attribute* attr, char* buf)
{
  return sprintf(buf, "%d", counter);
}

static ssize_t my_sys_store(struct kobject* kobj, struct kobj_attribute* attr, const char* buf, size_t count)
{
  return 0;
}

static struct kobject* my_kobject;
static struct kobj_attribute my_sys_attribute = __ATTR(my_sys, 0664, my_sys_show, my_sys_store);

static int __init custom_init(void)
{
  int retval = 0;
  
  my_kobject = kobject_create_and_add("my_kobject", kernel_kobj);
  if (!my_kobject)
  {
    printk(KERN_ERR "kernel_mooc error kobject_create_and_add");
    return 1;
  }

  retval = sysfs_create_file(my_kobject, &my_sys_attribute.attr);
  if (retval)
  {
    printk(KERN_ERR "kernel_mooc error sysfs_create_file");
    return 1;
  }

  retval = request_irq(irq_num, &handler, IRQF_SHARED, "rtc1", &dev_id);
  if (retval)
  {
    printk(KERN_ERR "kernel_mooc error request_irq");
    return 1;
  }

  printk(KERN_INFO "kernel_mooc init_module kyamran");
  return 0;
}

static void __exit custom_exit(void)
{
  printk(KERN_INFO "kernel_mooc cleanup_module kyamran");
  free_irq(irq_num, &dev_id);
  kobject_put(my_kobject);
}

module_init(custom_init);
module_exit(custom_exit);

MODULE_LICENSE("GPL");
