/*
 * arch/arm/mach-at91/include/mach/uncompress.h
 *
 * Copyright (C) 2003 SAN People
 * Copyright (C) 2012 Jean-Christophe PLAGNIOL-VILLARD <plagnioj@jcrosoft.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __ASM_ARCH_UNCOMPRESS_H
#define __ASM_ARCH_UNCOMPRESS_H

#include <linux/io.h>
#include <linux/atmel_serial.h>

#include <mach/at91_dbgu.h>
#include <mach/cpu.h>

void __iomem *at91_uart;

#if !defined(CONFIG_ARCH_AT91X40)
static const u32 uarts_rm9200[] = {
	AT91_BASE_DBGU0,
	AT91RM9200_BASE_US0,
	AT91RM9200_BASE_US1,
	AT91RM9200_BASE_US2,
	AT91RM9200_BASE_US3,
	0,
};

static const u32 uarts_sam9260[] = {
	AT91_BASE_DBGU0,
	AT91SAM9260_BASE_US0,
	AT91SAM9260_BASE_US1,
	AT91SAM9260_BASE_US2,
	AT91SAM9260_BASE_US3,
	AT91SAM9260_BASE_US4,
	AT91SAM9260_BASE_US5,
	0,
};

static const u32 uarts_sam9261[] = {
	AT91_BASE_DBGU0,
	AT91SAM9261_BASE_US0,
	AT91SAM9261_BASE_US1,
	AT91SAM9261_BASE_US2,
	0,
};

static const u32 uarts_sam9263[] = {
	AT91_BASE_DBGU1,
	AT91SAM9263_BASE_US0,
	AT91SAM9263_BASE_US1,
	AT91SAM9263_BASE_US2,
	0,
};

static const u32 uarts_sam9g45[] = {
	AT91_BASE_DBGU1,
	AT91SAM9G45_BASE_US0,
	AT91SAM9G45_BASE_US1,
	AT91SAM9G45_BASE_US2,
	AT91SAM9G45_BASE_US3,
	0,
};

static const u32 uarts_sam9rl[] = {
	AT91_BASE_DBGU0,
	AT91SAM9RL_BASE_US0,
	AT91SAM9RL_BASE_US1,
	AT91SAM9RL_BASE_US2,
	AT91SAM9RL_BASE_US3,
	0,
};

static const u32 uarts_sam9x5[] = {
	AT91_BASE_DBGU0,
	AT91SAM9X5_BASE_USART0,
	AT91SAM9X5_BASE_USART1,
	AT91SAM9X5_BASE_USART2,
	0,
};

static inline const u32* decomp_soc_detect(u32 dbgu_base, u32* sram_base)
{
	u32 cidr, socid;

	cidr = __raw_readl(dbgu_base + AT91_DBGU_CIDR);
	socid = cidr & ~AT91_CIDR_VERSION;

	switch (socid) {
	case ARCH_ID_AT91RM9200:
		*sram_base = AT91RM9200_SRAM_BASE +
			     AT91RM9200_SRAM_SIZE - 8;
		return uarts_rm9200;

	case ARCH_ID_AT91SAM9G20:
	case ARCH_ID_AT91SAM9260:
		*sram_base = AT91SAM9260_SRAM_BASE +
			     AT91SAM9260_SRAM_SIZE - 8;
		return uarts_sam9260;

	case ARCH_ID_AT91SAM9261:
		*sram_base = AT91SAM9261_SRAM_BASE +
			     AT91SAM9261_SRAM_SIZE - 8;
		return uarts_sam9261;

	case ARCH_ID_AT91SAM9263:
		*sram_base = AT91SAM9263_SRAM0_BASE +
			     AT91SAM9263_SRAM0_SIZE - 8;
		return uarts_sam9263;

	case ARCH_ID_AT91SAM9G45:
		*sram_base = AT91SAM9G45_SRAM_BASE +
			     AT91SAM9G45_SRAM_SIZE - 8;
		return uarts_sam9g45;

	case ARCH_ID_AT91SAM9RL64:
		*sram_base = AT91SAM9RL_SRAM_BASE +
			     AT91SAM9RL_SRAM_SIZE - 8;
		return uarts_sam9rl;

	case ARCH_ID_AT91SAM9X5:
		*sram_base = AT91SAM9X5_SRAM_BASE +
			     AT91SAM9X5_SRAM_SIZE - 8;
		return uarts_sam9x5;
	}

	/* at91sam9g10 */
	if ((cidr & ~AT91_CIDR_EXT) == ARCH_ID_AT91SAM9G10) {
		*sram_base = AT91SAM9261_SRAM_BASE +
			     AT91SAM9261_SRAM_SIZE - 8;
		return uarts_sam9261;
	}
	/* at91sam9xe */
	else if ((cidr & AT91_CIDR_ARCH) == ARCH_FAMILY_AT91SAM9XE) {
		*sram_base = AT91SAM9260_SRAM_BASE +
			     AT91SAM9260_SRAM_SIZE - 8;
		return uarts_sam9260;
	}

	return NULL;
}

#define UART_BOOT_MAGIC 0x64656370	/* 'decp' */

static inline void arch_decomp_setup(void)
{
	int i = 0;
	const u32* usarts;
	u32 sram_base;

	usarts = decomp_soc_detect(AT91_BASE_DBGU0, &sram_base);

	if (!usarts)
		usarts = decomp_soc_detect(AT91_BASE_DBGU1, &sram_base);
	if (!usarts) {
		at91_uart = NULL;
		return;
	}

	if (__raw_readl((void __iomem *)sram_base) == UART_BOOT_MAGIC) {
		at91_uart = (void __iomem *)__raw_readl((void __iomem *)sram_base + 4);
		return;
	}

	do {
		/* physical address */
		at91_uart = (void __iomem *)usarts[i];

		if (__raw_readl(at91_uart + ATMEL_US_BRGR))
			return;
		i++;
	} while (usarts[i]);

	at91_uart = NULL;
}
#else
static inline void arch_decomp_setup(void)
{
	at91_uart = NULL;
}
#endif

/*
 * The following code assumes the serial port has already been
 * initialized by the bootloader.  If you didn't setup a port in
 * your bootloader then nothing will appear (which might be desired).
 *
 * This does not append a newline
 */
static void putc(int c)
{
	if (!at91_uart)
		return;

	while (!(__raw_readl(at91_uart + ATMEL_US_CSR) & ATMEL_US_TXRDY))
		barrier();
	__raw_writel(c, at91_uart + ATMEL_US_THR);
}

static inline void flush(void)
{
	if (!at91_uart)
		return;

	/* wait for transmission to complete */
	while (!(__raw_readl(at91_uart + ATMEL_US_CSR) & ATMEL_US_TXEMPTY))
		barrier();
}

#define arch_decomp_wdog()

#endif
