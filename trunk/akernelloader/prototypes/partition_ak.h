/*  Akernelloader TEAM
    akaloaderadmin@gmail.com

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. 
 */

#ifndef _PARTITION_AK_H_
#define _PARTITION_AK_H_

#include <types_ak.h>


#define PARTITION_PRIM_MAX  4   /* Primary partitions max. entry  */
#define PARTITION_EXTD_MAX  32  /* Max. extended partitions */

#define PARTITION_FAT32_LBA    0xc
#define PARTITION_FAT32        0xb
#define PARTITION_EXT2        0x83

#define PARTITION_BOOTABLE      0x80  /* Bootable (active) */

#define MBR_SIGNATURE                           0xAA55
#define EBR_SIGNATURE                           MBR_SIGNATURE

#define PARTITION_TYPE_EXTENDED             0x05 /* Extended partition */
#define PARTITION_TYPE_WIN95_EXTENDED       0x0F /* Windows 95 extended partition */

typedef struct _PARTITION_RECORD {
        __u8 status;              /* Partition status; see above */
        __u8 chs_start[3];        /* Cylinder-head-sector address to first block of partition */
        __u8 type;                /* Partition type; see above */
        __u8 chs_end[3];          /* Cylinder-head-sector address to last block of partition */
        __u32 lba_start;          /* Local block address to first sector of partition */
        __u32 block_count;        /* Number of blocks in partition */
} __attribute__((__packed__)) PARTITION_RECORD;

typedef struct _MASTER_BOOT_RECORD {
        __u8 code_area[446];                /* Code area; normally empty */
        PARTITION_RECORD partitions[4];     /* 4 primary partitions */
        __u16 signature;                    /* MBR signature; 0xAA55 */
} __attribute__((__packed__)) MASTER_BOOT_RECORD;

typedef struct _EXTENDED_BOOT_RECORD {
        __u8 code_area[446];            /* Code area; normally empty */
        PARTITION_RECORD partition;     /* Primary partition */
        PARTITION_RECORD next_ebr;      /* Next extended boot record in the chain */
        __u8 reserved[32];              /* Normally empty */
        __u16 signature;                /* EBR signature; 0xAA55 */
} __attribute__((__packed__)) EXTENDED_BOOT_RECORD;





 struct part_entry
      {
    unsigned int  start;
        unsigned int  length;
        int type;
      } partition;

#ifdef __DEF_PART__ 1
unsigned long partition_find = 0XFFFF;
#else
extern unsigned long partition_find;
#endif

int open_partition (void);

#endif /* End partition_ak.h */





