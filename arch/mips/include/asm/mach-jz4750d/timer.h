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

#ifndef __ASM_MACH_JZ4750D_TIMER
#define __ASM_MACH_JZ4750D_TIMER

void jz4750d_timer_enable_watchdog(void);
void jz4750d_timer_disable_watchdog(void);

#endif
