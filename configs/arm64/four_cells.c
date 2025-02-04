/*
 * Jailhouse, a Linux-based partitioning hypervisor
 *
 * Test configuration for Raspberry Pi 4 (quad-core Cortex-A72, 1GB, 2GB, 4GB or 8GB RAM)
 *
 * Copyright (c) Siemens AG, 2020
 *
 * Authors:
 *  Jan Kiszka <jan.kiszka@siemens.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 *
 * Reservation via device tree: reg = <0x0 0x20000000 0x10000000>;
 */

#include <jailhouse/types.h>
#include <jailhouse/cell-config.h>

struct {
	struct jailhouse_system header;
	__u64 cpus[1];
	struct jailhouse_memory mem_regions[19];
	struct jailhouse_irqchip irqchips[2];
	struct jailhouse_pci_device pci_devices[3];
} __attribute__((packed)) config = {
	.header = {
		.signature = JAILHOUSE_SYSTEM_SIGNATURE,
		.revision = JAILHOUSE_CONFIG_REVISION,
		.flags = JAILHOUSE_SYS_VIRTUAL_DEBUG_CONSOLE,
		.hypervisor_memory = {
			.phys_start = 0x20000000,
			.size       = 0x00400000,
		},
		.debug_console = {
			.address = 0xfe215040,
			.size = 0x40,
			.type = JAILHOUSE_CON_TYPE_8250,
			.flags = JAILHOUSE_CON_ACCESS_MMIO |
				 JAILHOUSE_CON_REGDIST_4,
		},
		.platform_info = {
			.pci_mmconfig_base = 0xff900000,
			.pci_mmconfig_end_bus = 0,
			.pci_is_virtual = 1,
			.pci_domain = 1,
			.arm = {
				.gic_version = 2,
				.gicd_base = 0xff841000,
				.gicc_base = 0xff842000,
				.gich_base = 0xff844000,
				.gicv_base = 0xff846000,
				.maintenance_irq = 25,
			},
		},
		.root_cell = {
			.name = "Raspberry-Pi4",

			.cpu_set_size = sizeof(config.cpus),
			.num_memory_regions = ARRAY_SIZE(config.mem_regions),
			.num_irqchips = ARRAY_SIZE(config.irqchips),
			.num_pci_devices = ARRAY_SIZE(config.pci_devices),

			.vpci_irq_base = 182 - 32,
		},
	},

	.cpus = {
		0b1111,
	},

	.mem_regions = {
		/* IVSHMEM shared memory regions for 00:00.0 (demo) */
		/*Cells-States memory region*/
		{
			.phys_start = 0x24000000,
			.virt_start = 0x24000000,
			.size = 0x2000,
			.flags = JAILHOUSE_MEM_READ,
		},
		/*Read/Write memory region*/
		{
			.phys_start = 0x24002000,
			.virt_start = 0x24002000,
			.size = 0x18000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE,
		},
		/*Root-cell Read/Write*/
		{
			.phys_start = 0x2401a000,
			.virt_start = 0x2401a000,
			.size = 0x4000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE,
		},
		/*Bare-metal cell Read/Write*/
		{
			.phys_start = 0x2401e000,
			.virt_start = 0x2401e000,
			.size = 0x4000,
			.flags = JAILHOUSE_MEM_READ,
		},
		/*Linux 1 cell Read/Write*/
		{
			.phys_start = 0x24022000,
			.virt_start = 0x24022000,
			.size = 0x4000,
			.flags = JAILHOUSE_MEM_READ,
		},
                /*Linux 2 cell Read/Write*/
		{
			.phys_start = 0x24026000,
			.virt_start = 0x24026000,
			.size = 0x4000,
			.flags = JAILHOUSE_MEM_READ,
		},
		/* IVSHMEM shared memory regions for 00:01.0 (networking) Index=6,4xmem*/
		JAILHOUSE_SHMEM_NET_REGIONS(0x35000000, 0),
                 /* IVSHMEM shared memory regions for 00:01.0 (networking) Index =10,4xmem*/
                JAILHOUSE_SHMEM_NET_REGIONS(0x35100000, 0),
		/* MMIO 1 (permissive) */ {
			.phys_start = 0xfd500000,
			.virt_start = 0xfd500000,
			.size =        0x1b00000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* MMIO 2 (permissive) */{
			.phys_start = 0x600000000,
			.virt_start = 0x600000000,
			.size =         0x8000000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* RAM (0M-~506M) */
 		{
			.phys_start = 0x0,
			.virt_start = 0x0,
			.size = 0x1fa10000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_EXECUTE,
		},

		/* ~2M reserved for shared memory regions */

		/* 4M reserved for the hypervisor */

		/* RAM (512M-4032M) */
		{
			.phys_start = 0x20000000,
			.virt_start = 0x20000000,
			.size = 0xdc000000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_EXECUTE,
		},

		/* RAM (4096M-8192M) */
		{
			.phys_start = 0x100000000,
			.virt_start = 0x100000000,
			.size = 0x100000000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_EXECUTE,
		},
	},

	.irqchips = {
		/* GIC */
		{
			.address = 0xff841000,
			.pin_base = 32,
			.pin_bitmap = {
				0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
			},
		},
		/* GIC */
		{
			.address = 0xff841000,
			.pin_base = 160,
			.pin_bitmap = {
				0xffffffff, 0xffffffff
			},
		},
	},

	.pci_devices = {
		{
			 /* IVSHMEM 0001:00:00.0 (demo) */
			.type = JAILHOUSE_PCI_TYPE_IVSHMEM,
			.domain = 1,
			.bdf = 0 << 3,
			.bar_mask = JAILHOUSE_IVSHMEM_BAR_MASK_INTX,
			.shmem_regions_start = 0,
			.shmem_dev_id = 0,
			.shmem_peers = 4,
			.shmem_protocol = JAILHOUSE_SHMEM_PROTO_UNDEFINED,
		},
		{
			/* IVSHMEM 0001:00:01.0 (networking) */
			.type = JAILHOUSE_PCI_TYPE_IVSHMEM,
			.domain = 1,
			.bdf = 1 << 3,
			.bar_mask = JAILHOUSE_IVSHMEM_BAR_MASK_INTX,
			.shmem_regions_start = 6,
			.shmem_dev_id = 0,
			.shmem_peers = 2,
			.shmem_protocol = JAILHOUSE_SHMEM_PROTO_VETH,
		},
			/* IVSHMEM 0001:00:02.0 (networking) */
                {
                        .type = JAILHOUSE_PCI_TYPE_IVSHMEM,
                        .domain = 2,
                        .bdf = 2 << 3,
                        .bar_mask = JAILHOUSE_IVSHMEM_BAR_MASK_INTX,
                        .shmem_regions_start = 10,
                        .shmem_dev_id = 0,
                        .shmem_peers = 2,
                        .shmem_protocol = JAILHOUSE_SHMEM_PROTO_VETH,
                },
	},
};
