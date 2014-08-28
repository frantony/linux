/*
 *  Copyright (C) 2012, Antony Pavlov <antonynpavlov@gmail.com>
 *  JZ4750D platform devices
 *
 *  based on JZ4740 platform devices
 *  Copyright (C) 2009-2010, Lars-Peter Clausen <lars@metafoo.de>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 */

#include <linux/device.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/resource.h>

#include <linux/dma-mapping.h>

#include <asm/mach-jz4750d/platform.h>
#include <asm/mach-jz4750d/base.h>
#include <asm/mach-jz4750d/irq.h>

#include <linux/serial_core.h>
#include <linux/serial_8250.h>

/* Serial */
#define JZ4750D_UART_DATA(_id) \
	{ \
		.flags = (UPF_SKIP_TEST | UPF_IOREMAP | UPF_FIXED_TYPE \
			/* | UPF_BOOT_AUTOCONF */), \
		.iotype = UPIO_MEM, \
		.regshift = 2, \
		.type = PORT_INGENIC_JZ, \
		.mapbase = JZ4750D_UART ## _id ## _BASE_ADDR, \
		.irq = JZ4750D_IRQ_UART ## _id, \
		.uartclk = 12000000 \
	}

static struct plat_serial8250_port jz4750d_uart_data[] = {
	JZ4750D_UART_DATA(0),
	JZ4750D_UART_DATA(1),
	{},
};

static struct platform_device jz4750d_uart_device = {
	.name = "serial8250",
	.id = 0,
	.dev = {
		.platform_data = jz4750d_uart_data,
	},
};

void jz4750d_serial_device_register(void)
{
	platform_device_register(&jz4750d_uart_device);
}
