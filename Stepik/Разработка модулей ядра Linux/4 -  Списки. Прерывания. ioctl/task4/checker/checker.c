#include <linux/module.h>
#include <linux/kernel.h>
#include "../checker.h"

EXPORT_SYMBOL(check_timer);

int init_module(void)
{
  printk(KERN_INFO "init_module checker");
  return 0;
}

void cleanup_module(void)
{
  printk(KERN_INFO "cleanup_module checker");
}

void check_timer(void)
{
  printk(KERN_INFO "check_timer");
}

MODULE_LICENSE("GPL");
