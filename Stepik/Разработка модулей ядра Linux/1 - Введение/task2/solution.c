#include <linux/module.h>
#include <linux/kernel.h>

#include "checker.h"

int init_module(void)
{
  printk(KERN_INFO "kernel_mooc init_module kyamran");
  call_me("Hello from my module!");
  return 0;
}

void cleanup_module(void)
{
  printk(KERN_INFO "kernel_mooc cleanup_module kyamran");
}

MODULE_LICENSE("GPL");
