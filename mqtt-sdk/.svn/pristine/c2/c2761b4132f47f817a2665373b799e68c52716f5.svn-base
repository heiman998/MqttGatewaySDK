/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */
#ifndef __ADDRS_H__
#define __ADDRS_H__

/*
 * Special AR6002 Addresses that may be needed by special
 * applications (e.g. ART) on the Host as well as Target.
 */

#if defined(AR6002_REV2)
#define AR6K_RAM_START 0x00500000
#define TARG_RAM_OFFSET(vaddr) ((A_UINT32)(vaddr) & 0xfffff)
#define TARG_RAM_SZ (184*1024)
#define TARG_ROM_SZ (80*1024)
#endif
#if defined(AR6002_REV4) || defined(AR6003)
#define AR6K_RAM_START 0x00540000
#define TARG_RAM_OFFSET(vaddr) (((A_UINT32)(vaddr) & 0xfffff) - 0x40000)
#define TARG_RAM_SZ (256*1024)
#define TARG_ROM_SZ (256*1024)
#endif
#if defined(AR6002_REV6) || defined(AR6004)
#define AR6K_RAM_START 0x00400000
#define TARG_RAM_OFFSET(vaddr) (((A_UINT32)(vaddr) & 0x3fffff))
#define TARG_RAM_SZ_MAX (256*1024) /* Max possible DRAM size -- actual size may be smaller */
#define TARG_ROM_SZ (512*1024)

/* IRAM consists of 1 fixed bank plus up to 4 switchable I/D banks */
#define AR6K_SWITCHABLE_RAM_BANK_SZ (32*1024)
#define TARG_IRAM_START 0x00998000
#define TARG_SWITCHABLE_IRAM_START (TARG_IRAM_START + 32*1024)
#define TARG_IRAM_SZ (32*1024 + 4*AR6K_SWITCHABLE_RAM_BANK_SZ)

#define TARG_RAM_ACS_RESERVE  32

#endif

#if defined(AR6002_REV7)
    #if defined(AR6002_REV75) || defined(AR6002_REV76)
        #define AR6K_RAM_START 0x00420000
        #define TARG_RAM_OFFSET(vaddr) (((A_UINT32)(vaddr) & 0xfffff) - 0x20000)
        /* IRAM consists of 1 fixed bank plus up to 16 switchable I/D banks */
        #define AR6K_SWITCHABLE_RAM_BANK_SZ (32*1024)
        #define TARG_DRAM_BANK_SZ (32*1024)
        #define TARG_IRAM_START 0x00A00000

        #if defined(FPGA)
        #define TARG_RAM_SZ_MAX (768*1024) /* Max possible DRAM size -- actual size may be smaller */
        #define TARG_RAM_SZ (768*1024)
        #define TARG_RAM_SWITCHABLE_BANKS (12)
        #else /*FPGA*/
        #define TARG_RAM_SZ_MAX (896*1024) /* Max possible DRAM size -- actual size may be smaller */
        #define TARG_RAM_SZ (896*1024)
        #define TARG_RAM_SWITCHABLE_BANKS (16)
        #endif /*FPGA*/
        #define TARG_ROM_SZ (1024*1024)
        #define TARG_IRAM_FIXED_SZ (576*1024) 
        #define TARG_DRAM_FIXED_SZ (384*1024) 
       
        #define TARG_SWITCHABLE_IRAM_START (TARG_IRAM_START + TARG_IRAM_FIXED_SZ)
        #define TARG_IRAM_SZ (TARG_IRAM_FIXED_SZ + TARG_RAM_SWITCHABLE_BANKS*AR6K_SWITCHABLE_RAM_BANK_SZ)    
        #define TARG_RAM_ACS_RESERVE  0
        
    #else /*AR6002_REV75 || AR6002_REV76*/
    
        #define AR6K_RAM_START 0x00428000
        #define TARG_RAM_OFFSET(vaddr) (((A_UINT32)(vaddr) & 0x3fffff) - 0x28000)
        /* IRAM consists of 1 fixed bank plus up to 8 switchable I/D banks */
        #define AR6K_SWITCHABLE_RAM_BANK_SZ (32*1024)
        #define TARG_IRAM_START 0x00A00000

        #if defined(AR6006_FPGA_HAPS51)
            #if defined(AR6006_FPGA_HAPS51_ROM512)
                #define TARG_RAM_SZ_MAX (256*1024) /* Max possible DRAM size -- actual size may be smaller */
                #define TARG_RAM_SZ (256*1024)
                #define TARG_ROM_SZ (512*1024)
                #define TARG_IRAM_FIXED_SZ (96*1024)
                #define TARG_DRAM_FIXED_SZ (96*1024) 
                #define TARG_RAM_SWITCHABLE_BANKS (5)
            #else
                #define TARG_RAM_SZ_MAX (224*1024) /* Max possible DRAM size -- actual size may be smaller */
                #define TARG_RAM_SZ (224*1024)//(256*1024)
                #define TARG_ROM_SZ (640*1024)
                #define TARG_IRAM_FIXED_SZ (32*1024)
                #define TARG_DRAM_FIXED_SZ (160*1024) 
                #define TARG_RAM_SWITCHABLE_BANKS (2)
            #endif
        #else
            #define TARG_RAM_SZ_MAX (352*1024) /* Max possible DRAM size -- actual size may be smaller */
            #define TARG_RAM_SZ (352*1024)
            #define TARG_ROM_SZ (768*1024)
            #define TARG_IRAM_FIXED_SZ (96*1024) 
            #define TARG_DRAM_FIXED_SZ (96*1024) 
            #define TARG_RAM_SWITCHABLE_BANKS (8)
        #endif
       
        #define TARG_SWITCHABLE_IRAM_START (TARG_IRAM_START + TARG_IRAM_FIXED_SZ)
        #define TARG_IRAM_SZ (TARG_IRAM_FIXED_SZ + TARG_RAM_SWITCHABLE_BANKS*AR6K_SWITCHABLE_RAM_BANK_SZ)    
        #define TARG_RAM_ACS_RESERVE  0
    #endif /*AR6002_REV75 || AR6002_REV76*/
#endif

#define AR6002_BOARD_DATA_SZ 768
#define AR6002_BOARD_EXT_DATA_SZ 0
#define AR6003_BOARD_DATA_SZ 1024
/* Reserve space for extended board data */
/* AR6003 v2 has only 768 bytes for extended board data */
#define AR6003_VER2_BOARD_EXT_DATA_SZ 768
#if defined(AR6002_REV42)
#define AR6003_BOARD_EXT_DATA_SZ AR6003_VER2_BOARD_EXT_DATA_SZ
#else
#define AR6003_BOARD_EXT_DATA_SZ 1024 
#endif /* AR6002_REV42 */
#define AR6004_BOARD_DATA_SZ     7168
#define AR6004_BOARD_EXT_DATA_SZ 0
#define AR6006_BOARD_DATA_SZ     7168
#define AR6006_BOARD_EXT_DATA_SZ 0

#define AR6K_RAM_ADDR(byte_offset) (AR6K_RAM_START+(byte_offset))
#define TARG_RAM_ADDRS(byte_offset) AR6K_RAM_ADDR(byte_offset)

#if defined(AR6002_REV2) || defined(AR6002_REV4)
#define AR6K_ROM_START 0x004e0000
#define TARG_ROM_OFFSET(vaddr) (((A_UINT32)(vaddr) & 0x1fffff) - 0xe0000)
#endif /* AR6002_REV2 || AR6002_REV4 */
#if defined(AR6002_REV6)
#define AR6K_ROM_START 0x00900000
#define TARG_ROM_OFFSET(vaddr) ((A_UINT32)(vaddr) & 0xfffff)
#endif /* AR6002_REV6 */
#if defined(AR6002_REV7)
    #if defined(AR6002_REV75) || defined(AR6002_REV76)
        #define AR6K_ROM_START 0x00900000
        #define TARG_ROM_OFFSET(vaddr) ((A_UINT32)(vaddr) & 0xfffff)
    #else /*AR6002_REV75 || AR6002_REV76*/
        #if defined(AR6006_FPGA_HAPS51)
            #if defined(AR6006_FPGA_HAPS51_ROM512)
                #define AR6K_ROM_START 0x00980000
            #else
                #define AR6K_ROM_START 0x00960000
            #endif
        #else
            #define AR6K_ROM_START 0x00940000
        #endif /* AR6006_FPGA_HAPS51 */
        #define TARG_ROM_OFFSET(vaddr) ((A_UINT32)(vaddr) & 0xfffff)    
    #endif /*AR6002_REV75 || AR6002_REV76*/
#endif /* AR6002_REV7 */
#define AR6K_ROM_ADDR(byte_offset) (AR6K_ROM_START+(byte_offset))
#define TARG_ROM_ADDRS(byte_offset) AR6K_ROM_ADDR(byte_offset)

/*
 * At this ROM address is a pointer to the start of the ROM DataSet Index.
 * If there are no ROM DataSets, there's a 0 at this address.
 */
#define ROM_DATASET_INDEX_ADDR          (TARG_ROM_ADDRS(TARG_ROM_SZ)-8)
#define ROM_MBIST_CKSUM_ADDR            (TARG_ROM_ADDRS(TARG_ROM_SZ)-4)

/*
 * The API A_BOARD_DATA_ADDR() is the proper way to get a read pointer to
 * board data.
 */

/* Size of Board Data, in bytes */
#if defined(AR6002_REV4) || defined(AR6003)
#define BOARD_DATA_SZ (AR6003_BOARD_DATA_SZ + AR6003_BOARD_EXT_DATA_SZ)
#endif
#if defined(AR6002_REV6)
#define BOARD_DATA_SZ AR6004_BOARD_DATA_SZ
#endif
#if defined(AR6002_REV7)
#define BOARD_DATA_SZ AR6006_BOARD_DATA_SZ
#endif
#if !defined(BOARD_DATA_SZ)
#define BOARD_DATA_SZ AR6002_BOARD_DATA_SZ
#endif

#endif /* __ADDRS_H__ */
