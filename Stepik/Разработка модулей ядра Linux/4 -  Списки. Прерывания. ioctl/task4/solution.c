#include <linux/hrtimer.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

#include "checker.h"

static int delays_size = 0;
static unsigned long delays[256];
module_param_array(delays, ulong, &delays_size, 0);
static int index = 0;

static struct hrtimer global_timer;
static ktime_t ktime;

static enum hrtimer_restart hrtimer_hander(struct hrtimer *timer)
{
  printk(KERN_INFO "kernel_mooc hrtimer_hander");
  check_timer();
  ++index;
  
  if (index < delays_size)
  {
    ktime = ktime_set(0, delays[index] * 1000000);
    hrtimer_forward_now(timer, ktime);
    return HRTIMER_RESTART;
  }
  
  return HRTIMER_NORESTART;
}

int init_module(void)
{
  printk(KERN_INFO "kernel_mooc init_module kyamran");
  
  hrtimer_init(&global_timer, CLOCK_REALTIME, HRTIMER_MODE_REL);
  global_timer.function = hrtimer_hander;

  if (index < delays_size)
  {
    check_timer();
    ktime = ktime_set(0, delays[index] * 1000000);
    hrtimer_start(&global_timer, ktime, HRTIMER_MODE_REL);
  }
  
  return 0;
}

void cleanup_module(void)
{
  printk(KERN_INFO "kernel_mooc cleanup_module kyamran");
  hrtimer_cancel(&global_timer);
}

MODULE_LICENSE("GPL");
