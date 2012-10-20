/*
 *  Copyright (C) 2012, Antony Pavlov <antonynpavlov@gmail.com>
 *  JZ4750D reset routines
 *
 *  based on JZ4740 reset routines
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
#include <linux/pm.h>

#include <asm/reboot.h>

#include <asm/mach-jz4750d/base.h>
#include <asm/mach-jz4750d/timer.h>

static void jz4750d_halt(void)
{
	while (1) {
		__asm__(".set push;\n"
			".set mips3;\n"
			"wait;\n"
			".set pop;\n"
		);
	}
}

#define JZ_REG_WDT_DATA 0x00
#define JZ_REG_WDT_COUNTER_ENABLE 0x04
#define JZ_REG_WDT_COUNTER 0x08
#define JZ_REG_WDT_CTRL 0x0c

static void jz4750d_restart(char *command)
{
	void __iomem *wdt_base = ioremap(JZ4750D_WDT_BASE_ADDR, 0x0f);

	jz4750d_timer_enable_watchdog();

	writeb(0, wdt_base + JZ_REG_WDT_COUNTER_ENABLE);

	writew(0, wdt_base + JZ_REG_WDT_COUNTER);
	writew(0, wdt_base + JZ_REG_WDT_DATA);
	writew(BIT(2), wdt_base + JZ_REG_WDT_CTRL);

	writeb(1, wdt_base + JZ_REG_WDT_COUNTER_ENABLE);
	jz4750d_halt();
}

#define JZ_REG_RTC_CTRL		0x00
#define JZ_REG_RTC_HIBERNATE	0x20

#define JZ_RTC_CTRL_WRDY	BIT(7)

static void jz4750d_power_off(void)
{
	void __iomem *rtc_base = ioremap(JZ4750D_RTC_BASE_ADDR, 0x24);
	uint32_t ctrl;

	do {
		ctrl = readl(rtc_base + JZ_REG_RTC_CTRL);
	} while (!(ctrl & JZ_RTC_CTRL_WRDY));

	writel(1, rtc_base + JZ_REG_RTC_HIBERNATE);
	jz4750d_halt();
}

void jz4750d_reset_init(void)
{
	_machine_restart = jz4750d_restart;
	_machine_halt = jz4750d_halt;
	pm_power_off = jz4750d_power_off;
}
