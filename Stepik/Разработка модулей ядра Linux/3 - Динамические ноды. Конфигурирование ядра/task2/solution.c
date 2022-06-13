#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/uaccess.h>

#define MYDEV_NAME "solution_node"
#define KBUF_SIZE 255

static dev_t first;
static unsigned int count = 1;
static int my_major = 240;
static int my_minor = 0;
static struct cdev* my_cdev;

static int open_count = -1;

static int solution_node_open(struct inode* sol_inode, struct file* sol_file)
{
  printk(KERN_INFO "kernel_mooc solution_node_open");
  sol_file->private_data = kmalloc(KBUF_SIZE + 1, GFP_KERNEL);
  memset(sol_file->private_data, 0, KBUF_SIZE + 1);
  ++open_count;
  sprintf(sol_file->private_data, "%d", open_count);
  return 0;
}

static int solution_node_release(struct inode* sol_inode, struct file* sol_file)
{
  printk(KERN_INFO "kernel_mooc solution_node_release");
  kfree(sol_file->private_data);
  return 0;
}

static ssize_t solution_node_read(struct file* sol_file, char __user* buf, size_t buf_size, loff_t* ppos)
{
  int read_bytes = 0;
  int retval = 0;

  printk(KERN_INFO "kernel_mooc solution_node_read");

  read_bytes = (*ppos + buf_size) < KBUF_SIZE ? buf_size : strlen(sol_file->private_data + *ppos);
  retval = copy_to_user(buf, sol_file->private_data + *ppos, read_bytes);
  if (retval)
  {
    return -EINVAL;
  }
  *ppos += read_bytes;
  return read_bytes;
}

static ssize_t solution_node_write(struct file* sol_file, const char __user* buf, size_t buf_size, loff_t* ppos)
{
  int write_bytes = 0;
  int retval = 0;
  
  printk(KERN_INFO "kernel_mooc solution_node_write");

  write_bytes = (*ppos + buf_size) < KBUF_SIZE ? buf_size : KBUF_SIZE - *ppos;
  retval = copy_from_user(sol_file->private_data + *ppos, buf, write_bytes);
  if (retval)
  {
    return -EINVAL;
  }
  *ppos += write_bytes;
  return write_bytes;
}

static loff_t solution_node_llseek(struct file *sol_file, loff_t offset, int orig)
{
  loff_t testpos = 0;

  printk(KERN_INFO "kernel_mooc solution_node_llseek");

  switch (orig)
  {
  case SEEK_SET:
    testpos = offset;
    break;
  case SEEK_CUR:
    testpos = sol_file->f_pos + offset;
    break;
  case SEEK_END:
    testpos = strlen(sol_file->private_data) + offset;
    break;
  default:
    return -EINVAL;
  }

  testpos = testpos < KBUF_SIZE ? testpos : KBUF_SIZE;
  testpos = testpos >= 0 ? testpos : 0;
  sol_file->f_pos = testpos;

  return testpos;
}

static const struct file_operations mycdev_fops =
{
  .owner = THIS_MODULE,
  .open = solution_node_open,
  .release = solution_node_release,
  .read = solution_node_read,
  .write = solution_node_write,
  .llseek  = solution_node_llseek
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
  cdev_del(my_cdev);
  unregister_chrdev_region(first, count);
}

module_init(custom_init);
module_exit(custom_exit);

MODULE_LICENSE("GPL");
