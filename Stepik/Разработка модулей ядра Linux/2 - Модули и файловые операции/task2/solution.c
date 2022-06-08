#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/string.h>
#include <linux/sysfs.h>

static int sum = 0;

static int a = 0;
module_param(a, int, 0);

static int b = 0;
module_param(b, int, 0);

static int c_size = 0;
static int c[5] = { 0, 0, 0, 0, 0};
module_param_array(c, int, &c_size, 0);

static ssize_t my_sys_show(struct kobject* kobj, struct kobj_attribute* attr, char* buf)
{
  buf[0] = '\0';
  return sprintf(buf, "%d", sum);
}

static ssize_t my_sys_store(struct kobject* kobj, struct kobj_attribute* attr, const char* buf, size_t count)
{
	return 0;
}

static struct kobject* my_kobject;
static struct kobj_attribute my_sys_attribute = __ATTR(my_sys, 0664, my_sys_show, my_sys_store);

static int __init custom_init(void)
{
  int i = 0;
  int retval = 0;
  
  sum += a + b;
  for (i = 0; i < c_size; ++i)
  {
    sum += c[i];
  }
  
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
