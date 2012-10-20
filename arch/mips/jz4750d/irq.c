/*
 *  Copyright (C) 2012, Antony Pavlov <antonynpavlov@gmail.com>
 *  JZ4750D platform IRQ support
 *
 *  based on JZ4740 platform IRQ support
 *  Copyright (C) 2009-2010, Lars-Peter Clausen <lars@metafoo.de>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 */

#include <linux/errno.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/timex.h>
#include <linux/slab.h>
#include <linux/delay.h>

#include <linux/debugfs.h>
#include <linux/seq_file.h>

#include <asm/io.h>
#include <asm/mipsregs.h>
#include <asm/irq_cpu.h>

#include <asm/mach-jz4750d/base.h>

static void __iomem *jz_intc_base;
static uint32_t jz_intc_wakeup;

#define JZ_REG_INTC_STATUS	0x00
#define JZ_REG_INTC_MASK	0x04
#define JZ_REG_INTC_SET_MASK	0x08
#define JZ_REG_INTC_CLEAR_MASK	0x0c
#define JZ_REG_INTC_PENDING	0x10

#define IRQ_BIT(x) BIT((x) - JZ4750D_IRQ_BASE)

static inline unsigned long intc_irq_bit(struct irq_data *data)
{
	return (unsigned long)irq_data_get_irq_chip_data(data);
}

static void intc_irq_unmask(struct irq_data *d)
{
	writel(intc_irq_bit(d), jz_intc_base + JZ_REG_INTC_CLEAR_MASK);
}

static void intc_irq_mask(struct irq_data *d)
{
	writel(intc_irq_bit(d), jz_intc_base + JZ_REG_INTC_SET_MASK);
}

static int intc_irq_set_wake(struct irq_data *d, unsigned int on)
{
	if (on)
		jz_intc_wakeup |= intc_irq_bit(d);
	else
		jz_intc_wakeup &= ~intc_irq_bit(d);

	return 0;
}

static struct irq_chip intc_irq_type = {
	.name =		"INTC",
	.irq_mask =	intc_irq_mask,
	.irq_mask_ack =	intc_irq_mask,
	.irq_unmask =	intc_irq_unmask,
	.irq_set_wake =	intc_irq_set_wake,
};

static irqreturn_t jz4750d_cascade(int irq, void *data)
{
	uint32_t irq_reg;

	irq_reg = readl(jz_intc_base + JZ_REG_INTC_PENDING);

	if (irq_reg)
		generic_handle_irq(__fls(irq_reg) + JZ4750D_IRQ_BASE);

	return IRQ_HANDLED;
}

static struct irqaction jz4750d_cascade_action = {
	.handler = jz4750d_cascade,
	.name = "JZ4750D cascade interrupt",
};

void __init arch_init_irq(void)
{
	int i;

	mips_cpu_irq_init();

	jz_intc_base = ioremap(JZ4750D_INTC_BASE_ADDR, 0x14);

	for (i = JZ4750D_IRQ_BASE; i < JZ4750D_IRQ_BASE + 32; i++) {
		irq_set_chip_data(i, (void *)IRQ_BIT(i));
		irq_set_chip_and_handler(i, &intc_irq_type, handle_level_irq);
	}

	setup_irq(2, &jz4750d_cascade_action);
}

asmlinkage void plat_irq_dispatch(void)
{
	unsigned int pending = read_c0_status() & read_c0_cause() & ST0_IM;
	if (pending & STATUSF_IP2)
		do_IRQ(2);
	else if (pending & STATUSF_IP3)
		do_IRQ(3);
	else
		spurious_interrupt();
}

#ifdef CONFIG_DEBUG_FS

static inline void intc_seq_reg(struct seq_file *s, const char *name,
	unsigned int reg)
{
	seq_printf(s, "%s:\t\t%08x\n", name, readl(jz_intc_base + reg));
}

static int intc_regs_show(struct seq_file *s, void *unused)
{
	intc_seq_reg(s, "Status", JZ_REG_INTC_STATUS);
	intc_seq_reg(s, "Mask", JZ_REG_INTC_MASK);
	intc_seq_reg(s, "Pending", JZ_REG_INTC_PENDING);

	return 0;
}

static int intc_regs_open(struct inode *inode, struct file *file)
{
	return single_open(file, intc_regs_show, NULL);
}

static const struct file_operations intc_regs_operations = {
	.open		= intc_regs_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int __init intc_debugfs_init(void)
{
	(void) debugfs_create_file("jz_regs_intc", S_IFREG | S_IRUGO,
				NULL, NULL, &intc_regs_operations);
	return 0;
}
subsys_initcall(intc_debugfs_init);

#endif
