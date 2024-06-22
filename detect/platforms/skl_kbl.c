// SPDX-License-Identifier: GPL-2.0-only
#include "defs.h"

struct platform platform_skl_kbl_s_h = {
	.name = "Skylake/KabyLake(/rebranded as CoffeeLake) S/H",
	.pchs = (struct pch[]) {
		// Intel 100 Series and Intel C230 Series
		{"H110", 0xa143},
		{"H170", 0xa144},
		{"Z170", 0xa145},
		{"Q170", 0xa146},
		{"Q150", 0xa147},
		{"B150", 0xa148},
		{"C236", 0xa149},
		{"C232", 0xa14a},
		{"QM170", 0xa14d},
		{"HM170", 0xa14e},
		{"CM236", 0xa150},
		{"HM175", 0xa152},
		{"QM175", 0xa153},
		{"CM238", 0xa154},
		// Intel 200 Series (Including Intel X299), Intel Z370, Intel H310C, B365 Chipset
		{"Z270", 0xa2c5},
		{"H270", 0xa2c4},
		{"B250", 0xa2c8},
		{"Q250", 0xa2c7},
		{"Q270", 0xa2c6},
		{"X299", 0xa2d2},
		{"Z370", 0xa2c9},
		{"H310C", 0xa2ca},
		// datasheet missing B365??
		{0}
	},
	.global_pins = (struct pin[]) {
		{ "GPP_B13", "PLTRST#",    0xaf, 0xa8, 1<<13 },
		{0}
	},
	.espi_check_port = 0xc7,
	.espi_check_offset = 0x3418,
	.espi_check_bit = 1<<1,
	.lpc_pins = (struct pin[]) {
		{"GPP_A1",  "LAD0",        0xaf, 0xa0, 1<<1  },
		{"GPP_A2",  "LAD1",        0xaf, 0xa0, 1<<2  },
		{"GPP_A3",  "LAD2",        0xaf, 0xa0, 1<<3  },
		{"GPP_A4",  "LAD3",        0xaf, 0xa0, 1<<4  },
		{"GPP_A5",  "LFRAME#",     0xaf, 0xa0, 1<<5  },
		{"GPP_A6",  "SERIRQ",      0xaf, 0xa0, 1<<6  },
		{"GPP_A8",  "CLKRUN",      0xaf, 0xa0, 1<<8  },
		{"GPP_A9",  "CLKOUT_LPC0", 0xaf, 0xa0, 1<<9  },
		{0}
	},
	.espi_pins = (struct pin[]) {
		{"GPP_A1",  "ESPI_IO0",    0xaf, 0xa0, 1<<1  },
		{"GPP_A2",  "ESPI_IO1",    0xaf, 0xa0, 1<<2  },
		{"GPP_A3",  "ESPI_IO2",    0xaf, 0xa0, 1<<3  },
		{"GPP_A4",  "ESPI_IO3",    0xaf, 0xa0, 1<<4  },
		{"GPP_A5",  "ESPI_CS#",    0xaf, 0xa0, 1<<5  },
		{"GPP_A9",  "ESPI_CLK",    0xaf, 0xa0, 1<<9  },
		{"GPP_A14", "ESPI_RESET#", 0xaf, 0xa0, 1<<14 },
		{0}
	}
};

struct platform platform_skl_kbl_lp = {
	.name = "Skylake/KabyLake LP",
	.pchs = (struct pch[]) {
		// FIXME: add 6th gen LP

		// 7th Gen (U/Y) and 8th Gen (U Quad-Core)
		{"PCH-U Base", 0x9d53},
		{"PCH-U Premium", 0x9d58},
		{"PCH-Y Premium", 0x9d56},
		{"PCH-U with iHDCP 2.2 Base", 0x9d50},
		{"PCH-U with iHDCP 2.2 Premium", 0x9d4e},
		{"PCH-Y with iHDCP 2.2 Premium", 0x9d4b},
		{0}
	},
	.global_pins = (struct pin[]) {
		{ "GPP_B13", "PLTRST#",    0xaf, 0xa8, 1<<13 },
		{0}
	},
	.espi_check_port = 0xc7,
	.espi_check_offset = 0x3418,
	.espi_check_bit = 1<<1,
	.lpc_pins = (struct pin[]) {
		{"GPP_A1",  "LAD0",        0xaf, 0xa0, 1<<1  },
		{"GPP_A2",  "LAD1",        0xaf, 0xa0, 1<<2  },
		{"GPP_A3",  "LAD2",        0xaf, 0xa0, 1<<3  },
		{"GPP_A4",  "LAD3",        0xaf, 0xa0, 1<<4  },
		{"GPP_A5",  "LFRAME#",     0xaf, 0xa0, 1<<5  },
		{"GPP_A6",  "SERIRQ",      0xaf, 0xa0, 1<<6  },
		{"GPP_A8",  "CLKRUN",      0xaf, 0xa0, 1<<8  },
		{"GPP_A9",  "CLKOUT_LPC0", 0xaf, 0xa0, 1<<9  },
		{0}
	},
	.espi_pins = (struct pin[]) {
		{"GPP_A1",  "ESPI_IO0",    0xaf, 0xa0, 1<<1  },
		{"GPP_A2",  "ESPI_IO1",    0xaf, 0xa0, 1<<2  },
		{"GPP_A3",  "ESPI_IO2",    0xaf, 0xa0, 1<<3  },
		{"GPP_A4",  "ESPI_IO3",    0xaf, 0xa0, 1<<4  },
		{"GPP_A5",  "ESPI_CS#",    0xaf, 0xa0, 1<<5  },
		{"GPP_A9",  "ESPI_CLK",    0xaf, 0xa0, 1<<9  },
		{"GPP_A14", "ESPI_RESET#", 0xaf, 0xa0, 1<<14 },
		{0}
	}
};
