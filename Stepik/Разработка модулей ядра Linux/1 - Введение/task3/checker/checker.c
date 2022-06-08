#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>

#include "../checker.h"

EXPORT_SYMBOL(array_sum);
EXPORT_SYMBOL(generate_output);

int init_module(void)
{
  printk(KERN_INFO "init_module checker");
  return 0;
}

void cleanup_module(void)
{
  printk(KERN_INFO "cleanup_module checker");
}

int array_sum(short *arr, size_t n)
{
  int sum = 0;
  size_t i = 0;
  for (i = 0; i < n; ++i)
  {
    sum += arr[i];
  }

  return sum;
}

ssize_t generate_output(int sum, short *arr, size_t size, char *buf)
{
  size_t i = 0;
  sprintf(buf, "%d ", sum);
  for (i = 0; i < size; ++i)
  {
    char number [16];
    sprintf(number, "%d ", arr[i]);
    strcat(buf, number);
  }

  return strlen(buf);
}

MODULE_LICENSE("GPL");
