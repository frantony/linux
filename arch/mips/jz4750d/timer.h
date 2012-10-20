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

#ifndef __MIPS_JZ4750D_TIMER_H__
#define __MIPS_JZ4750D_TIMER_H__

#include <linux/module.h>
#include <linux/io.h>

#define JZ_REG_TIMER_ENABLE		0x00
#define JZ_REG_TIMER_ENABLE_SET		0x04
#define JZ_REG_TIMER_ENABLE_CLEAR	0x08
#define JZ_REG_TIMER_FLAG		0x10
#define JZ_REG_TIMER_FLAG_SET		0x14
#define JZ_REG_TIMER_FLAG_CLEAR		0x18
#define JZ_REG_TIMER_STOP		0x0C
#define JZ_REG_TIMER_STOP_SET		0x1C
#define JZ_REG_TIMER_STOP_CLEAR		0x2C
#define JZ_REG_TIMER_MASK		0x20
#define JZ_REG_TIMER_MASK_SET		0x24
#define JZ_REG_TIMER_MASK_CLEAR		0x28

#define JZ_REG_TIMER_DFR(x) (((x) * 0x10) + 0x30)
#define JZ_REG_TIMER_DHR(x) (((x) * 0x10) + 0x34)
#define JZ_REG_TIMER_CNT(x) (((x) * 0x10) + 0x38)
#define JZ_REG_TIMER_CTRL(x) (((x) * 0x10) + 0x3C)

#define JZ_TIMER_IRQ_HALF(x) BIT((x) + 0x10)
#define JZ_TIMER_IRQ_FULL(x) BIT(x)

#define JZ_TIMER_CTRL_PWM_ABBRUPT_SHUTDOWN	BIT(9)
#define JZ_TIMER_CTRL_PWM_ACTIVE_LOW		BIT(8)
#define JZ_TIMER_CTRL_PWM_ENABLE		BIT(7)
#define JZ_TIMER_CTRL_PRESCALE_MASK		0x1c
#define JZ_TIMER_CTRL_PRESCALE_OFFSET		0x3
#define JZ_TIMER_CTRL_PRESCALE_1		(0 << 3)
#define JZ_TIMER_CTRL_PRESCALE_4		(1 << 3)
#define JZ_TIMER_CTRL_PRESCALE_16		(2 << 3)
#define JZ_TIMER_CTRL_PRESCALE_64		(3 << 3)
#define JZ_TIMER_CTRL_PRESCALE_256		(4 << 3)
#define JZ_TIMER_CTRL_PRESCALE_1024		(5 << 3)

#define JZ_TIMER_CTRL_PRESCALER(x) ((x) << JZ_TIMER_CTRL_PRESCALE_OFFSET)

#define JZ_TIMER_CTRL_SRC_EXT		BIT(2)
#define JZ_TIMER_CTRL_SRC_RTC		BIT(1)
#define JZ_TIMER_CTRL_SRC_PCLK		BIT(0)

extern void __iomem *jz4750d_timer_base;
void __init jz4750d_timer_init(void);

static inline void jz4750d_timer_stop(unsigned int timer)
{
	writel(BIT(timer), jz4750d_timer_base + JZ_REG_TIMER_STOP_SET);
}

static inline void jz4750d_timer_start(unsigned int timer)
{
	writel(BIT(timer), jz4750d_timer_base + JZ_REG_TIMER_STOP_CLEAR);
}

static inline bool jz4750d_timer_is_enabled(unsigned int timer)
{
	return readb(jz4750d_timer_base + JZ_REG_TIMER_ENABLE) & BIT(timer);
}

static inline void jz4750d_timer_enable(unsigned int timer)
{
	writeb(BIT(timer), jz4750d_timer_base + JZ_REG_TIMER_ENABLE_SET);
}

static inline void jz4750d_timer_disable(unsigned int timer)
{
	writeb(BIT(timer), jz4750d_timer_base + JZ_REG_TIMER_ENABLE_CLEAR);
}

static inline void jz4750d_timer_set_period(unsigned int timer, u16 period)
{
	writew(period, jz4750d_timer_base + JZ_REG_TIMER_DFR(timer));
}

static inline void jz4750d_timer_set_duty(unsigned int timer, u16 duty)
{
	writew(duty, jz4750d_timer_base + JZ_REG_TIMER_DHR(timer));
}

static inline void jz4750d_timer_set_count(unsigned int timer, u16 count)
{
	writew(count, jz4750d_timer_base + JZ_REG_TIMER_CNT(timer));
}

static inline u16 jz4750d_timer_get_count(unsigned int timer)
{
	return readw(jz4750d_timer_base + JZ_REG_TIMER_CNT(timer));
}

static inline void jz4750d_timer_ack_full(unsigned int timer)
{
	writel(JZ_TIMER_IRQ_FULL(timer),
		jz4750d_timer_base + JZ_REG_TIMER_FLAG_CLEAR);
}

static inline void jz4750d_timer_irq_full_enable(unsigned int timer)
{
	writel(JZ_TIMER_IRQ_FULL(timer),
		jz4750d_timer_base + JZ_REG_TIMER_FLAG_CLEAR);
	writel(JZ_TIMER_IRQ_FULL(timer),
		jz4750d_timer_base + JZ_REG_TIMER_MASK_CLEAR);
}

static inline void jz4750d_timer_irq_full_disable(unsigned int timer)
{
	writel(JZ_TIMER_IRQ_FULL(timer),
		jz4750d_timer_base + JZ_REG_TIMER_MASK_SET);
}

static inline void jz4750d_timer_set_ctrl(unsigned int timer, u16 ctrl)
{
	writew(ctrl, jz4750d_timer_base + JZ_REG_TIMER_CTRL(timer));
}

static inline u16 jz4750d_timer_get_ctrl(unsigned int timer)
{
	return readw(jz4750d_timer_base + JZ_REG_TIMER_CTRL(timer));
}

#define JZ_REG_OSTIMER_DR		0xD0
#define JZ_REG_OSTIMER_CNT		0xD8
#define JZ_REG_OSTIMER_CTRL		0xDC

#define JZ_OSTIMER_CTRL_PRESCALE_1		(0 << 3)
#define JZ_OSTIMER_CTRL_PRESCALE_4		(1 << 3)
#define JZ_OSTIMER_CTRL_PRESCALE_16		(2 << 3)
#define JZ_OSTIMER_CTRL_PRESCALE_64		(3 << 3)
#define JZ_OSTIMER_CTRL_PRESCALE_256		(4 << 3)
#define JZ_OSTIMER_CTRL_PRESCALE_1024		(5 << 3)

#define JZ_OSTIMER_CTRL_SRC_EXT		BIT(2)
#define JZ_OSTIMER_CTRL_SRC_RTC		BIT(1)
#define JZ_OSTIMER_CTRL_SRC_PCLK	BIT(0)

#define JZ_TIMER_SCR_OSTSC		BIT(15)
#define JZ_TIMER_ESR_OSTST		BIT(15)

static inline void jz4750d_ostimer_set_ctrl(u16 ctrl)
{
	writew(ctrl, jz4750d_timer_base + JZ_REG_OSTIMER_CTRL);
}

static inline uint32_t jz4750d_ostimer_get_count(void)
{
	return readl(jz4750d_timer_base + JZ_REG_OSTIMER_CNT);
}

static inline void jz4750d_ostimer_start(void)
{
	writel(JZ_TIMER_SCR_OSTSC,
		jz4750d_timer_base + JZ_REG_TIMER_STOP_CLEAR);
}

static inline void jz4750d_ostimer_enable(void)
{
	writel(JZ_TIMER_ESR_OSTST,
		jz4750d_timer_base + JZ_REG_TIMER_ENABLE_SET);
}

static inline void jz4750d_ostimer_set_period(u32 period)
{
	writel(period, jz4750d_timer_base + JZ_REG_OSTIMER_DR);
}
#endif
