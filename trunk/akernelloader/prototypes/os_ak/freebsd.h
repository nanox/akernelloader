#ifndef _FREEBSD_H_
#define _FREEBSD_H_

/* Only change the version number if you break compatibility. */
#define BOOTINFO_VERSION        1

#define N_BIOS_GEOM             8

/*
 * A zero bootinfo field often means that there is no info available.
 * Flags are used to indicate the validity of fields where zero is a
 * normal value.
 */
struct bootinfo
  {
    unsigned int bi_version;
    unsigned char *bi_kernelname;
    struct nfs_diskless *bi_nfs_diskless;
    /* End of fields that are always present. */
#define bi_endcommon            bi_n_bios_used
    unsigned int bi_n_bios_used;
    unsigned long bi_bios_geom[N_BIOS_GEOM];
    unsigned int bi_size;
    unsigned char bi_memsizes_valid;
    unsigned char bi_bios_dev;
    unsigned char bi_pad[2];
    unsigned long bi_basemem;
    unsigned long bi_extmem;
    unsigned long bi_symtab;
    unsigned long bi_esymtab;
  };

#endif

