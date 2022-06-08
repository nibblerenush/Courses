#include <linux/module.h>
#include <linux/kernel.h>
#include "../checker.h"

EXPORT_SYMBOL(call_me);

int init_module(void)
{
  printk(KERN_INFO "init_module checker");
  call_me("call_me");
  return 0;
}

void cleanup_module(void)
{
  printk(KERN_INFO "cleanup_module checker");
}

void call_me(const char* message)
{
  printk(KERN_INFO "%s", message);
}

MODULE_LICENSE("GPL");
