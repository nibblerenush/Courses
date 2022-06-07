#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>

#include "../checker.h"

EXPORT_SYMBOL(get_void_size);
EXPORT_SYMBOL(submit_void_ptr);
EXPORT_SYMBOL(get_int_array_size);
EXPORT_SYMBOL(submit_int_array_ptr);
EXPORT_SYMBOL(submit_struct_ptr);
EXPORT_SYMBOL(checker_kfree);

int init_module(void)
{
  printk(KERN_INFO "init_module checker");
  return 0;
}

void cleanup_module(void)
{
  printk(KERN_INFO "cleanup_module checker");
}

ssize_t get_void_size(void)
{
  return 16;
}

void submit_void_ptr(void *p)
{
  (void)p;
}

ssize_t get_int_array_size(void)
{
  return 8;
}

void submit_int_array_ptr(int *p)
{
  (void)p;
}

void submit_struct_ptr(struct device *p)
{
  (void)p;
}

void checker_kfree(void *p)
{
  kfree(p);
}

MODULE_LICENSE("GPL");
