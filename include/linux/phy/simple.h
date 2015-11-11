/*
 * Copyright (C) 2015 Alban Bedel <albeu@free.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __LINUX_PHY_SIMPLE__
#define __LINUX_PHY_SIMPLE__

#include <linux/phy/phy.h>

struct reset_control;
struct clk;

struct simple_phy {
	struct regulator *regulator;
	struct reset_control *reset;
	struct clk *clk;
};

struct simple_phy_desc {
	const struct phy_ops *ops;
	const char *regulator;
	const char *reset;
	const char *clk;
};

struct phy *devm_simple_phy_create(struct device *dev,
				const struct simple_phy_desc *desc,
				struct simple_phy *sphy);

int simple_phy_power_on(struct phy *phy);

int simple_phy_power_off(struct phy *phy);

#endif /* __LINUX_PHY_SIMPLE__ */
