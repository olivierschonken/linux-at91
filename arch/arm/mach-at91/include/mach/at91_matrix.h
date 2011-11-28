/*
 * Copyright (C) 2011 Jean-Christophe PLAGNIOL-VILLARD <plagnioj@jcrosoft.com>
 *
 * Under GPLv2
 */

#ifndef __MACH_AT91_MATRIX_H__
#define __MACH_AT91_MATRIX_H__

#ifndef __ASSEMBLY__
extern void __iomem *at91_matrix_base;

#define at91_matrix_read(field) \
	__raw_readl(at91_matrix_base + field)

#define at91_matrix_write(field, value) \
	__raw_writel(value, at91_matrix_base + field);

#define AT91_EBI_NONE		(0 << 0)
#define AT91_EBI_NFD0_ON_D0	(1 << 0)
#define AT91_EBI_NFD0_ON_D16	(1 << 2)
int at91_matrix_configure_nand(int ebi, int mode);
int at91_matrix_configure_cf(int ebi, int cs);
int at91_matrix_configure_smc(int ebi, int cs);

#define AT91_EBI_VDDIOMSEL_3_3V	0
#define AT91_EBI_VDDIOMSEL_1_8V	1
int at91_matrix_configure_voltage(int ebi, int mode);

#else
.extern at91_matrix_base
#endif

#endif /* __MACH_AT91_MATRIX_H__ */
