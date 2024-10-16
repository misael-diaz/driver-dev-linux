#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("misael-diaz");
MODULE_DESCRIPTION("Dynamically loadable Linux kernel module exercise");

#define DRIVER_BUFFER_SIZE 80
#define DRIVER_BUFFER_LEN (DRIVER_BUFFER_SIZE - 1)

static char driver_buf[DRIVER_BUFFER_SIZE];
static struct proc_dir_entry *pde;

static ssize_t driver_write (struct file *filp,
			     const char __user *buffer,
			     size_t size,
			     loff_t *offset)
{
	printk("driver_write: entry\n");
	printk("sz: %zu\n", size);
	if (DRIVER_BUFFER_SIZE < size) {
		printk("driver_write: message has been truncated\n");
		printk("driver_write: exit\n");
		copy_from_user(driver_buf, buffer, DRIVER_BUFFER_SIZE);
		driver_buf[DRIVER_BUFFER_LEN] = 0;
		// if we don't return size (even though we have truncated the message)
		// we are going to have to handle another write system call and we don't
		// want to because we have already committed ourselves to truncating so
		// by doing this we make sure that we only have to handle one write;
		// this makes sense for completing the echoing challenge
		return size;
	}
	copy_from_user(driver_buf, buffer, size);
	printk("driver_write: exit\n");
	return size;
}

static ssize_t driver_read (struct file *filp,
			    char __user *buffer,
			    size_t size,
			    loff_t *offset)
{
	size_t rc = 0;
	size_t const sz_driver_buf = 1 + strlen(driver_buf);
	printk("driver_read: entry\n");
	if (sz_driver_buf == *offset) {
		// we have already copied to the userspace the entire message so we return
		// zero bytes to indicate that there's nothing else to read
		printk("driver_read: exit\n");
		return 0;
	}

	rc = copy_to_user(buffer, driver_buf, sz_driver_buf);
	if (rc) {
		printk("driver_read: CopyError\n");
		printk("driver_read: exit\n");
		return 0;
	}

	*offset = sz_driver_buf;
	printk("driver_read: exit\n");
	return sz_driver_buf;
}

static struct file_operations fop = {
	.read = driver_read,
	.write = driver_write
};

static int mod_init (void)
{
	printk("mod_init: entry\n");
	pde = proc_create("ldd_driver", 0, NULL, &fop);
	printk("mod_init: exit\n");
	return 0;
}

static void mod_exit (void)
{
	printk("mod_exit: entry\n");
	proc_remove(pde);
	printk("mod_exit: exit\n");
}

module_init(mod_init);
module_exit(mod_exit);

/*

linux-driver-dev                                             October 10, 2024

author: @misael-diaz
source: src/module/ldd.c

Copyright (C) 2024 Misael Díaz-Maldonado

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/
