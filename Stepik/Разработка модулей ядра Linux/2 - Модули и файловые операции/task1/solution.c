#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/sysfs.h>

static int counter = 0;

static ssize_t my_sys_show(struct kobject* kobj, struct kobj_attribute* attr, char* buf)
{
  ++counter;
  buf[0] = '\0';
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
  printk(KERN_INFO "kernel_mooc init_module kyamran");
  
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
    kobject_put(my_kobject);
    return 1;
  }
  
  return 0;
}

static void __exit custom_exit(void)
{
  printk(KERN_INFO "kernel_mooc cleanup_module kyamran");
  kobject_put(my_kobject);
}

module_init(custom_init);
module_exit(custom_exit);

MODULE_LICENSE("GPL");
