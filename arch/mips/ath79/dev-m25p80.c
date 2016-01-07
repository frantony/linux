/*
 *  Copyright (C) 2009-2012 Gabor Juhos <juhosg@openwrt.org>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/concat.h>

#include "dev-spi.h"
#include "dev-m25p80.h"

static struct ath79_spi_controller_data ath79_spi0_cdata =
{
};

static struct spi_board_info ath79_spi_info[] = {
	{
		.bus_num	= 0,
		.chip_select	= 0,
		.max_speed_hz	= 25000000,
		.modalias	= "m25p80",
		.controller_data = &ath79_spi0_cdata,
	},
};

static struct ath79_spi_platform_data ath79_spi_data;

void __init ath79_register_m25p80(struct flash_platform_data *pdata)
{
	ath79_spi_data.bus_num = 0;
	ath79_spi_data.num_chipselect = 1;
	ath79_spi0_cdata.is_flash = true;
	ath79_spi_info[0].platform_data = pdata;
	ath79_register_spi(&ath79_spi_data, ath79_spi_info, 1);
}
