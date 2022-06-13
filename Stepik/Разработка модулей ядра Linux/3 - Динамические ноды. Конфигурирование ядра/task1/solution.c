#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/uaccess.h>

#define MYDEV_NAME "solution_node"
#define MYCLASS_NAME "solution_class"
#define KBUF_SIZE (size_t)((10) * PAGE_SIZE)

static dev_t first;
static unsigned int count = 1;
static int my_major;
static int my_minor = 0;
static struct cdev* my_cdev = NULL;
static struct class* my_class = NULL;
static char* node_name;
module_param(node_name, charp, 0);

char kbuf[KBUF_SIZE];
char* kbufptr = NULL;

static int solution_node_open(struct inode* sol_inode, struct file* sol_file)
{
  printk(KERN_INFO "kernel_mooc solution_node_open");
  kbufptr = kbuf;
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
  sprintf(kbuf, "%d\n", my_major);
    
  while (buf_size && *kbufptr)
  {
    put_user(*(kbufptr++), buf++);
    buf_size--;
    bytes_read++;
  }
  
  return bytes_read;
}

static const struct file_operations mycdev_fops =
{
  .owner = THIS_MODULE,
  .open = solution_node_open,
  .release = solution_node_release,
  .read = solution_node_read,
};

static int __init custom_init(void)
{
  printk(KERN_INFO "kernel_mooc init_module kyamran");
  
  if (alloc_chrdev_region(&first, my_minor, count, MYDEV_NAME))
  {
    printk(KERN_ERR "kernel_mooc error alloc_chrdev_region");
    return -1;
  }
  my_major = MAJOR(first);
  
  my_cdev = cdev_alloc();
  if (!my_cdev)
  {
    printk(KERN_ERR "kernel_mooc error cdev_alloc");
    return -1;
  }
  
  cdev_init(my_cdev, &mycdev_fops);
  
  if (cdev_add(my_cdev, first, count))
  {
    printk(KERN_ERR "kernel_mooc error cdev_add");
    return -1;
  }

  my_class = class_create(THIS_MODULE, MYCLASS_NAME);
  if (!my_class)
  {
    printk(KERN_ERR "kernel_mooc error class_create");
    return -1;
  }

  if (!device_create(my_class, NULL, first, "%s", node_name))
  {
    printk(KERN_ERR "kernel_mooc error device_create");
    return -1;
  }

  return 0;
}

static void __exit custom_exit(void)
{
  printk(KERN_INFO "kernel_mooc cleanup_module kyamran");

  device_destroy(my_class, first);

  class_destroy(my_class);
  
  if (my_cdev)
  {
    cdev_del(my_cdev);
  }

  unregister_chrdev_region(first, count);
}

module_init(custom_init);
module_exit(custom_exit);

MODULE_LICENSE("GPL");
