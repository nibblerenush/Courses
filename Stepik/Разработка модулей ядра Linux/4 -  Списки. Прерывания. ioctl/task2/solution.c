#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/uaccess.h>

#define IOC_MAGIC 'k'

#define SUM_LENGTH _IOWR(IOC_MAGIC, 1, char*)
#define SUM_CONTENT _IOWR(IOC_MAGIC, 2, char*)

#define MYDEV_NAME "solution_node"
#define KBUF_SIZE 20

static dev_t first;
static unsigned int count = 1;
static int my_major = 240;
static int my_minor = 0;
static struct cdev* my_cdev;

static int str_len_sum = 0;
static int number_sum = 0;

char kbuf[KBUF_SIZE + 1];
char* kbufptr = NULL;

static int solution_node_open(struct inode* sol_inode, struct file* sol_file)
{
  printk(KERN_INFO "kernel_mooc solution_node_open");
  kbufptr = kbuf;
  memset(kbufptr, 0, KBUF_SIZE + 1);
  return 0;
}

static int solution_node_release(struct inode* sol_inode, struct file* sol_file)
{
  printk(KERN_INFO "kernel_mooc solution_node_release");
  return 0;
}

static ssize_t solution_node_read(struct file* sol_file, char __user* buf, size_t buf_size, loff_t* ppos)
{
  int bytes_read = 0;
  printk(KERN_INFO "kernel_mooc solution_node_read");
  
  kbuf[0] = '\0';
  sprintf(kbuf, "%d %d\n", str_len_sum, number_sum);
    
  while (buf_size && *kbufptr)
  {
    put_user(*(kbufptr++), buf++);
    buf_size--;
    bytes_read++;
  }
  
  return bytes_read;
}

static long solution_node_unlocked_ioctl(struct file *sol_file, unsigned int command, unsigned long args)
{
  int i = 0;
  char ch = 0;
  long number = 0;
  int retval = 0;
  
  printk(KERN_INFO "kernel_mooc solution_node_unlocked_ioctl");
  
  get_user(ch, (char*)args);
  while (ch != 0 && i < KBUF_SIZE)
  {
    kbuf[i] = ch;
    i++;
    get_user(ch, (char*)args + i);
  }
  kbuf[i] = 0;
  printk(KERN_INFO "kernel_mooc kbuf = %s", kbuf);

  switch (command)
  {
  case SUM_LENGTH:
    str_len_sum += strlen(kbuf);
    return str_len_sum;
  case SUM_CONTENT:
    retval = kstrtol(kbuf, 10, &number);
    if (retval)
    {
      return retval;
    }
    number_sum += number;
    return number_sum;
  }

  return -EINVAL;
}

static const struct file_operations mycdev_fops =
{
  .owner = THIS_MODULE,
  .open = solution_node_open,
  .release = solution_node_release,
  .read = solution_node_read,
  .unlocked_ioctl = solution_node_unlocked_ioctl
};

static int __init custom_init(void)
{
  printk(KERN_INFO "kernel_mooc init_module kyamran");
  
  first = MKDEV(my_major, my_minor);
  register_chrdev_region(first, count, MYDEV_NAME);
  
  my_cdev = cdev_alloc();
  cdev_init(my_cdev, &mycdev_fops);
  cdev_add(my_cdev, first, count);

  return 0;
}

static void __exit custom_exit(void)
{
  printk(KERN_INFO "kernel_mooc cleanup_module kyamran");
  
  if (my_cdev)
  {
    cdev_del(my_cdev);
  }

  unregister_chrdev_region(first, count);
}

module_init(custom_init);
module_exit(custom_exit);

MODULE_LICENSE("GPL");
