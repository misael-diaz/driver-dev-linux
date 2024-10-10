#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL3-or-higher");
MODULE_AUTHOR("misael-diaz");
MODULE_DESCRIPTION("Dynamically loadable Linux kernel module exercise");

static int mod_init (void)
{
	printk("hello world from mod_init\n");
	return 0;
}

static void mod_exit (void)
{
	printk("exiting from mod_exit\n");
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
