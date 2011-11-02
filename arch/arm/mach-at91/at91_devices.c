/*
 * arch/arm/mach-at91/at91cap9_devices.c
 *
 *  Copyright (C) 2007 Stelian Pop <stelian.pop@leadtechdesign.com>
 *  Copyright (C) 2007 Lead Tech Design <www.leadtechdesign.com>
 *  Copyright (C) 2007 Atmel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#include <mach/gpio.h>

#include "generic.h"

void at91cap9_configure_dbgu_pins(void)
{
	at91_set_A_periph(AT91_PIN_PC30, 0);		/* DRXD */
	at91_set_A_periph(AT91_PIN_PC31, 1);		/* DTXD */
}

void at91cap9_configure_usart0_pins(unsigned pins)
{
	at91_set_A_periph(AT91_PIN_PA22, 1);		/* TXD0 */
	at91_set_A_periph(AT91_PIN_PA23, 0);		/* RXD0 */

	if (pins & ATMEL_UART_RTS)
		at91_set_A_periph(AT91_PIN_PA24, 0);	/* RTS0 */
	if (pins & ATMEL_UART_CTS)
		at91_set_A_periph(AT91_PIN_PA25, 0);	/* CTS0 */
}

void at91cap9_configure_usart1_pins(unsigned pins)
{
	at91_set_A_periph(AT91_PIN_PD0, 1);		/* TXD1 */
	at91_set_A_periph(AT91_PIN_PD1, 0);		/* RXD1 */

	if (pins & ATMEL_UART_RTS)
		at91_set_B_periph(AT91_PIN_PD7, 0);	/* RTS1 */
	if (pins & ATMEL_UART_CTS)
		at91_set_B_periph(AT91_PIN_PD8, 0);	/* CTS1 */
}

void at91cap9_configure_usart2_pins(unsigned pins)
{
	at91_set_A_periph(AT91_PIN_PD2, 1);		/* TXD2 */
	at91_set_A_periph(AT91_PIN_PD3, 0);		/* RXD2 */

	if (pins & ATMEL_UART_RTS)
		at91_set_B_periph(AT91_PIN_PD5, 0);	/* RTS2 */
	if (pins & ATMEL_UART_CTS)
		at91_set_B_periph(AT91_PIN_PD6, 0);	/* CTS2 */
}

void at91rm9200_configure_dbgu_pins(void)
{
	at91_set_A_periph(AT91_PIN_PA30, 0);		/* DRXD */
	at91_set_A_periph(AT91_PIN_PA31, 1);		/* DTXD */
}

void at91rm9200_configure_usart0_pins(unsigned pins)
{
	at91_set_A_periph(AT91_PIN_PA17, 1);		/* TXD0 */
	at91_set_A_periph(AT91_PIN_PA18, 0);		/* RXD0 */

	if (pins & ATMEL_UART_CTS)
		at91_set_A_periph(AT91_PIN_PA20, 0);	/* CTS0 */

	if (pins & ATMEL_UART_RTS) {
		/*
		 * AT91RM9200 Errata #39 - RTS0 is not internally connected to PA21.
		 *  We need to drive the pin manually.  Default is off (RTS is active low).
		 */
		at91_set_gpio_output(AT91_PIN_PA21, 1);
	}
}

void at91rm9200_configure_usart1_pins(unsigned pins)
{
	at91_set_A_periph(AT91_PIN_PB20, 1);		/* TXD1 */
	at91_set_A_periph(AT91_PIN_PB21, 0);		/* RXD1 */

	if (pins & ATMEL_UART_RI)
		at91_set_A_periph(AT91_PIN_PB18, 0);	/* RI1 */
	if (pins & ATMEL_UART_DTR)
		at91_set_A_periph(AT91_PIN_PB19, 0);	/* DTR1 */
	if (pins & ATMEL_UART_DCD)
		at91_set_A_periph(AT91_PIN_PB23, 0);	/* DCD1 */
	if (pins & ATMEL_UART_CTS)
		at91_set_A_periph(AT91_PIN_PB24, 0);	/* CTS1 */
	if (pins & ATMEL_UART_DSR)
		at91_set_A_periph(AT91_PIN_PB25, 0);	/* DSR1 */
	if (pins & ATMEL_UART_RTS)
		at91_set_A_periph(AT91_PIN_PB26, 0);	/* RTS1 */
}

void at91rm9200_configure_usart2_pins(unsigned pins)
{
	at91_set_A_periph(AT91_PIN_PA22, 0);		/* RXD2 */
	at91_set_A_periph(AT91_PIN_PA23, 1);		/* TXD2 */

	if (pins & ATMEL_UART_CTS)
		at91_set_B_periph(AT91_PIN_PA30, 0);	/* CTS2 */
	if (pins & ATMEL_UART_RTS)
		at91_set_B_periph(AT91_PIN_PA31, 0);	/* RTS2 */
}

void at91rm9200_configure_usart3_pins(unsigned pins)
{
	at91_set_B_periph(AT91_PIN_PA5, 1);		/* TXD3 */
	at91_set_B_periph(AT91_PIN_PA6, 0);		/* RXD3 */

	if (pins & ATMEL_UART_CTS)
		at91_set_B_periph(AT91_PIN_PB1, 0);	/* CTS3 */
	if (pins & ATMEL_UART_RTS)
		at91_set_B_periph(AT91_PIN_PB0, 0);	/* RTS3 */
}


