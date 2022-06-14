#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/list.h>
#include <linux/list_sort.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/types.h>

struct module_entry
{
  char name[MODULE_NAME_LEN];
  struct list_head list;
};

static struct module_entry my_module_entry;

static ssize_t my_sys_show(struct kobject* kobj, struct kobj_attribute* attr, char* buf)
{
  ssize_t counter = 0;
  struct list_head* iter = NULL;
  list_for_each(iter, &my_module_entry.list)
  {
    struct module_entry* entry = list_entry(iter, struct module_entry, list);
    counter += sprintf(buf + counter, "%s\n", entry->name);
  }
  return counter;
}

static ssize_t my_sys_store(struct kobject* kobj, struct kobj_attribute* attr, const char* buf, size_t count)
{
  return 0;
}

static struct kobject* my_kobject = NULL;
static struct kobj_attribute my_sys_attribute = __ATTR(my_sys, 0664, my_sys_show, my_sys_store);

static int compare(void* priv, struct list_head* a, struct list_head* b)
{
  struct module_entry* a_me = list_entry(a, struct module_entry, list);
  struct module_entry* b_me = list_entry(b, struct module_entry, list);
  return strcmp(a_me->name, b_me->name);
}

static void sort_module_list(void)
{
  list_sort(NULL, &my_module_entry.list, compare);
}

static void copy_module_list(void)
{
  struct list_head* iter = NULL;
  INIT_LIST_HEAD(&my_module_entry.list);
  
  list_for_each(iter, THIS_MODULE->list.prev)
  {
    struct module* iter_module = list_entry(iter, struct module, list);
    struct module_entry* entry = kmalloc(sizeof(struct module_entry), GFP_KERNEL);
    strcpy(entry->name, iter_module->name);
    list_add(&entry->list, &my_module_entry.list);
  }
}

static int __init custom_init(void)
{
  int retval = 0;
  printk(KERN_INFO "kernel_mooc init_module kyamran");
  
  my_kobject = kobject_create_and_add("my_kobject", kernel_kobj);
  if (!my_kobject)
  {
    printk(KERN_ERR "kernel_mooc error kobject_create_and_add");
    return 1;
  }

  retval = sysfs_create_file(my_kobject, &my_sys_attribute.attr);
  if (retval)
  {
    printk(KERN_ERR "kernel_mooc error sysfs_create_file");
    kobject_put(my_kobject);
    return 1;
  }

  copy_module_list();
  sort_module_list();

  return 0;
}

static void __exit custom_exit(void)
{
  struct list_head* iter = NULL;
  struct module_entry* entry = NULL;

  kobject_put(my_kobject);
  
  iter = my_module_entry.list.next;
  while (iter != &my_module_entry.list)
  {
    struct list_head* iter_temp = iter->next;
    list_del(iter);
    entry = list_entry(iter, struct module_entry, list);
    kfree(entry);
    iter = iter_temp;
  }
  
  printk(KERN_INFO "kernel_mooc cleanup_module kyamran");
}

module_init(custom_init);
module_exit(custom_exit);

MODULE_LICENSE("GPL");
