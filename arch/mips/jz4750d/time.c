/*
 *  Copyright (C) 2012, Antony Pavlov <antonynpavlov@gmail.com>
 *  JZ4750D platform time support
 *
 *  based on JZ4740 platform time support
 *  Copyright (C) 2010, Lars-Peter Clausen <lars@metafoo.de>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 */

#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/time.h>

#include <linux/clockchips.h>

#include <asm/time.h>

#include "clock.h"
#include "timer.h"

#define TIMER_CLOCKEVENT 5

static uint16_t jz4750d_jiffies_per_tick;

static cycle_t jz4750d_clocksource_read(struct clocksource *cs)
{
	return (cycle_t)jz4750d_ostimer_get_count();
}

static struct clocksource jz4750d_clocksource = {
	.name = "jz4750d-timer",
	.rating = 200,
	.read = jz4750d_clocksource_read,
	.mask = CLOCKSOURCE_MASK(32),
	.flags = CLOCK_SOURCE_WATCHDOG,
};

static irqreturn_t jz4750d_clockevent_irq(int irq, void *devid)
{
	struct clock_event_device *cd = devid;

	jz4750d_timer_ack_full(TIMER_CLOCKEVENT);

	if (cd->mode != CLOCK_EVT_MODE_PERIODIC)
		jz4750d_timer_disable(TIMER_CLOCKEVENT);

	cd->event_handler(cd);

	return IRQ_HANDLED;
}

static void jz4750d_clockevent_set_mode(enum clock_event_mode mode,
	struct clock_event_device *cd)
{
	switch (mode) {
	case CLOCK_EVT_MODE_PERIODIC:
		jz4750d_timer_set_count(TIMER_CLOCKEVENT, 0);
		jz4750d_timer_set_period(TIMER_CLOCKEVENT, jz4750d_jiffies_per_tick);
	case CLOCK_EVT_MODE_RESUME:
		jz4750d_timer_irq_full_enable(TIMER_CLOCKEVENT);
		jz4750d_timer_enable(TIMER_CLOCKEVENT);
		break;
	case CLOCK_EVT_MODE_ONESHOT:
	case CLOCK_EVT_MODE_SHUTDOWN:
		jz4750d_timer_disable(TIMER_CLOCKEVENT);
		break;
	default:
		break;
	}
}

static int jz4750d_clockevent_set_next(unsigned long evt,
	struct clock_event_device *cd)
{
	jz4750d_timer_set_count(TIMER_CLOCKEVENT, 0);
	jz4750d_timer_set_period(TIMER_CLOCKEVENT, evt);
	jz4750d_timer_enable(TIMER_CLOCKEVENT);

	return 0;
}

static struct clock_event_device jz4750d_clockevent = {
	.name = "jz4750d-timer",
	.features = CLOCK_EVT_FEAT_PERIODIC,
	.set_next_event = jz4750d_clockevent_set_next,
	.set_mode = jz4750d_clockevent_set_mode,
	.rating = 200,
	.irq = JZ4750D_IRQ_TCU1,
};

static struct irqaction timer_irqaction = {
	.handler = jz4750d_clockevent_irq,
	.flags = IRQF_DISABLED | IRQF_PERCPU | IRQF_TIMER,
	.name = "jz4750d-timerirq",
	.dev_id = &jz4750d_clockevent,
};

void __init plat_time_init(void)
{
	int ret;
	uint32_t clk_rate;

	jz4750d_timer_init();

	clk_rate = jz4750d_clock_bdata.ext_rate >> 4;
	jz4750d_jiffies_per_tick = DIV_ROUND_CLOSEST(clk_rate, HZ);

	clockevent_set_clock(&jz4750d_clockevent, clk_rate);
	jz4750d_clockevent.min_delta_ns =
		clockevent_delta2ns(100, &jz4750d_clockevent);
	jz4750d_clockevent.max_delta_ns =
		clockevent_delta2ns(0xffff, &jz4750d_clockevent);
	jz4750d_clockevent.cpumask = cpumask_of(0);

	jz4750d_timer_irq_full_disable(TIMER_CLOCKEVENT);

	clockevents_register_device(&jz4750d_clockevent);

	ret = clocksource_register_hz(&jz4750d_clocksource, clk_rate);

	if (ret)
		printk(KERN_ERR "Failed to register clocksource: %d\n", ret);

	setup_irq(JZ4750D_IRQ_TCU1, &timer_irqaction);

	jz4750d_timer_set_ctrl(TIMER_CLOCKEVENT, JZ_TIMER_CTRL_PRESCALE_16
		| JZ_TIMER_CTRL_SRC_EXT);
	jz4750d_ostimer_set_ctrl(JZ_OSTIMER_CTRL_PRESCALE_16
		| JZ_OSTIMER_CTRL_SRC_EXT);

	jz4750d_timer_set_period(TIMER_CLOCKEVENT, jz4750d_jiffies_per_tick);
	jz4750d_timer_irq_full_enable(TIMER_CLOCKEVENT);

	jz4750d_ostimer_set_period(0xffffffff);

	jz4750d_timer_enable(TIMER_CLOCKEVENT);
	jz4750d_timer_start(TIMER_CLOCKEVENT);
	jz4750d_ostimer_enable();
	jz4750d_ostimer_start();
}
