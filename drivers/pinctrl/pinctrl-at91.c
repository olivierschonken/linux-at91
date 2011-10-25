/*
 * pin control driver for Atmel AT91
 *
 * Copyright (c) 2011-2012 Jean-Christophe PLAGNIOL-VILLARD <plagnioj@jcrosoft.com>
 *
 * Licensed under GPLv2.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/pinctrl/pinctrl.h>
#include <linux/pinctrl/pinmux.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/bitops.h>

#include <mach/hardware.h>
#include <mach/at91_pio.h>
#include <mach/gpio.h>

#define AT91_NUM_PADS	255

#define DRIVER_NAME "pinctrl-at91"

struct at91_pin {
	u32 pin;
	u32 mux;
	u32 pull;
	bool multidrive;
};

struct at91_pin_group {
	char *name;
	struct at91_pin *pins;
	int npins;
};

struct at91_pinctrl {
	u32 *mux_mask;
	u32 nb_mux;
	u32 nb_bank;
	struct pinctrl_desc pdesc;
	struct at91_pin_group *groups;
	int ngroups;
};

# if 0
static int at91_get_groups_count(struct pinctrl_dev *pctldev)
{
	struct at91_pinctrl *d = pinctrl_dev_get_drvdata(pctldev);

	return d->pdesc.npins;
}
#else
static int at91_list_groups(struct pinctrl_dev *pctldev, unsigned selector)
{
	struct at91_pinctrl *d = pinctrl_dev_get_drvdata(pctldev);

	if (selector >= d->pdesc.npins)
		return -EINVAL;
	return 0;
}
#endif

static const char *at91_get_group_name(struct pinctrl_dev *pctldev,
				       unsigned selector)
{
	struct at91_pinctrl *d = pinctrl_dev_get_drvdata(pctldev);

	return d->pdesc.pins[selector].name;
}

static int at91_get_group_pins(struct pinctrl_dev *pctldev, unsigned selector,
			       const unsigned **pins,
			       unsigned *num_pins)
{
	struct at91_pinctrl *d = pinctrl_dev_get_drvdata(pctldev);

	*pins = &d->pdesc.pins[selector].number;
	*num_pins = 1;
	return 0;
}

static void at91_pin_dbg_show(struct pinctrl_dev *pctldev, struct seq_file *s,
		   unsigned offset)
{
	seq_printf(s, " " DRIVER_NAME);
}

static struct pinctrl_ops at91_pctrl_ops = {
#if 1
	.list_groups = at91_list_groups,
#else
	.get_groups_count = at91_get_groups_count,
#endif
	.get_group_name = at91_get_group_name,
	.get_group_pins = at91_get_group_pins,
	.pin_dbg_show = at91_pin_dbg_show,
};

struct at91_pin_func {
	char *name;
	int pull_up;
	int multidrive;
	int mux;
};

#define PERIPHERAL_A 0
#define PERIPHERAL_B 1

static const char * const dbgu_rxd[] = { "PB14", "PB16" };
static struct at91_pin_func dbgu_rxd_funcs[] = {
	{ .mux = PERIPHERAL_A, },
	{ .mux = PERIPHERAL_A, }
};
static const char * const dbgu_txd[] = { "PB15" };
static struct at91_pin_func dbgu_txd_funcs[] = {
	{ .mux = PERIPHERAL_A, }
};
static const char * const uart0_rxd[] = { "PB4" };
static struct at91_pin_func uart0_rxd_funcs[] = {
	{ .mux = PERIPHERAL_A, }
};
static const char * const uart0_txd[] = { "PB5" };
static struct at91_pin_func uart0_txd_funcs[] = {
	{
		.mux = PERIPHERAL_A,
		.pull_up = 1,
	}
};
static const char * const uart0_cts[] = { "PB26" };
static struct at91_pin_func uart0_cts_funcs[] = {
	{ .mux = PERIPHERAL_A, }
};
static const char * const uart0_rts[] = { "PB27" };
static struct at91_pin_func uart0_rts_funcs[] = {
	{ .mux = PERIPHERAL_A, }
};

struct at91_pmx_func {
	const char *name;
	const char * const *groups;
	const unsigned num_groups;
	const struct at91_pin_func *funcs;
};

#define AT91_PMX_FUNCTION(n, g)	\
	{					\
		.name = n,			\
		.groups = g,			\
		.num_groups = ARRAY_SIZE(g),	\
		.funcs = g##_funcs,		\
	}

static const struct at91_pmx_func at91_pmx_functions[] = {
	AT91_PMX_FUNCTION("dbgu_rxd", dbgu_rxd),
	AT91_PMX_FUNCTION("dbgu_txd", dbgu_txd),
	AT91_PMX_FUNCTION("uart0_rxd", uart0_rxd),
	AT91_PMX_FUNCTION("uart0_txd", uart0_txd),
	AT91_PMX_FUNCTION("uart0_cts", uart0_cts),
	AT91_PMX_FUNCTION("uart0_rts", uart0_rts),
};

static const struct at91_pin_func* at91_get_group_func(const struct at91_pmx_func *f, const char* name)
{
	int i;

	for (i = 0; i < f->num_groups; i++) {
		if (strcmp(f->groups[i], name) == 0)
			return &f->funcs[i];
	}

	return NULL;
}

static int at91_pinmux_enable(struct pinctrl_dev *pctldev, unsigned selector,
	unsigned group)
{
#if 0
	const struct at91_pmx_func *pmx_func = &at91_pmx_functions[selector];
#else
	const struct at91_pmx_func *pmx_func;
#endif
	struct at91_pinctrl *d = pinctrl_dev_get_drvdata(pctldev);
	const struct at91_pin_func *f;
	
	pr_info("selector = %d, group = %d\n", selector, group);

	return 0;

	f = at91_get_group_func(pmx_func, d->pdesc.pins[group].name);

	if (f->mux) {
		at91_set_B_periph(group, f->pull_up);
	} else {
		at91_set_A_periph(group, f->pull_up);
	}
	at91_set_multi_drive(group, f->multidrive);

	return 0;
}

static void at91_pinmux_disable(struct pinctrl_dev *pmxdev, unsigned selector,
	unsigned group)
{
	at91_set_GPIO_periph(group, 0);
}

static int at91_pinmux_list_funcs(struct pinctrl_dev *pmxdev, unsigned selector)
{
	if (selector >= ARRAY_SIZE(at91_pmx_functions))
		return -EINVAL;
	return 0;
}

static const char *at91_pinmux_get_func_name(struct pinctrl_dev *pctldev,
					  unsigned selector)
{
	return at91_pmx_functions[selector].name;
}

static int at91_pinmux_get_groups(struct pinctrl_dev *pctldev, unsigned selector,
			       const char * const **groups,
			       unsigned * const num_groups)
{
	*groups = at91_pmx_functions[selector].groups;
	*num_groups = at91_pmx_functions[selector].num_groups;
	return 0;
}

static int at91_pinmux_request_gpio(struct pinctrl_dev *pmxdev,
	struct pinctrl_gpio_range *range, unsigned offset)
{
	return 0;
}

static struct pinmux_ops at91_pinmux_ops = {
	.list_functions		= at91_pinmux_list_funcs,
	.enable			= at91_pinmux_enable,
	.disable		= at91_pinmux_disable,
	.get_function_name	= at91_pinmux_get_func_name,
	.get_function_groups	= at91_pinmux_get_groups,
	.gpio_request_enable	= at91_pinmux_request_gpio,
};

static struct at91_pinctrl at91_pctrl = {
	.pdesc = {
		.name		= DRIVER_NAME,
		.pctlops	= &at91_pctrl_ops,
		.pmxops		= &at91_pinmux_ops,
		.owner		= THIS_MODULE,
	},
};

static int inline at91_pinctrl_alloc_pin(struct platform_device *pdev)
{
	int i, j;
	struct pinctrl_pin_desc *at91_pads;

	at91_pads = devm_kzalloc(&pdev->dev,
		sizeof(*at91_pads) * at91_pctrl.pdesc.npins, GFP_KERNEL);
	if (!at91_pads) {
		dev_err(&pdev->dev, "could not alloc pins\n");
		return -ENOMEM;
	}

	at91_pctrl.pdesc.pins = at91_pads;

	for (i = 0; i < at91_pctrl.nb_bank; i++) {
		for (j = 0; j < 32; j++, at91_pads++) {
			at91_pads->number = i * 32 + j;
			at91_pads->name =
				kasprintf(GFP_KERNEL, "P%c%d", 'A' + i, j);
			dev_dbg(&pdev->dev, "alloc pin %d on bank %c, '%s' => %d\n",
				j, 'A' + i, at91_pads->name, at91_pads->number);
		}
	}

	return 0;
}

static int inline at91_pinctrl_probe_mux_mask(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	int size = at91_pctrl.nb_mux * at91_pctrl.nb_bank;

	at91_pctrl.mux_mask = devm_kzalloc(&pdev->dev,
		sizeof(u32) * size, GFP_KERNEL);
	if (!at91_pctrl.mux_mask) {
		dev_err(&pdev->dev, "could not alloc mux_mask\n");
		return -ENOMEM;
	}

	return of_property_read_u32_array(np, "atmel,mux-mask",
					  at91_pctrl.mux_mask, size);
}

static int __devinit at91_pinctrl_parse_pin(struct platform_device *pdev,
		struct device_node *np, struct at91_pin *p)
{
	int ret;
	u32 bank;
	u32 mask;

	ret = of_property_read_u32(np, "atmel,pin-id", &p->pin);
	if (ret) {
		dev_err(&pdev->dev, "could not found atmel,pin-id\n");
		return ret;
	}

	bank = p->pin / 32;
	mask = 1 << (p->pin % 32);

	ret = of_property_read_u32(np, "atmel,mux", &p->mux);
	if (ret) {
		dev_err(&pdev->dev, "could not found atmel,mux\n");
		return ret;
	}

	if (p->mux >= at91_pctrl.nb_mux) {
		dev_err(&pdev->dev, "mux (%d) > nb mux (%d)\n",
			p->mux, at91_pctrl.nb_mux);
		return -EINVAL;
	}

	if (!(at91_pctrl.mux_mask[bank * 2 + p->mux] & mask)) {
		dev_err(&pdev->dev, "mux %d not supported for pin %d\n",
			p->mux, p->pin);
		return -EINVAL;
	}

	of_property_read_u32(np, "atmel,pull", &p->pull);
	if (p->pull > 2) {
		dev_err(&pdev->dev, "pull %d not supported for pin %d\n",
			p->pull, p->pin);
		return -EINVAL;
	}
	p->multidrive = of_property_read_bool(np, "atmel,multidrive");

	return 0;
}

static int __devinit at91_pinctrl_parse_group(struct platform_device *pdev,
		struct device_node *np, struct at91_pin_group *g)
{
	struct device_node *child;
	int i = 0;
	int ret;

	g->npins = 0;

	/* count the pin in the group */
	for_each_child_of_node(np, child) {
		g->npins++;
	}

	g->pins = devm_kzalloc(&pdev->dev, g->npins * sizeof(*g->pins),
			       GFP_KERNEL);

	if (!g->pins)
		return -ENOMEM;

	for_each_child_of_node(np, child) {
		ret = at91_pinctrl_parse_pin(pdev, child, &g->pins[i++]);
		if (ret)
			return ret;
	}

	return 0;
}

static int __devinit at91_pinctrl_probe_child(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct device_node *child;
	int i = 0;
	int ret;

	at91_pctrl.ngroups = 0;

	child = of_get_next_child(np, NULL);
	if (!child) {
		dev_err(&pdev->dev, "no group is defined\n");
		return -ENOENT;
	}

	for_each_child_of_node(np, child) {
		pr_info("%s: %d: '%s'\n", __func__, at91_pctrl.ngroups, child->name);
		at91_pctrl.ngroups++;
	}

	at91_pctrl.groups = devm_kzalloc(&pdev->dev,
		sizeof(*at91_pctrl.groups) * at91_pctrl.ngroups, GFP_KERNEL);

	if (!at91_pctrl.groups) {
		dev_err(&pdev->dev, "could not alloc groups\n");
		return -ENOMEM;
	}

	for_each_child_of_node(np, child) {
		pr_info("%s: %d: '%s'\n", __func__, at91_pctrl.ngroups, child->name);
		ret = at91_pinctrl_parse_group(pdev, child,
					       &at91_pctrl.groups[i++]);
		if (ret)
			return ret;
	}

	return 0;
}

static int __devinit at91_pinctrl_probe(struct platform_device *pdev)
{
	int ret = -ENXIO;
	struct pinctrl_dev *d;
	struct device_node *np = pdev->dev.of_node;

	if (!np) {
		dev_err(&pdev->dev, "DT only driver\n");
		goto out_err;
	}

	if (of_property_read_u32(np, "atmel,nb-bank", &at91_pctrl.nb_bank)) {
		dev_err(&pdev->dev, "missing atmel,nb-bank\n");
		goto out_err;
	}

	at91_pctrl.pdesc.npins = at91_pctrl.nb_bank * 32;
	at91_pctrl.nb_mux = 2;

	ret = at91_pinctrl_alloc_pin(pdev);
	if (ret)
		goto out_err;

	ret = at91_pinctrl_probe_mux_mask(pdev);
	if (ret)
		goto out_err;

	ret = at91_pinctrl_probe_child(pdev);
	if (ret)
		goto out_err;

	platform_set_drvdata(pdev, &at91_pctrl);

	/* Now register the pin controller and all pins it handles */
	d = pinctrl_register(&at91_pctrl.pdesc, &pdev->dev, NULL);
	if (!d) {
		dev_err(&pdev->dev, "could not register AT91 pin controller driver\n");
		ret = -EINVAL;
		goto out_err;
	}

	dev_info(&pdev->dev, "initialized AT91 pin controller driver\n");

	return 0;

out_err:
	dev_err(&pdev->dev, "AT91 pin controller driver err (%d)\n", ret);
	return ret;
}

static const struct of_device_id pinctrl_ids[] = {
	{ .compatible = "atmel,at91rm9200-pinctrl" },
	{}
};

static struct platform_driver at91_pinctrl_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = pinctrl_ids,
	},
	.probe = at91_pinctrl_probe,
};

static int __init at91_pinctrl_init(void)
{
	return platform_driver_register(&at91_pinctrl_driver);
}
arch_initcall(at91_pinctrl_init);

MODULE_AUTHOR("Jean-Christope PLAGNIOL-VILLARD <plagnioj@jcrosoft.com>");
MODULE_DESCRIPTION("AT91 pin control driver");
MODULE_LICENSE("GPL");
