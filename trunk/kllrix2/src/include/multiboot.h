/* multiboot.h - the header for Multiboot */
/* Copyright (C) 1999, 2001  Free Software Foundation, Inc.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

#ifndef MULTIBOOT_H_
#define MULTIBOOT_H_

/* The size of our stack (16KB). */
#define STACK_SIZE                      0x4000

/*
 *  Flags to be set in the 'flags' parameter above
 */

/* is there basic lower/upper memory information? */
#define MB_INFO_MEMORY			0x00000001
/* is there a boot device set? */
#define MB_INFO_BOOTDEV			0x00000002
/* is the command-line defined? */
#define MB_INFO_CMDLINE			0x00000004
/* are there modules to do something with? */
#define MB_INFO_MODS			0x00000008

/* These next two are mutually exclusive */

/* is there a symbol table loaded? */
#define MB_INFO_AOUT_SYMS		0x00000010
/* is there an ELF section header table? */
#define MB_INFO_ELF_SHDR		0x00000020

/* is there a full memory map? */
#define MB_INFO_MEM_MAP			0x00000040

/* Is there drive info?  */
#define MB_INFO_DRIVE_INFO		0x00000080

/* Is there a config table?  */
#define MB_INFO_CONFIG_TABLE		0x00000100

/* Is there a boot loader name?  */
#define MB_INFO_BOOT_LOADER_NAME	0x00000200

/* Is there a APM table?  */
#define MB_INFO_APM_TABLE		0x00000400

/* Is there video information?  */
#define MB_INFO_VIDEO_INFO		0x00000800


#ifndef ASM
/* Do not include here in boot.S. */

/* Types. */

/* The Multiboot header. */
typedef struct multiboot_header
{
	unsigned long magic;
	unsigned long flags;
	unsigned long checksum;
	unsigned long header_addr;
	unsigned long load_addr;
	unsigned long load_end_addr;
	unsigned long bss_end_addr;
	unsigned long entry_addr;
} multiboot_header_t;

/* The symbol table for a.out. */
typedef struct aout_symbol_table
{
	unsigned long tabsize;
	unsigned long strsize;
	unsigned long addr;
	unsigned long reserved;
} aout_symbol_table_t;

/* The section header table for ELF. */
typedef struct elf_section_header_table
{
	unsigned long num;
	unsigned long size;
	unsigned long addr;
	unsigned long shndx;
} elf_section_header_table_t;

/* The Multiboot information. */
typedef struct multiboot_info
{
	/* MultiBoot info version number */
	unsigned long flags;

	/* Available memory from BIOS */
	unsigned long mem_lower;
	unsigned long mem_upper;

	/* "root" partition */
	unsigned long boot_device;

	/* Kernel command line */
	unsigned long cmdline;

	/* Boot-Module list */
	unsigned long mods_count;
	unsigned long mods_addr;

	union
	{
		struct
		{
			/* (a.out) Kernel symbol table info */
			unsigned long tabsize;
			unsigned long strsize;
			unsigned long addr;
			unsigned long pad;
		}
		a;

		struct
		{
			/* (ELF) Kernel section header table */
			unsigned long num;
			unsigned long size;
			unsigned long addr;
			unsigned long shndx;
		}
		e;
	}
	syms;

	/* Memory Mapping buffer */
	unsigned long mmap_length;
	unsigned long mmap_addr;

	/* Drive Info buffer */
	unsigned long drives_length;
	unsigned long drives_addr;

	/* ROM configuration table */
	unsigned long config_table;

	/* Boot Loader Name */
	unsigned long boot_loader_name;

	/* APM table */
	unsigned long apm_table;

	/* Video */
	unsigned long vbe_control_info;
	unsigned long vbe_mode_info;
	unsigned short vbe_mode;
	unsigned short vbe_interface_seg;
	unsigned short vbe_interface_off;
	unsigned short vbe_interface_len;
} multiboot_info_t;

/* The module structure. */
typedef struct module
{
	unsigned long mod_start;
	unsigned long mod_end;
	unsigned long string;
	unsigned long reserved;
} module_t;

/* The memory map. Be careful that the offset 0 is base_addr_low
but no size. */
typedef struct memory_map
{
	unsigned long size;
	unsigned long base_addr_low;
	unsigned long base_addr_high;
	unsigned long length_low;
	unsigned long length_high;
	unsigned long type;
} memory_map_t;

/* VBE controller information.  */
struct vbe_controller
{
  unsigned char signature[4];
  unsigned short version;
  unsigned long oem_string;
  unsigned long capabilities;
  unsigned long video_mode;
  unsigned short total_memory;
  unsigned short oem_software_rev;
  unsigned long oem_vendor_name;
  unsigned long oem_product_name;
  unsigned long oem_product_rev;
  unsigned char reserved[222];
  unsigned char oem_data[256];
} __attribute__ ((packed));

/* VBE mode information.  */
struct vbe_mode
{
  unsigned short mode_attributes;
  unsigned char win_a_attributes;
  unsigned char win_b_attributes;
  unsigned short win_granularity;
  unsigned short win_size;
  unsigned short win_a_segment;
  unsigned short win_b_segment;
  unsigned long win_func;
  unsigned short bytes_per_scanline;

  /* >=1.2 */
  unsigned short x_resolution;
  unsigned short y_resolution;
  unsigned char x_char_size;
  unsigned char y_char_size;
  unsigned char number_of_planes;
  unsigned char bits_per_pixel;
  unsigned char number_of_banks;
  unsigned char memory_model;
  unsigned char bank_size;
  unsigned char number_of_image_pages;
  unsigned char reserved0;

  /* direct color */
  unsigned char red_mask_size;
  unsigned char red_field_position;
  unsigned char green_mask_size;
  unsigned char green_field_position;
  unsigned char blue_mask_size;
  unsigned char blue_field_position;
  unsigned char reserved_mask_size;
  unsigned char reserved_field_position;
  unsigned char direct_color_mode_info;

  /* >=2.0 */
  unsigned long phys_base;
  unsigned long reserved1;
  unsigned short reversed2;

  /* >=3.0 */
  unsigned short linear_bytes_per_scanline;
  unsigned char banked_number_of_image_pages;
  unsigned char linear_number_of_image_pages;
  unsigned char linear_red_mask_size;
  unsigned char linear_red_field_position;
  unsigned char linear_green_mask_size;
  unsigned char linear_green_field_position;
  unsigned char linear_blue_mask_size;
  unsigned char linear_blue_field_position;
  unsigned char linear_reserved_mask_size;
  unsigned char linear_reserved_field_position;
  unsigned long max_pixel_clock;

  unsigned char reserved3[189];
} __attribute__ ((packed));

#endif /* ! ASM */

#endif /* MULTIBOOT_H_ */
