/*
 * Copyright (C) 2015 Alban Bedel <albeu@free.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/phy/simple.h>
#include <linux/reset.h>

struct ath79_usb_phy {
	struct simple_phy sphy;
	struct reset_control *suspend_override;
};

static int ath79_usb_phy_power_on(struct phy *phy)
{
	struct ath79_usb_phy *priv = container_of(
		phy_get_drvdata(phy), struct ath79_usb_phy, sphy);
	int err;

	err = simple_phy_power_on(phy);
	if (err)
		return err;

	if (priv->suspend_override) {
		err = reset_control_assert(priv->suspend_override);
		if (err) {
			simple_phy_power_off(phy);
			return err;
		}
	}

	return 0;
}

static int ath79_usb_phy_power_off(struct phy *phy)
{
	struct ath79_usb_phy *priv = container_of(
		phy_get_drvdata(phy), struct ath79_usb_phy, sphy);
	int err;

	if (priv->suspend_override) {
		err = reset_control_deassert(priv->suspend_override);
		if (err)
			return err;
	}

	err = simple_phy_power_off(phy);
	if (err && priv->suspend_override)
		reset_control_assert(priv->suspend_override);

	return err;
}

static const struct phy_ops ath79_usb_phy_ops = {
	.power_on	= ath79_usb_phy_power_on,
	.power_off	= ath79_usb_phy_power_off,
	.owner		= THIS_MODULE,
};

static const struct simple_phy_desc ath79_usb_phy_desc = {
	.ops = &ath79_usb_phy_ops,
	.reset = "usb-phy",
	.clk = (void *)-ENOENT,
};

static int ath79_usb_phy_probe(struct platform_device *pdev)
{
	struct ath79_usb_phy *priv;
	struct phy *phy;

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->suspend_override = devm_reset_control_get_optional(
		&pdev->dev, "usb-suspend-override");
	if (IS_ERR(priv->suspend_override)) {
		if (PTR_ERR(priv->suspend_override) == -ENOENT)
			priv->suspend_override = NULL;
		else
			return PTR_ERR(priv->suspend_override);
	}

	phy = devm_simple_phy_create(&pdev->dev,
				&ath79_usb_phy_desc, &priv->sphy);
	if (IS_ERR(phy))
		return PTR_ERR(phy);

	return PTR_ERR_OR_ZERO(devm_of_phy_provider_register(
				&pdev->dev, of_phy_simple_xlate));
}

static const struct of_device_id ath79_usb_phy_of_match[] = {
	{ .compatible = "qca,ar7100-usb-phy" },
	{}
};
MODULE_DEVICE_TABLE(of, ath79_usb_phy_of_match);

static struct platform_driver ath79_usb_phy_driver = {
	.probe	= ath79_usb_phy_probe,
	.driver = {
		.of_match_table	= ath79_usb_phy_of_match,
		.name		= "ath79-usb-phy",
	}
};
module_platform_driver(ath79_usb_phy_driver);

MODULE_DESCRIPTION("ATH79 USB PHY driver");
MODULE_AUTHOR("Alban Bedel <albeu@free.fr>");
MODULE_LICENSE("GPL");
