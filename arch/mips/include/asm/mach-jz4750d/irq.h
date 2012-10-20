/*
 *  Copyright (C) 2012, Antony Pavlov <antonynpavlov@gmail.com>
 *  JZ4750D IRQ definitions
 *
 *  based on JZ4740 IRQ definitions
 *  Copyright (C) 2009-2010, Lars-Peter Clausen <lars@metafoo.de>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 */

#ifndef __ASM_MACH_JZ4750D_IRQ_H__
#define __ASM_MACH_JZ4750D_IRQ_H__

#define MIPS_CPU_IRQ_BASE 0
#define JZ4750D_IRQ_BASE 8

#define JZ4750D_IRQ(x)		(JZ4750D_IRQ_BASE + (x))

#define JZ4750D_IRQ_UART1	JZ4750D_IRQ(8)
#define JZ4750D_IRQ_UART0	JZ4750D_IRQ(9)
#define JZ4750D_IRQ_TCU1	JZ4750D_IRQ(22)

#define NR_IRQS (256)

#endif
