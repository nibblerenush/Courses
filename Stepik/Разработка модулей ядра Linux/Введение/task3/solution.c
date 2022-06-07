#include <linux/kernel.h>
#include <linux/module.h>

#include "checker.h"

static int __init custom_init(void)
{
  CHECKER_MACRO;
  printk(KERN_INFO "kernel_mooc init_module kyamran");

  const size_t arr_size = 5;
  short arr[] = {1, 2, 3, 4, 5};
  const int correct_sum = 15;
  
  int i = 0;
  const int count = 15;
  for (i = 0; i < count; ++i)
  {
    char buf [256];
    buf[0] = '\0';

    int sum = array_sum(arr, arr_size);
    ssize_t buf_size = generate_output(sum, arr, arr_size, buf);
    buf[buf_size + 1] = '\0';

    if (sum == correct_sum)
    {  
      printk(KERN_INFO "%s", buf);
    }
    else
    {
      printk(KERN_ERR "%s", buf);
    }
  }

  return 0;
}

static void __exit custom_exit(void)
{
  CHECKER_MACRO;
  printk(KERN_INFO "kernel_mooc cleanup_module kyamran");
}

module_init(custom_init);
module_exit(custom_exit);

MODULE_LICENSE("GPL");
