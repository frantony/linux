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
#include <linux/bootmem.h>
#include <linux/kernel.h>
#include <linux/of_fdt.h>

#include <linux/clk-provider.h>
#include <linux/clocksource.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/of_platform.h>

#include <asm/bootinfo.h>
#include <asm/prom.h>

void __init plat_mem_setup(void)
{
	__dt_setup_arch(__dtb_start);
}

void __init device_tree_init(void)
{
	unflatten_and_copy_device_tree();
}

const char *get_system_type(void)
{
	return "JZ4750D";
}

static struct of_device_id __initdata jz4780_ids[] = {
	{ .compatible = "simple-bus", },
	{},
};

int __init jz4780_publish_devices(void)
{
	return of_platform_bus_probe(NULL, jz4780_ids, NULL);
}
device_initcall(jz4780_publish_devices);
