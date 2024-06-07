// SPDX-License-Identifier: GPL-2.0-only
#include "defs.h"

struct platform platform_cfl_s_h = {
	.name = "CoffeeLake S/H",
	.pchs = (struct pch[]) {
		// Intel 300 Series and Intel C240 Series
		{"Q370", 0xa306},
		{"H370", 0xa304},
		{"Z390", 0xa305},
		{"B360", 0xa308},
		{"H310", 0xa303},
		{"C242", 0xa30a},
		{"C246", 0xa309},
		{"HM370", 0xa30d},
		{"QM370", 0xa30c},
		{"CM246", 0xa30e},
	},
	.global_pins = (struct pin[]) {
		{ "GPP_B13", "PLTRST#",    0x6e, 0x88, 1<<13 },
		{0}
	},
	.lpc_pins = (struct pin[]) {
		{"GPP_A1",  "LAD0",        0x6e, 0x80, 1<<1  },
		{"GPP_A2",  "LAD1",        0x6e, 0x80, 1<<2  },
		{"GPP_A3",  "LAD2",        0x6e, 0x80, 1<<3  },
		{"GPP_A4",  "LAD3",        0x6e, 0x80, 1<<4  },
		{"GPP_A5",  "LFRAME#",     0x6e, 0x80, 1<<5  },
		{"GPP_A6",  "SERIRQ",      0x6e, 0x80, 1<<6  },
		{"GPP_A8",  "CLKRUN",      0x6e, 0x80, 1<<8  },
		{"GPP_A9",  "CLKOUT_LPC0", 0x6e, 0x80, 1<<9  },
		{0}
	},
	.espi_pins = (struct pin[]) {
		{"GPP_A1",  "ESPI_IO0",    0x6e, 0x80, 1<<1  },
		{"GPP_A2",  "ESPI_IO1",    0x6e, 0x80, 1<<2  },
		{"GPP_A3",  "ESPI_IO2",    0x6e, 0x80, 1<<3  },
		{"GPP_A4",  "ESPI_IO3",    0x6e, 0x80, 1<<4  },
		{"GPP_A5",  "ESPI_CS#",    0x6e, 0x80, 1<<5  },
		{"GPP_A9",  "ESPI_CLK",    0x6e, 0x80, 1<<9  },
		{"GPP_A14", "ESPI_RESET#", 0x6e, 0x80, 1<<14 },
		{0}
	}
};

// FIXME: add Coffee Lake (and Amber Lake) LP
