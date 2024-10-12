#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("misael-diaz");
MODULE_DESCRIPTION("Dynamically loadable Linux kernel module exercise");

static struct proc_dir_entry *p_driver_proc_node;

static ssize_t driver_read (struct file *file,
			    char __user *buffer,
			    size_t size,
			    loff_t *offset)
{
	size_t rc = 0;
	char const msg[] = "awk!\n";
	printk("driver_read: entry\n");
	if (sizeof(msg) != (strlen(msg) + 1)) {
		printk("driver_read: StringSizingError\n");
		return 0;
	}

	// after the first read we reach "EOF" sort to speak so we return zero to indicate
	// that there are no more bytes left (to be read) in the userspace buffer
	if (sizeof(msg) == *offset) {
		printk("driver_read: exit\n");
		return 0;
	}

	rc = copy_to_user(buffer, msg, sizeof(msg));
	if (rc) {
		printk("driver_read: CopyError\n");
		printk("driver_read: exit\n");
		return 0;
	}

	*offset = sizeof(msg);
	printk("driver_read: exit\n");
	return sizeof(msg);
}

static struct file_operations driver_file_operations = {
	.read = driver_read
};

static int mod_init (void)
{
	printk("mod_init: entry\n");
	p_driver_proc_node = proc_create("ldd_driver", 0, NULL, &driver_file_operations);
	printk("mod_init: exit\n");
	return 0;
}

static void mod_exit (void)
{
	printk("mod_exit: entry\n");
	proc_remove(p_driver_proc_node);
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
