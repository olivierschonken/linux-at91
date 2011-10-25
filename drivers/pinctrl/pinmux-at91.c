/*
 * pinmux driver for CSR SiRFprimaII
 *
 * Copyright (c) 2011 Cambridge Silicon Radio Limited, a CSR plc group company.
 *
 * Licensed under GPLv2 or later.
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

#define DRIVER_NAME "pinmux-at91"

static const struct pinctrl_pin_desc at91_pads[] = {
	/* Control A */
	PINCTRL_PIN(AT91_PIN_PA0, "PA0"),
	PINCTRL_PIN(AT91_PIN_PA1, "PA1"),
	PINCTRL_PIN(AT91_PIN_PA2, "PA2"),
	PINCTRL_PIN(AT91_PIN_PA3, "PA3"),
	PINCTRL_PIN(AT91_PIN_PA4, "PA4"),
	PINCTRL_PIN(AT91_PIN_PA5, "PA5"),
	PINCTRL_PIN(AT91_PIN_PA6, "PA6"),
	PINCTRL_PIN(AT91_PIN_PA7, "PA7"),
	PINCTRL_PIN(AT91_PIN_PA8, "PA8"),
	PINCTRL_PIN(AT91_PIN_PA9, "PA9"),
	PINCTRL_PIN(AT91_PIN_PA10, "PA10"),
	PINCTRL_PIN(AT91_PIN_PA11, "PA11"),
	PINCTRL_PIN(AT91_PIN_PA12, "PA12"),
	PINCTRL_PIN(AT91_PIN_PA13, "PA13"),
	PINCTRL_PIN(AT91_PIN_PA14, "PA14"),
	PINCTRL_PIN(AT91_PIN_PA15, "PA15"),
	PINCTRL_PIN(AT91_PIN_PA16, "PA16"),
	PINCTRL_PIN(AT91_PIN_PA17, "PA17"),
	PINCTRL_PIN(AT91_PIN_PA18, "PA18"),
	PINCTRL_PIN(AT91_PIN_PA19, "PA19"),
	PINCTRL_PIN(AT91_PIN_PA20, "PA20"),
	PINCTRL_PIN(AT91_PIN_PA21, "PA21"),
	PINCTRL_PIN(AT91_PIN_PA22, "PA22"),
	PINCTRL_PIN(AT91_PIN_PA23, "PA23"),
	PINCTRL_PIN(AT91_PIN_PA24, "PA24"),
	PINCTRL_PIN(AT91_PIN_PA25, "PA25"),
	PINCTRL_PIN(AT91_PIN_PA26, "PA26"),
	PINCTRL_PIN(AT91_PIN_PA27, "PA27"),
	PINCTRL_PIN(AT91_PIN_PA28, "PA28"),
	PINCTRL_PIN(AT91_PIN_PA29, "PA29"),
	PINCTRL_PIN(AT91_PIN_PA30, "PA30"),
	PINCTRL_PIN(AT91_PIN_PA31, "PA31"),
	/* Control B */
	PINCTRL_PIN(AT91_PIN_PB0, "PB0"),
	PINCTRL_PIN(AT91_PIN_PB1, "PB1"),
	PINCTRL_PIN(AT91_PIN_PB2, "PB2"),
	PINCTRL_PIN(AT91_PIN_PB3, "PB3"),
	PINCTRL_PIN(AT91_PIN_PB4, "PB4"),
	PINCTRL_PIN(AT91_PIN_PB5, "PB5"),
	PINCTRL_PIN(AT91_PIN_PB6, "PB6"),
	PINCTRL_PIN(AT91_PIN_PB7, "PB7"),
	PINCTRL_PIN(AT91_PIN_PB8, "PB8"),
	PINCTRL_PIN(AT91_PIN_PB9, "PB9"),
	PINCTRL_PIN(AT91_PIN_PB10, "PB10"),
	PINCTRL_PIN(AT91_PIN_PB11, "PB11"),
	PINCTRL_PIN(AT91_PIN_PB12, "PB12"),
	PINCTRL_PIN(AT91_PIN_PB13, "PB13"),
	PINCTRL_PIN(AT91_PIN_PB14, "PB14"),
	PINCTRL_PIN(AT91_PIN_PB15, "PB15"),
	PINCTRL_PIN(AT91_PIN_PB16, "PB16"),
	PINCTRL_PIN(AT91_PIN_PB17, "PB17"),
	PINCTRL_PIN(AT91_PIN_PB18, "PB18"),
	PINCTRL_PIN(AT91_PIN_PB19, "PB19"),
	PINCTRL_PIN(AT91_PIN_PB20, "PB20"),
	PINCTRL_PIN(AT91_PIN_PB21, "PB21"),
	PINCTRL_PIN(AT91_PIN_PB22, "PB22"),
	PINCTRL_PIN(AT91_PIN_PB23, "PB23"),
	PINCTRL_PIN(AT91_PIN_PB24, "PB24"),
	PINCTRL_PIN(AT91_PIN_PB25, "PB25"),
	PINCTRL_PIN(AT91_PIN_PB26, "PB26"),
	PINCTRL_PIN(AT91_PIN_PB27, "PB27"),
	PINCTRL_PIN(AT91_PIN_PB28, "PB28"),
	PINCTRL_PIN(AT91_PIN_PB29, "PB29"),
	PINCTRL_PIN(AT91_PIN_PB30, "PB30"),
	PINCTRL_PIN(AT91_PIN_PB31, "PB31"),
	/* Control C */
	PINCTRL_PIN(AT91_PIN_PC0, "PC0"),
	PINCTRL_PIN(AT91_PIN_PC1, "PC1"),
	PINCTRL_PIN(AT91_PIN_PC2, "PC2"),
	PINCTRL_PIN(AT91_PIN_PC3, "PC3"),
	PINCTRL_PIN(AT91_PIN_PC4, "PC4"),
	PINCTRL_PIN(AT91_PIN_PC5, "PC5"),
	PINCTRL_PIN(AT91_PIN_PC6, "PC6"),
	PINCTRL_PIN(AT91_PIN_PC7, "PC7"),
	PINCTRL_PIN(AT91_PIN_PC8, "PC8"),
	PINCTRL_PIN(AT91_PIN_PC9, "PC9"),
	PINCTRL_PIN(AT91_PIN_PC10, "PC10"),
	PINCTRL_PIN(AT91_PIN_PC11, "PC11"),
	PINCTRL_PIN(AT91_PIN_PC12, "PC12"),
	PINCTRL_PIN(AT91_PIN_PC13, "PC13"),
	PINCTRL_PIN(AT91_PIN_PC14, "PC14"),
	PINCTRL_PIN(AT91_PIN_PC15, "PC15"),
	PINCTRL_PIN(AT91_PIN_PC16, "PC16"),
	PINCTRL_PIN(AT91_PIN_PC17, "PC17"),
	PINCTRL_PIN(AT91_PIN_PC18, "PC18"),
	PINCTRL_PIN(AT91_PIN_PC19, "PC19"),
	PINCTRL_PIN(AT91_PIN_PC20, "PC20"),
	PINCTRL_PIN(AT91_PIN_PC21, "PC21"),
	PINCTRL_PIN(AT91_PIN_PC22, "PC22"),
	PINCTRL_PIN(AT91_PIN_PC23, "PC23"),
	PINCTRL_PIN(AT91_PIN_PC24, "PC24"),
	PINCTRL_PIN(AT91_PIN_PC25, "PC25"),
	PINCTRL_PIN(AT91_PIN_PC26, "PC26"),
	PINCTRL_PIN(AT91_PIN_PC27, "PC27"),
	PINCTRL_PIN(AT91_PIN_PC28, "PC28"),
	PINCTRL_PIN(AT91_PIN_PC29, "PC29"),
	PINCTRL_PIN(AT91_PIN_PC30, "PC30"),
	PINCTRL_PIN(AT91_PIN_PC31, "PC31"),
	/* Control D */
	PINCTRL_PIN(AT91_PIN_PD0, "PD0"),
	PINCTRL_PIN(AT91_PIN_PD1, "PD1"),
	PINCTRL_PIN(AT91_PIN_PD2, "PD2"),
	PINCTRL_PIN(AT91_PIN_PD3, "PD3"),
	PINCTRL_PIN(AT91_PIN_PD4, "PD4"),
	PINCTRL_PIN(AT91_PIN_PD5, "PD5"),
	PINCTRL_PIN(AT91_PIN_PD6, "PD6"),
	PINCTRL_PIN(AT91_PIN_PD7, "PD7"),
	PINCTRL_PIN(AT91_PIN_PD8, "PD8"),
	PINCTRL_PIN(AT91_PIN_PD9, "PD9"),
	PINCTRL_PIN(AT91_PIN_PD10, "PD10"),
	PINCTRL_PIN(AT91_PIN_PD11, "PD11"),
	PINCTRL_PIN(AT91_PIN_PD12, "PD12"),
	PINCTRL_PIN(AT91_PIN_PD13, "PD13"),
	PINCTRL_PIN(AT91_PIN_PD14, "PD14"),
	PINCTRL_PIN(AT91_PIN_PD15, "PD15"),
	PINCTRL_PIN(AT91_PIN_PD16, "PD16"),
	PINCTRL_PIN(AT91_PIN_PD17, "PD17"),
	PINCTRL_PIN(AT91_PIN_PD18, "PD18"),
	PINCTRL_PIN(AT91_PIN_PD19, "PD19"),
	PINCTRL_PIN(AT91_PIN_PD20, "PD20"),
	PINCTRL_PIN(AT91_PIN_PD21, "PD21"),
	PINCTRL_PIN(AT91_PIN_PD22, "PD22"),
	PINCTRL_PIN(AT91_PIN_PD23, "PD23"),
	PINCTRL_PIN(AT91_PIN_PD24, "PD24"),
	PINCTRL_PIN(AT91_PIN_PD25, "PD25"),
	PINCTRL_PIN(AT91_PIN_PD26, "PD26"),
	PINCTRL_PIN(AT91_PIN_PD27, "PD27"),
	PINCTRL_PIN(AT91_PIN_PD28, "PD28"),
	PINCTRL_PIN(AT91_PIN_PD29, "PD29"),
	PINCTRL_PIN(AT91_PIN_PD30, "PD30"),
	PINCTRL_PIN(AT91_PIN_PD31, "PD31"),
	/* Control E */
	PINCTRL_PIN(AT91_PIN_PE0, "PE0"),
	PINCTRL_PIN(AT91_PIN_PE1, "PE1"),
	PINCTRL_PIN(AT91_PIN_PE2, "PE2"),
	PINCTRL_PIN(AT91_PIN_PE3, "PE3"),
	PINCTRL_PIN(AT91_PIN_PE4, "PE4"),
	PINCTRL_PIN(AT91_PIN_PE5, "PE5"),
	PINCTRL_PIN(AT91_PIN_PE6, "PE6"),
	PINCTRL_PIN(AT91_PIN_PE7, "PE7"),
	PINCTRL_PIN(AT91_PIN_PE8, "PE8"),
	PINCTRL_PIN(AT91_PIN_PE9, "PE9"),
	PINCTRL_PIN(AT91_PIN_PE10, "PE10"),
	PINCTRL_PIN(AT91_PIN_PE11, "PE11"),
	PINCTRL_PIN(AT91_PIN_PE12, "PE12"),
	PINCTRL_PIN(AT91_PIN_PE13, "PE13"),
	PINCTRL_PIN(AT91_PIN_PE14, "PE14"),
	PINCTRL_PIN(AT91_PIN_PE15, "PE15"),
	PINCTRL_PIN(AT91_PIN_PE16, "PE16"),
	PINCTRL_PIN(AT91_PIN_PE17, "PE17"),
	PINCTRL_PIN(AT91_PIN_PE18, "PE18"),
	PINCTRL_PIN(AT91_PIN_PE19, "PE19"),
	PINCTRL_PIN(AT91_PIN_PE20, "PE20"),
	PINCTRL_PIN(AT91_PIN_PE21, "PE21"),
	PINCTRL_PIN(AT91_PIN_PE22, "PE22"),
	PINCTRL_PIN(AT91_PIN_PE23, "PE23"),
	PINCTRL_PIN(AT91_PIN_PE24, "PE24"),
	PINCTRL_PIN(AT91_PIN_PE25, "PE25"),
	PINCTRL_PIN(AT91_PIN_PE26, "PE26"),
	PINCTRL_PIN(AT91_PIN_PE27, "PE27"),
	PINCTRL_PIN(AT91_PIN_PE28, "PE28"),
	PINCTRL_PIN(AT91_PIN_PE29, "PE29"),
	PINCTRL_PIN(AT91_PIN_PE30, "PE30"),
	PINCTRL_PIN(AT91_PIN_PE31, "PD31"),
};

static int at91_list_groups(struct pinctrl_dev *pctldev, unsigned selector)
{
	if (selector >= ARRAY_SIZE(at91_pads))
		return -EINVAL;
	return 0;
}

static const char *at91_get_group_name(struct pinctrl_dev *pctldev,
				       unsigned selector)
{
	if (selector >= ARRAY_SIZE(at91_pads))
		return NULL;
	return at91_pads[selector].name;
}


static int at91_get_group_pins(struct pinctrl_dev *pctldev, unsigned selector,
			       const unsigned **pins,
			       unsigned *num_pins)
{
	if (selector >= ARRAY_SIZE(at91_pads))
		return -EINVAL;
	*pins = &at91_pads[selector].number;
	*num_pins = 1;
	return 0;
}

static void at91_pin_dbg_show(struct pinctrl_dev *pctldev, struct seq_file *s,
		   unsigned offset)
{
	seq_printf(s, " " DRIVER_NAME);
}

static struct pinctrl_ops at91_pctrl_ops = {
	.list_groups = at91_list_groups,
	.get_group_name = at91_get_group_name,
	.get_group_pins = at91_get_group_pins,
	.pin_dbg_show = at91_pin_dbg_show,
};

struct at91_pin_func {
	char *name;
	int pull_up;
	int multi_drive;
	int peripherical;
};

#define PERIPHERAL_A 0
#define PERIPHERAL_B 1

static const char * const dbgu_rxd[] = { "PB14", "PB16" };
static struct at91_pin_func dbgu_rxd_funcs[] = {
	{ .peripherical = PERIPHERAL_A, },
	{ .peripherical = PERIPHERAL_A, }
};
static const char * const dbgu_txd[] = { "PB15" };
static struct at91_pin_func dbgu_txd_funcs[] = {
	{ .peripherical = PERIPHERAL_A, }
};
static const char * const uart0_rxd[] = { "PB4" };
static struct at91_pin_func uart0_rxd_funcs[] = {
	{ .peripherical = PERIPHERAL_A, }
};
static const char * const uart0_txd[] = { "PB5" };
static struct at91_pin_func uart0_txd_funcs[] = {
	{
		.peripherical = PERIPHERAL_A,
		.pull_up = 1,
	}
};
static const char * const uart0_cts[] = { "PB26" };
static struct at91_pin_func uart0_cts_funcs[] = {
	{ .peripherical = PERIPHERAL_A, }
};
static const char * const uart0_rts[] = { "PB27" };
static struct at91_pin_func uart0_rts_funcs[] = {
	{ .peripherical = PERIPHERAL_A, }
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

static int at91_pinmux_enable(struct pinctrl_dev *pmxdev, unsigned selector,
	unsigned group)
{
	const struct at91_pmx_func *pmx_func = &at91_pmx_functions[selector];
	const struct at91_pin_func *f = at91_get_group_func(pmx_func, at91_pads[group].name);

	if (f->peripherical) {
		at91_set_B_periph(group, f->pull_up);
	} else {
		at91_set_A_periph(group, f->pull_up);
	}
	at91_set_multi_drive(group, f->multi_drive);

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
	.list_functions = at91_pinmux_list_funcs,
	.enable = at91_pinmux_enable,
	.disable = at91_pinmux_disable,
	.get_function_name = at91_pinmux_get_func_name,
	.get_function_groups = at91_pinmux_get_groups,
	.gpio_request_enable = at91_pinmux_request_gpio,
};

static struct pinctrl_desc at91_pinmux_desc = {
	.name = DRIVER_NAME,
	.pins = at91_pads,
	.npins = ARRAY_SIZE(at91_pads),
	.maxpin = AT91_NUM_PADS - 1,
	.pctlops = &at91_pctrl_ops,
	.pmxops = &at91_pinmux_ops,
	.owner = THIS_MODULE,
};

static int __devinit at91_pinmux_probe(struct platform_device *pdev)
{
	int ret;
	struct pinctrl_dev *d;

	/* Now register the pin controller and all pins it handles */
	d = pinctrl_register(&at91_pinmux_desc, &pdev->dev, NULL);
	if (!d) {
		dev_err(&pdev->dev, "could not register AT91 pinmux driver\n");
		ret = -EINVAL;
		goto out_err;
	}

	dev_info(&pdev->dev, "initialized AT91 pinmux driver\n");

	return 0;

out_err:
	return ret;
}

static const struct of_device_id pinmux_ids[] = {
	{ .compatible = "atmel,at91rm9200-gpio-pinmux" },
	{}
};

static struct platform_driver at91_pinmux_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = pinmux_ids,
	},
	.probe = at91_pinmux_probe,
};

static int __init at91_pinmux_init(void)
{
	return platform_driver_register(&at91_pinmux_driver);
}
arch_initcall(at91_pinmux_init);

MODULE_AUTHOR("Jean-Christope PLAGNIOL-VILLARD <plagnioj@jcrosoft.com>");
MODULE_DESCRIPTION("AT91 pin control driver");
MODULE_LICENSE("GPL");
