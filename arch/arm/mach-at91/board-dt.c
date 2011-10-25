/*
 *  Setup code for AT91SAM Evaluation Kits with Device Tree support
 *
 *  Covers: * AT91SAM9G45-EKES  board
 *          * AT91SAM9M10-EKES  board
 *          * AT91SAM9M10G45-EK board
 *
 *  Copyright (C) 2011 Atmel,
 *                2011 Nicolas Ferre <nicolas.ferre@atmel.com>
 *
 * Licensed under GPLv2 or later.
 */

#include <linux/types.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/irqdomain.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>

#include <mach/hardware.h>
#include <mach/board.h>
#include <mach/system_rev.h>
#include <mach/at91sam9_smc.h>

#include <asm/setup.h>
#include <asm/irq.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <linux/pinctrl/machine.h>
#include <linux/pinctrl/pinmux.h>

#include "sam9_smc.h"
#include "generic.h"

/* Pinmux settings */
static struct pinmux_map usb_a9g20_pinmux_map[] = {
	PINMUX_MAP_PRIMARY_SYS_HOG("DGBU_RX", "dbgu_rxd", NULL),
	PINMUX_MAP_PRIMARY_SYS_HOG("DGBU_TX", "dbgu_txd", NULL),
};

static struct pinmux_map sam9m10g45ek_pinmux_map[] = {
	PINMUX_MAP_PRIMARY_SYS_HOG("DGBU_RX", "dbgu_rxd", NULL),
	PINMUX_MAP_PRIMARY_SYS_HOG("DGBU_TX", "dbgu_txd", NULL),
};

void usb_a9g20_init(void)
{
	pinmux_register_mappings(usb_a9g20_pinmux_map,
				 ARRAY_SIZE(usb_a9g20_pinmux_map));
}
void sam9gm10g45ek_init(void)
{
	pinmux_register_mappings(sam9m10g45ek_pinmux_map,
				 ARRAY_SIZE(sam9m10g45ek_pinmux_map));
}

static const struct of_device_id at91_board_of_match[] __initconst = {
	{ .compatible = "atmel,at91sam9m10g45ek", .data = sam9gm10g45ek_init, },
	{ .compatible = "calao,usb-a9g20", .data = usb_a9g20_init, },
	{ /* sentinel */ }
};

static void __init early_dt_init(void)
{
	struct device_node *node;
	const struct of_device_id *of_id;
	void (*func)(void);

	node = of_find_matching_node(NULL, at91_board_of_match);
	if (node) {
		of_id = of_match_node(at91_board_of_match, node);
		func = of_id->data;
		func();
		of_node_put(node);
	}
}

static void __init ek_init_early(void)
{
	/* Initialize processor: 12.000 MHz crystal */
	at91_initialize(12000000);

	early_dt_init();

	/* DGBU on ttyS0. (Rx & Tx only) */
	at91_register_uart(0, 0, 0);

	/* set serial console to ttyS0 (ie, DBGU) */
	at91_set_serial_console(0);
}

/* det_pin is not connected */
static struct atmel_nand_data __initdata ek_nand_data = {
	.ale		= 21,
	.cle		= 22,
	.rdy_pin	= AT91_PIN_PC8,
	.enable_pin	= AT91_PIN_PC14,
};

static void __init ek_add_device_nand(void)
{
	at91_add_device_nand(&ek_nand_data);
}

static const struct of_device_id aic_of_match[] __initconst = {
	{ .compatible = "atmel,at91rm9200-aic", },
	{},
};

static void __init at91_dt_init_irq(void)
{
	irq_domain_generate_simple(aic_of_match, 0xfffff000, 0);
	at91_init_irq_default();
}

static void __init at91_dt_device_init(void)
{
	of_platform_populate(NULL, of_default_bus_match_table, NULL, NULL);

	/* NAND */
	ek_add_device_nand();

	printk("0x%x\n", gpio_request(AT91_PIN_PB31, ""));

	at91_set_A_periph(AT91_PIN_PB30, 0);

	printk("0x%x\n", gpio_request(AT91_PIN_PB30, ""));
}

static const char *at91_dt_board_compat[] __initdata = {
	"atmel,at91sam9m10g45ek",
	"calao,usb-a9g20",
	NULL
};

DT_MACHINE_START(at91sam9m10g45ek_dt, "Atmel AT91SAM (Device Tree)")
	/* Maintainer: Atmel */
	.timer		= &at91sam926x_timer,
	.map_io		= at91_map_io,
	.init_early	= ek_init_early,
	.init_irq	= at91_dt_init_irq,
	.init_machine	= at91_dt_device_init,
	.dt_compat	= at91_dt_board_compat,
MACHINE_END
