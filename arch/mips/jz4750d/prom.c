/*
 *  Copyright (C) 2012, Antony Pavlov <antonynpavlov@gmail.com>
 *  JZ4750D SoC prom code
 *
 *  based on JZ4740 SoC prom code
 *  Copyright (C) 2010, Lars-Peter Clausen <lars@metafoo.de>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>

#include <linux/serial_reg.h>

#include <asm/bootinfo.h>
#include <asm/mach-jz4750d/base.h>

static __init void jz4750d_init_cmdline(int argc, char *argv[])
{
	unsigned int count = COMMAND_LINE_SIZE - 1;
	int i;
	char *dst = &(arcs_cmdline[0]);
	char *src;

	for (i = 1; i < argc && count; ++i) {
		src = argv[i];
		while (*src && count) {
			*dst++ = *src++;
			--count;
		}
		*dst++ = ' ';
	}
	if (i > 1)
		--dst;

	*dst = 0;
}

void __init prom_init(void)
{
	jz4750d_init_cmdline((int)fw_arg0, (char **)fw_arg1);
	mips_machtype = MACH_INGENIC_JZ4750D;
}

void __init prom_free_prom_memory(void)
{
}

#define UART_REG(_reg) ((void __iomem *)CKSEG1ADDR(JZ4750D_UART1_BASE_ADDR + (_reg << 2)))

void prom_putchar(char c)
{
	uint8_t lsr;

	do {
		lsr = readb(UART_REG(UART_LSR));
	} while ((lsr & UART_LSR_TEMT) == 0);

	writeb(c, UART_REG(UART_TX));
}
