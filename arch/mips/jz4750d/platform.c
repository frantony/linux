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

#include "serial.h"
#include "clock.h"

/* Serial */
#define JZ4750D_UART_DATA(_id) \
	{ \
		.flags = UPF_SKIP_TEST | UPF_IOREMAP | UPF_FIXED_TYPE, \
		.iotype = UPIO_MEM, \
		.regshift = 2, \
		.serial_out = jz4750d_serial_out, \
		.type = PORT_16550, \
		.mapbase = JZ4750D_UART ## _id ## _BASE_ADDR, \
		.irq = JZ4750D_IRQ_UART ## _id, \
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

#define JZ_REG_CLOCK_CTRL	0x00
#define JZ_CLOCK_CTRL_KO_ENABLE	BIT(30)

void jz4750d_serial_device_register(void)
{
	void __iomem *cpm_base = ioremap(JZ4750D_CPM_BASE_ADDR, 0x100);
	struct plat_serial8250_port *p;
	int uart_rate;

	uart_rate = jz4750d_clock_bdata.ext_rate;

	/*
	 * FIXME
	 * ECS bit selects the clock source between EXCLK and EXCLK/2 output
	 * This bit is only used to APB device such as UART I2S I2C SSI SADC UDC_PHY etc.
	 */

	if (readl(cpm_base + JZ_REG_CLOCK_CTRL) & JZ_CLOCK_CTRL_KO_ENABLE) {
		uart_rate >>= 1;
	}

	for (p = jz4750d_uart_data; p->flags != 0; ++p)
		p->uartclk = uart_rate;

	platform_device_register(&jz4750d_uart_device);
}
