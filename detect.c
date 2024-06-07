// SPDX-License-Identifier: GPL-2.0-only
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>
#include "platforms/defs.h"

#define ISA		0, 31, 0
#define P2SB		0, 31, 1
#define SBREG_BAR 	0x10
#define SBREG_BARH 	0x14
#define P2SBC 		0xe0
# define P2SBC_HIDE 	(1<<8)

extern struct platform platform_skl_kbl_s_h;
extern struct platform platform_skl_kbl_lp;
extern struct platform platform_cfl_s_h;

static struct platform *platforms[] = {
	&platform_skl_kbl_s_h,
	&platform_skl_kbl_lp,
	&platform_cfl_s_h,
	NULL
};

static int devmemfd = -1;
static uint64_t sbreg_base = 0;

__attribute__((destructor)) static void close_devmemfd(void)
{
	if (devmemfd != -1)
		close(devmemfd);
}

#define PCI_READ(width) \
static uint## width ##_t pci_read ## width(uint8_t bus, uint8_t dev, uint8_t fn, uint16_t offset) \
{ \
	typeof(pci_read ## width(0, 0, 0, 0)) ret; \
	if (pread(devmemfd, &ret, sizeof ret, 0xe0000000 | bus << 20 | dev << 15 | fn << 12 | offset) != sizeof ret) { \
		perror("/dev/mem"); \
		ret = -1; \
	} \
	return ret; \
}

#define PCI_WRITE(width) \
static void pci_write ## width(uint8_t bus, uint8_t dev, uint8_t fn, uint16_t offset, uint## width ##_t val) \
{ \
	if (pwrite(devmemfd, &val, sizeof val, 0xe0000000 | bus << 20 | dev << 15 | fn << 12 | offset) != sizeof val) \
		perror("/dev/mem"); \
}

PCI_READ(16)
PCI_WRITE(16)
PCI_READ(32)
PCI_WRITE(32)

static uint32_t pcr_read32(uint8_t port, uint16_t offset)
{
	uint32_t ret;
	if (pread(devmemfd, &ret, sizeof ret, sbreg_base + (port << 16) + offset) != sizeof ret) {
		perror("/dev/mem");
		ret = -1;
	}
	return ret;
}

static void pcr_write32(uint8_t port, uint16_t offset, uint32_t val)
{
	if (pwrite(devmemfd, &val, sizeof val, sbreg_base + (port << 16) + offset) != sizeof val)
		perror("/dev/mem");
}

static int detect_pinset(struct pin *pins)
{
	int status = 0;
	for (struct pin *pin = pins; pin->gpio; ++pin) {
		int locked = !!(pcr_read32(pin->port, pin->lock_offset) &pin->lock_bit);
		status |= !locked;
		if (locked)
			printf("OK   - %s/%s\n", pin->gpio, pin->nf);
		else
			printf("FAIL - %s/%s\n", pin->gpio, pin->nf);
	}
	return status;
}

static int scan_platform(struct platform *platform)
{
	printf("Unhiding P2SB bridge...\n");
	pci_write32(P2SB, P2SBC, 0);
	if (pci_read32(P2SB, 0) == 0xffffffff) {
		fprintf(stderr, "Failed to unhide P2SB bridge, bailing!\n");
		return -1;
	}
	sbreg_base = ((uint64_t) pci_read32(P2SB, SBREG_BARH) << 32) |
	             (pci_read32(P2SB, SBREG_BAR) & 0xff000000);
	printf("Private configuration space is accesible at %016" PRIx64 "\n", sbreg_base);
	printf("Hiding P2SB bridge...\n");
	pci_write32(P2SB, P2SBC, P2SBC_HIDE);

	int status = detect_pinset(platform->global_pins);

	int is_espi = 1;
	if (platform->lpc_pins)
		is_espi = !!(pcr_read32(platform->espi_check_port, platform->espi_check_offset) & platform->espi_check_bit);

	if (is_espi) {
		printf("Platform in eSPI mode!\n");
		status |= detect_pinset(platform->espi_pins);
	} else {
		printf("Platform in LPC mode!\n");
		status |= detect_pinset(platform->lpc_pins);
	}

	return status;
}

int main(void)
{
	devmemfd = open("/dev/mem", O_RDWR);

	if (devmemfd < 0) {
		perror("/dev/mem");
		return 1;
	}


	uint16_t isa_vendor = pci_read16(ISA, 0);
	if (isa_vendor != 0x8086) {
		fprintf(stderr, "Unknown ISA bridge vendor %04x, expected 8086 for Intel\n", isa_vendor);
		return 1;
	}
	uint16_t isa_device = pci_read16(ISA, 2);
	struct platform *platform = NULL;
	for (struct platform **pp = platforms; *pp; ++pp) {
		for (struct pch *pch = (*pp)->pchs; pch->name; ++pch) {
			if (pch->pid == isa_device) {
				printf("Found platform %s with PCH %s!\n", (*pp)->name, pch->name);
				platform = *pp;
				break;
			}
		}
	}

	if (!platform) {
		fprintf(stderr, "Unknown ISA bridge device %04x\n", isa_device);
		return 1;
	}

	int err = scan_platform(platform);
	if (err < 0)
		return 127; // detection error
	return err;
}
