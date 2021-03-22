/*
 * linux-3.10/drivers/video/sunxi/disp2/disp/de/lowlevel_sun50iw1/de_clock.c
 *
 * Copyright (c) 2007-2017 Allwinnertech Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "de_clock.h"

static uintptr_t de_base = 0;

static de_clk_para de_clk_tbl[] =
{
	/*              mod_id    div    bus     reset     dram      gate      div    */
	DE_TOP_CFG(DE_CLK_CORE0,   1,  0x04, 0, 0x08, 0, 0x00, 32, 0x00, 0, 0x0c,  0, 4)
	DE_TOP_CFG(DE_CLK_CORE1,   1,  0x04, 1, 0x08, 1, 0x00, 32, 0x00, 1, 0x0c,  4, 4)
	DE_TOP_CFG(DE_CLK_WB,      1,  0x04, 2, 0x08, 2, 0x00, 32, 0x00, 2, 0x0c,  8, 4)
};

static s32 de_clk_set_div(u32 clk_no, u32 div)
{
	u32 i = 0;
	u32 reg_val;

	for (i=0; i<(sizeof(de_clk_tbl)/sizeof(de_clk_para)); i++) {
		if ((de_clk_tbl[i].clk_no == clk_no)) {
			reg_val = readl(de_clk_tbl[i].mod_div_adr + de_base);
			reg_val = SET_BITS(de_clk_tbl[i].mod_div_shift, de_clk_tbl[i].mod_div_width, reg_val, (div - 1));
			writel(reg_val, de_clk_tbl[i].mod_div_adr + de_base);

			return 0;
		}
	}

	__wrn("clk %d not foundis not initializd\n", clk_no);

	return -1;
}

static s32 __de_clk_enable(u32 clk_no, u32 enable)
{
	u32 i;
	u32 reg_val;

	for (i=0; i<(sizeof(de_clk_tbl)/sizeof(de_clk_para)); i++) {
		if ((de_clk_tbl[i].clk_no == clk_no)) {
			if (enable)
			{
				/* set clk div */
				de_clk_set_div(clk_no, de_clk_tbl[i].div);

				if (de_clk_tbl[i].ahb_reset_shift < 32)
				{
					reg_val = readl(de_clk_tbl[i].ahb_reset_adr + de_base);
					reg_val = SET_BITS(de_clk_tbl[i].ahb_reset_shift, 1, reg_val, 1);
					writel(reg_val, de_clk_tbl[i].ahb_reset_adr + de_base);
					__inf("clk %d reset enable\n", clk_no);
				}

				if (de_clk_tbl[i].ahb_gate_shift < 32)
				{
					reg_val = readl(de_clk_tbl[i].ahb_gate_adr + de_base);
					reg_val = SET_BITS(de_clk_tbl[i].ahb_gate_shift, 1, reg_val, 1);
					writel(reg_val, de_clk_tbl[i].ahb_gate_adr + de_base);
					__inf("clk %d gate enable\n", clk_no);
				}

				if (de_clk_tbl[i].mod_enable_shift < 32)
				{
					reg_val = readl(de_clk_tbl[i].mod_adr + de_base);
					reg_val = SET_BITS(de_clk_tbl[i].mod_enable_shift, 1, reg_val, 1);
					writel(reg_val, de_clk_tbl[i].mod_adr + de_base);
					__inf("clk %d mod enable\n", clk_no);
				}

				if (de_clk_tbl[i].dram_gate_shift < 32)
				{
					reg_val = readl(de_clk_tbl[i].dram_gate_adr + de_base);
					reg_val = SET_BITS(de_clk_tbl[i].dram_gate_shift, 1, reg_val, 1);
					writel(reg_val, de_clk_tbl[i].dram_gate_adr + de_base);
					__inf("clk %d dram enable\n", clk_no);
				}
			}
			else
			{
				if (de_clk_tbl[i].dram_gate_shift < 32)
				{
					reg_val = readl(de_clk_tbl[i].dram_gate_adr + de_base);
					reg_val = SET_BITS(de_clk_tbl[i].dram_gate_shift, 1, reg_val, 0);
					writel(reg_val, de_clk_tbl[i].dram_gate_adr + de_base);
					__inf("clk %d dram disable\n", clk_no);
				}

				if (de_clk_tbl[i].mod_enable_shift < 32)
				{
					reg_val = readl(de_clk_tbl[i].mod_adr + de_base);
					reg_val = SET_BITS(de_clk_tbl[i].mod_enable_shift, 1, reg_val, 0);
					writel(reg_val, de_clk_tbl[i].mod_adr + de_base);
					__inf("clk %d mod disable\n", clk_no);
				}

				if (de_clk_tbl[i].ahb_gate_shift < 32)
				{
					reg_val = readl(de_clk_tbl[i].ahb_gate_adr + de_base);
					reg_val = SET_BITS(de_clk_tbl[i].ahb_gate_shift, 1, reg_val, 0);
					writel(reg_val, de_clk_tbl[i].ahb_gate_adr + de_base);
					__inf("clk %d gate disable\n", clk_no);
				}

				if (de_clk_tbl[i].ahb_reset_shift < 32)
				{
					reg_val = readl(de_clk_tbl[i].ahb_reset_adr + de_base);
					reg_val = SET_BITS(de_clk_tbl[i].ahb_reset_shift, 1, reg_val, 0);
					writel(reg_val, de_clk_tbl[i].ahb_reset_adr + de_base);
					__inf("clk %d reset disable\n", clk_no);
				}
			}
		}
	}

	return 0;
}

s32 de_clk_enable(u32 clk_no)
{
	return __de_clk_enable(clk_no, 1);
}

s32 de_clk_disable(u32 clk_no)
{
	return __de_clk_enable(clk_no, 0);
}

s32 de_clk_set_reg_base(uintptr_t reg_base)
{
	de_base = reg_base;

	return 0;
}

