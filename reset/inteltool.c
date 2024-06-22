/* SPDX-License-Identifier: GPL-2.0-only */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include "inteltool.h"
#include "pcr.h"

#ifdef __NetBSD__
#include <machine/sysarch.h>
#endif

#define MAX_PCR_PORTS 8 /* how often may `--pcr` be specified */

#ifndef __DARWIN__
static int fd_mem;

void *map_physical(uint64_t phys_addr, size_t len)
{
	void *virt_addr;

	virt_addr = mmap(0, len, PROT_WRITE | PROT_READ, MAP_SHARED,
		    fd_mem, (off_t) phys_addr);

	if (virt_addr == MAP_FAILED) {
		printf("Error mapping physical memory 0x%08" PRIx64 "[0x%zx]\n",
			phys_addr, len);
		return NULL;
	}

	return virt_addr;
}

void unmap_physical(void *virt_addr, size_t len)
{
	munmap(virt_addr, len);
}
#endif

int main(int argc, char *argv[])
{
	struct pci_access *pacc = NULL;
	struct pci_dev *sb = NULL, *dev;

#if defined(__FreeBSD__)
	if (open("/dev/io", O_RDWR) < 0) {
		perror("/dev/io");
#elif defined(__NetBSD__)
# ifdef __i386__
	if (i386_iopl(3)) {
		perror("iopl");
# else
	if (x86_64_iopl(3)) {
		perror("iopl");
# endif
#else
	if (iopl(3)) {
		perror("iopl");
#endif
		printf("You need to be root.\n");
		exit(1);
	}

#ifndef __DARWIN__
	if ((fd_mem = open("/dev/mem", O_RDWR)) < 0) {
		perror("Can not open /dev/mem");
		exit(1);
	}
#endif

	pacc = pci_alloc();
	pacc->method = PCI_ACCESS_I386_TYPE1;
	pci_init(pacc);
	pci_scan_bus(pacc);

	/* Find the required devices */
	for (dev = pacc->devices; dev; dev = dev->next) {
		pci_fill_info(dev, PCI_FILL_CLASS);
		/* The ISA/LPC bridge can be 0x1f, 0x07, or 0x04 so we probe. */
		if (dev->device_class == 0x0601) { /* ISA/LPC bridge */
			if (sb == NULL) {
				sb = dev;
			} else {
				fprintf(stderr, "Multiple devices with class ID"
					" 0x0601, using %02x%02x:%02x.%02x\n",
					sb->domain, sb->bus, sb->dev, sb->func);
				break;
			}
		}
	}

	if (!sb) {
		printf("No southbridge found.\n");
		exit(1);
	}

	pci_fill_info(sb, PCI_FILL_IDENT | PCI_FILL_BASES | PCI_FILL_CLASS);

	if (sb->vendor_id != PCI_VENDOR_ID_INTEL) {
		printf("Not an Intel(R) southbridge.\n");
		exit(1);
	}

	pcr_init(sb);

	_Bool known_pch = 0;
	uint8_t port;
	uint16_t offset;

	switch (sb->device_id) {
	case PCI_DEVICE_ID_INTEL_H110:
	case PCI_DEVICE_ID_INTEL_H170:
	case PCI_DEVICE_ID_INTEL_Z170:
	case PCI_DEVICE_ID_INTEL_Q170:
	case PCI_DEVICE_ID_INTEL_Q150:
	case PCI_DEVICE_ID_INTEL_B150:
	case PCI_DEVICE_ID_INTEL_C236:
	case PCI_DEVICE_ID_INTEL_C232:
	case PCI_DEVICE_ID_INTEL_QM170:
	case PCI_DEVICE_ID_INTEL_HM170:
	case PCI_DEVICE_ID_INTEL_CM236:
	case PCI_DEVICE_ID_INTEL_HM175:
	case PCI_DEVICE_ID_INTEL_QM175:
	case PCI_DEVICE_ID_INTEL_CM238:
		printf("Found Sunrise Point S/H\n");
		known_pch = 1;
		port = 0xaf;
		offset = 0x528;
		break;

	case PCI_DEVICE_ID_INTEL_H270:
	case PCI_DEVICE_ID_INTEL_Z270:
	case PCI_DEVICE_ID_INTEL_Q270:
	case PCI_DEVICE_ID_INTEL_Q250:
	case PCI_DEVICE_ID_INTEL_B250:
	case PCI_DEVICE_ID_INTEL_Z370:
	case PCI_DEVICE_ID_INTEL_H310C:
	case PCI_DEVICE_ID_INTEL_X299:
		printf("Found Kaby Point S/H\n");
		known_pch = 1;
		port = 0xaf;
		offset = 0x528;
		break;

	case PCI_DEVICE_ID_INTEL_H610:
	case PCI_DEVICE_ID_INTEL_B660:
	case PCI_DEVICE_ID_INTEL_H670:
	case PCI_DEVICE_ID_INTEL_Q670:
	case PCI_DEVICE_ID_INTEL_Z690:
	case PCI_DEVICE_ID_INTEL_W680:
	case PCI_DEVICE_ID_INTEL_WM690:
	case PCI_DEVICE_ID_INTEL_HM670:
	case PCI_DEVICE_ID_INTEL_R680E:
	case PCI_DEVICE_ID_INTEL_Q670E:
	case PCI_DEVICE_ID_INTEL_H610E:
		printf("Found Alder Point S/H\n");
		known_pch = 1;
		port = 0x6d;
		offset = 0x7d0;
		break;

	case PCI_DEVICE_ID_INTEL_W790:
	case PCI_DEVICE_ID_INTEL_Z790:
	case PCI_DEVICE_ID_INTEL_H770:
	case PCI_DEVICE_ID_INTEL_B760:
	case PCI_DEVICE_ID_INTEL_HM770:
	case PCI_DEVICE_ID_INTEL_WM790:
	case PCI_DEVICE_ID_INTEL_C262:
	case PCI_DEVICE_ID_INTEL_C266:
		printf("Found Raptor Point S/H\n");
		known_pch = 1;
		port = 0x6d;
		offset = 0x7d0;
		break;

	default:
		printf("Unknown PCH device ID %04x\n", sb->device_id);
		break;
	}

	if (known_pch) {
		uint32_t dw0 = read_pcr32(port, offset + 0);
		uint32_t dw1 = read_pcr32(port, offset + 4);

		write_pcr32(port, offset + 0, 0x80000000);	// dw0
		write_pcr32(port, offset + 4, 0x00000000);	// dw1

		sleep(1);

		write_pcr32(port, offset + 0, dw0);
		write_pcr32(port, offset + 4, dw1);
	}

	/* Clean up */
	pcr_cleanup();
	/* `sb` wasn't allocated by pci_get_dev() */
	pci_cleanup(pacc);

	return 0;
}

