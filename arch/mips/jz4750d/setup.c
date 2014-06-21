/*
 *  Copyright (C) 2012, Antony Pavlov <antonynpavlov@gmail.com>
 *  JZ4750D setup code
 *
 *  based on JZ4740 setup code
 *  Copyright (C) 2009-2010, Lars-Peter Clausen <lars@metafoo.de>
 *  Copyright (C) 2011, Maarten ter Huurne <maarten@treewalker.org>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 */

#include <linux/init.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/of_fdt.h>

#include <asm/bootinfo.h>

#include "reset.h"

void __init plat_mem_setup(void)
{
	jz4750d_reset_init();

	/* FIXME: the detection of the memory size is skipped */
	add_memory_region(0, 0x04000000 /* 64 M */, BOOT_MEM_RAM);

	/*
	 * Load the builtin devicetree. This causes the chosen node to be
	 * parsed resulting in our memory appearing
	 */
	__dt_setup_arch(&__dtb_start);
}

const char *get_system_type(void)
{
	return "JZ4750D";
}

void __init device_tree_init(void)
{
	unsigned long base, size;

	if (!initial_boot_params)
		return;

	base = virt_to_phys((void *)initial_boot_params);
	size = be32_to_cpu(initial_boot_params->totalsize);

	/* Before we do anything, lets reserve the dt blob */
	reserve_bootmem(base, size, BOOTMEM_DEFAULT);

	unflatten_device_tree();
}
