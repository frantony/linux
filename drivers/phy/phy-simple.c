/*
 * Copyright (C) 2015 Alban Bedel <albeu@free.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/phy/simple.h>
#include <linux/clk.h>
#include <linux/reset.h>

int simple_phy_power_on(struct phy *phy)
{
	struct simple_phy *sphy = phy_get_drvdata(phy);
	int err;

	if (sphy->regulator) {
		err = regulator_enable(sphy->regulator);
		if (err)
			return err;
	}

	if (sphy->clk) {
		err = clk_prepare_enable(sphy->clk);
		if (err)
			goto regulator_disable;
	}

	if (sphy->reset) {
		err = reset_control_deassert(sphy->reset);
		if (err)
			goto clock_disable;
	}

	return 0;

clock_disable:
	if (sphy->clk)
		clk_disable_unprepare(sphy->clk);
regulator_disable:
	if (sphy->regulator)
		WARN_ON(regulator_disable(sphy->regulator));

	return err;
}
EXPORT_SYMBOL_GPL(simple_phy_power_on);

int simple_phy_power_off(struct phy *phy)
{
	struct simple_phy *sphy = phy_get_drvdata(phy);
	int err;

	if (sphy->reset) {
		err = reset_control_assert(sphy->reset);
		if (err)
			return err;
	}

	if (sphy->clk)
		clk_disable_unprepare(sphy->clk);

	if (sphy->regulator) {
		err = regulator_disable(sphy->regulator);
		if (err)
			goto clock_enable;
	}

	return 0;

clock_enable:
	if (sphy->clk)
		WARN_ON(clk_prepare_enable(sphy->clk));
	if (sphy->reset)
		WARN_ON(reset_control_deassert(sphy->reset));

	return err;
}
EXPORT_SYMBOL_GPL(simple_phy_power_off);

static const struct phy_ops simple_phy_ops = {
	.power_on	= simple_phy_power_on,
	.power_off	= simple_phy_power_off,
	.owner		= THIS_MODULE,
};

struct phy *devm_simple_phy_create(struct device *dev,
				const struct simple_phy_desc *desc,
				struct simple_phy *sphy)
{
	struct phy *phy;

	if (!dev || !desc)
		return ERR_PTR(-EINVAL);

	if (!sphy) {
		sphy = devm_kzalloc(dev, sizeof(*sphy), GFP_KERNEL);
		if (!sphy)
			return ERR_PTR(-ENOMEM);
	}

	if (!IS_ERR_OR_NULL(desc->regulator)) {
		sphy->regulator = devm_regulator_get(dev, desc->regulator);
		if (IS_ERR(sphy->regulator)) {
			if (PTR_ERR(sphy->regulator) == -ENOENT)
				sphy->regulator = NULL;
			else
				return ERR_PTR(PTR_ERR(sphy->regulator));
		}
	}

	if (!IS_ERR(desc->clk)) {
		sphy->clk = devm_clk_get(dev, desc->clk);
		if (IS_ERR(sphy->clk)) {
			if (PTR_ERR(sphy->clk) == -ENOENT)
				sphy->clk = NULL;
			else
				return ERR_PTR(PTR_ERR(sphy->clk));
		}
	}

	if (!IS_ERR(desc->reset)) {
		sphy->reset = devm_reset_control_get(dev, desc->reset);
		if (IS_ERR(sphy->reset)) {
			int err = PTR_ERR(sphy->reset);

			if (err == -ENOENT || err == -ENOTSUPP)
				sphy->reset = NULL;
			else
				return ERR_PTR(err);
		}
	}

	phy = devm_phy_create(dev, NULL, desc->ops ?: &simple_phy_ops);
	if (IS_ERR(phy))
		return ERR_PTR(PTR_ERR(phy));

	phy_set_drvdata(phy, sphy);

	return phy;
}
EXPORT_SYMBOL_GPL(devm_simple_phy_create);

#ifdef CONFIG_PHY_SIMPLE_PDEV
#ifdef CONFIG_OF
/* Default config, no regulator, default clock and reset if any */
static const struct simple_phy_desc simple_phy_default_desc = {};

static const struct of_device_id simple_phy_of_match[] = {
	{ .compatible = "simple-phy", .data = &simple_phy_default_desc },
	{}
};
MODULE_DEVICE_TABLE(of, simple_phy_of_match);

const struct simple_phy_desc *simple_phy_get_of_desc(struct device *dev)
{
	const struct of_device_id *match;

	match = of_match_device(simple_phy_of_match, dev);

	return match ? match->data : NULL;
}
#else
const struct simple_phy_desc *simple_phy_get_of_desc(struct device *dev)
{
	return NULL;
}
#endif

static int simple_phy_probe(struct platform_device *pdev)
{
	const struct simple_phy_desc *desc = pdev->dev.platform_data;
	struct phy *phy;

	if (!desc)
		desc = simple_phy_get_of_desc(&pdev->dev);
	if (!desc)
		return -EINVAL;

	phy = devm_simple_phy_create(&pdev->dev, desc, NULL);
	if (IS_ERR(phy))
		return PTR_ERR(phy);

	return PTR_ERR_OR_ZERO(devm_of_phy_provider_register(
				&pdev->dev, of_phy_simple_xlate));
}

static struct platform_driver simple_phy_driver = {
	.probe	= simple_phy_probe,
	.driver = {
		.of_match_table	= of_match_ptr(simple_phy_of_match),
		.name		= "phy-simple",
	}
};
module_platform_driver(simple_phy_driver);

#endif /* CONFIG_PHY_SIMPLE_PDEV */

MODULE_DESCRIPTION("Simple PHY driver");
MODULE_AUTHOR("Alban Bedel <albeu@free.fr>");
MODULE_LICENSE("GPL");
