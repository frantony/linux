/*
 *  Copyright (C) 2012, Antony Pavlov <antonynpavlov@gmail.com>
 *  Ritmix RZX-50 board setup routines.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>

#include <asm/mach-jz4750d/platform.h>

static int __init rzx50_board_setup(void)
{
	printk(KERN_ERR "Ritmix RZX-50 board setup\n");

	jz4750d_serial_device_register();

	return 0;
}
arch_initcall(rzx50_board_setup);
