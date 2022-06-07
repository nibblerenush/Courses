#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>

#include "checker.h"

void* void_ptr = NULL;
void* int_array_ptr = NULL;
void* device_ptr = NULL;

static int __init custom_init(void)
{
  ssize_t void_size = get_void_size();
  void_ptr = kmalloc(void_size, GFP_KERNEL);
  submit_void_ptr(void_ptr);

  ssize_t int_array_size = get_int_array_size();
  int_array_ptr = kmalloc(int_array_size * sizeof(int), GFP_KERNEL);
  submit_int_array_ptr((int*)int_array_ptr);

  device_ptr = kmalloc(sizeof(struct device), GFP_KERNEL);
  submit_struct_ptr((struct device*)device_ptr);

  printk(KERN_INFO "kernel_mooc init_module kyamran");
  return 0;
}

static void __exit custom_exit(void)
{
  checker_kfree(void_ptr);
  checker_kfree(int_array_ptr);
  checker_kfree(device_ptr);
  printk(KERN_INFO "kernel_mooc cleanup_module kyamran");
}

module_init(custom_init);
module_exit(custom_exit);

MODULE_LICENSE("GPL");
