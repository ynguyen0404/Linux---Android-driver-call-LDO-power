/*
 * Driver Control LDO
 *
 * Copyright (C) 2007 8D Technologies inc.
 * Nguyen Van Huynh Y <nguyen.van.huynh.y@styl.solutions>
 * Copyright (C) 2008 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/slab.h>
#include <linux/regulator/consumer.h>

struct my_ldo_priv {
	struct regulator *my_ldo;
};

static struct my_ldo_priv *my_ldo_create(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct my_ldo_priv *priv;
	int ret, error;
	
	priv = devm_kzalloc(dev, sizeof(struct my_ldo_priv), GFP_KERNEL);
	if (!priv)
	{
		pr_err("my_ldo devm_kzalloc failed");
		return ERR_PTR(-ENOMEM);
	}

	priv->my_ldo = regulator_get(&pdev->dev, "my_ldo");
	if (IS_ERR(priv->my_ldo)) {
		error = PTR_ERR(priv->my_ldo);
		pr_err("%s: regulator get failed my_ldo rc=%d\n",
			__func__, error);
	}
	ret = regulator_enable(priv->my_ldo);
	if (ret) {
		pr_err("%s: Regulator my_ldo enable failed rc=%d\n",
			__func__, ret);
	}
	return priv;
}

static const struct of_device_id of_myldo_match[] = {
	{ .compatible = "power-config", },
	{},
};

MODULE_DEVICE_TABLE(of, of_myldo_match);

static int myldo_probe(struct platform_device *pdev)
{
	struct my_ldo_priv *priv;
	priv = my_ldo_create(pdev);
		if (IS_ERR(priv))
		{
			pr_err("my_ldo_create failed");
			return PTR_ERR(priv);
		}
	platform_set_drvdata(pdev, priv);
	return 0;
}

static void myldo_shutdown(struct platform_device *pdev)
{
	struct my_ldo_priv *priv = platform_get_drvdata(pdev);
	if (!IS_ERR(priv->my_ldo))
	{
        	regulator_put(priv->my_ldo);
	}

}

static struct platform_driver gpio_led_driver = {
	.probe		= myldo_probe,
	.shutdown	= myldo_shutdown,
	.driver		= {
		.name	= "myldo",
		.of_match_table = of_myldo_match,
	},
};

module_platform_driver(gpio_led_driver);

MODULE_AUTHOR("ynguyen, Nguyen Van Huynh Y <nguyen.van.huynh.y@styl.solutions>");
MODULE_DESCRIPTION("LDO control driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:myldo");
