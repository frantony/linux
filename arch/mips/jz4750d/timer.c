/*
 *  Copyright (C) 2012, Antony Pavlov <antonynpavlov@gmail.com>
 *  JZ4750D platform timer support
 *
 *  based on JZ4740 platform timer support
 *  Copyright (C) 2010, Lars-Peter Clausen <lars@metafoo.de>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 */

#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include "timer.h"

#include <asm/mach-jz4750d/base.h>

void __iomem *jz4750d_timer_base;

void jz4750d_timer_enable_watchdog(void)
{
	writel(BIT(16), jz4750d_timer_base + JZ_REG_TIMER_STOP_CLEAR);
}
EXPORT_SYMBOL_GPL(jz4750d_timer_enable_watchdog);

void jz4750d_timer_disable_watchdog(void)
{
	writel(BIT(16), jz4750d_timer_base + JZ_REG_TIMER_STOP_SET);
}
EXPORT_SYMBOL_GPL(jz4750d_timer_disable_watchdog);

void __init jz4750d_timer_init(void)
{
	jz4750d_timer_base = ioremap(JZ4750D_TCU_BASE_ADDR, 0x100);

	if (!jz4750d_timer_base)
		panic("Failed to ioremap timer registers");

	/* Disable __ALL__ timer clocks */
	writel(0x000180ff, jz4750d_timer_base + JZ_REG_TIMER_STOP_SET);

	/* Timer irqs are unmasked by default, mask them __ALL__ */
	writel(0x003f803f, jz4750d_timer_base + JZ_REG_TIMER_MASK_SET);
}
