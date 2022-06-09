#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/uaccess.h>

#define MYDEV_NAME "solution_node"
#define KBUF_SIZE (size_t)((10) * PAGE_SIZE)

static dev_t first;
static unsigned int count = 1;
static int my_major = 240;
static int my_minor = 0;
static struct cdev* my_cdev;

static size_t open_count = 0;
static size_t write_count = 0;

char kbuf[KBUF_SIZE];
char* kbufptr = NULL;

static int solution_node_open(struct inode* sol_inode, struct file* sol_file)
{
  printk(KERN_INFO "kernel_mooc solution_node_open");
  kbufptr = kbuf;
  ++open_count;
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
  sprintf(kbuf, "%lu %lu\n", open_count, write_count);
  
  while (buf_size && *kbufptr)
  {
    put_user(*(kbufptr++), buf++);
    buf_size--;
    bytes_read++;
  }
  
  return bytes_read;
}

static ssize_t solution_node_write(struct file* sol_file, const char __user* buf, size_t buf_size, loff_t* ppos)
{
  printk(KERN_INFO "kernel_mooc solution_node_write");
  write_count += buf_size;
  return buf_size;
}

static const struct file_operations mycdev_fops =
{
  .owner = THIS_MODULE,
  .open = solution_node_open,
  .release = solution_node_release,
  .read = solution_node_read,
  .write = solution_node_write
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
