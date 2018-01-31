/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */
#include <osapi.h>
#include <nvram_api.h>
#include "hw/spiMasterMio_reg_map.h"
#include <iot_flashotp.h>
#include <AR6K_version.h> 

#include <hw/gpio_reg.h>
#include "si_api.h"
//#if defined(CONFIG_MULTIIO_SPI_IFACE)
#include "hw/spiMasterMio_reg_map.h"
#include "si_multiIO_api.h"
#include "spi_nvram_internal.h"
//

/*
 * This application will first try to read OTP and will
 * conditionally write values to OTP.  Next the application
 * will program flash using the flashimage.inc bytes found 
 * in bytes[].  The application will return a value which 
 * indicates the status of both operations.
 */
#define MASTER_SPI_CONFIG_DEFAULT (SMM_SPIMSTRCONFIG_PINHOLDN_MASK|SMM_SPIMSTRCONFIG_PINWPN_MASK)
#define	GPIO_PIN_FUNCTION      2
/* nv_config_mio_single & nv_config_legacy & iototpcfg_main are defined in iot_otpcfg.c */
extern const struct nvram_config_s nv_config_mio_single;
extern const struct nvram_config_s nv_config_legacy;
extern const struct nvram_config_s nv_config_legacy_68pin;

extern const struct nvram_config_s nv_config_mio_quad_68pin;
extern A_INT32 iototpcfg_main(A_UINT32 param);
extern struct nvramr_api_s *nvramr_api;
extern void patch_spi_nvramr_module_install();

A_UINT8 flash_id[SPI_FLASH_PARA] = {0, 0, 0};
void patch_multiIO_spi_write_QE(int quad_mode)
{
    A_UINT8 status[2];
    union{
        A_UINT32 req;
        A_UINT8 req_byte[4];
    }u;
    A_BOOL   quad_enable_on;
    A_UINT8  wr_status;
	
	if (flash_id[0] == 0)
	{
		// Read Identification: Manufacturer ID (1 byte) + Device ID (Memory Type + Memory Capacity, 2 bytes)
	    u.req_byte[0] = SPI_CMD_IDENTIFICATION;
	    si_multiIO_request(&u.req, 3, 1);
	    si_multiIO_response(&flash_id[0], 3);
	}
	
	// Micron SPI Flash
	if (flash_id[0] == SPI_FlASH_MANUFACTURER_ID_MICRON)
	{
		A_UINT8 clock_cycle;

		// Micron Flash N25Q032A supported
		
		if (quad_mode)
		{
			// Quad Mode
			clock_cycle = DUMMY_CLOCK_CYCLE_VALUE;
		}
		else
		{
			clock_cycle = DUMMY_CLOCK_CYCLE_VALUE_NON_QUAD;
		}
		
	    /* 1) read 2 bytes of nonvolatile configuration register */
	    u.req_byte[0] = SPI_CMD_NONVOLATILE_CONF_REG_READ;
		
	    si_multiIO_request(&u.req, 2, 1);
	    si_multiIO_response(&status[0], 2);

		if ((status[1] & DUMMY_CLOCK_CYCLE_MASK) != clock_cycle)
		{
			status[1] &= ~DUMMY_CLOCK_CYCLE_MASK;
			status[1] |= clock_cycle;
			
	        u.req_byte[0] = SPI_CMD_WREN;
			
	        si_multiIO_request(&u.req, 0, 1);
	        multiIO_spi_wait_part_done();
			
		    u.req_byte[0] = SPI_CMD_NONVOLATILE_CONF_REG_WRITE;
			u.req_byte[1] = status[0];
			u.req_byte[2] = status[1];
			
		    si_multiIO_request(&u.req, 0, 3);
	        multiIO_spi_wait_part_done();
		}
		
	    /* 1) read 1 bytes of volatile configuration register */
	    u.req_byte[0] = SPI_CMD_VOLATILE_CONF_REG_READ;
		
	    si_multiIO_request(&u.req, 1, 1);
	    si_multiIO_response(&status[0], 1);
		
		if ((status[0] & DUMMY_CLOCK_CYCLE_MASK) != clock_cycle)
		{
			status[0] &= ~DUMMY_CLOCK_CYCLE_MASK;
			status[0] |= clock_cycle;
	    	
	        u.req_byte[0] = SPI_CMD_WREN;
			
	        si_multiIO_request(&u.req, 0, 1);
	        multiIO_spi_wait_part_done();
			
		    u.req_byte[0] = SPI_CMD_VOLATILE_CONF_REG_WRITE;
			u.req_byte[1] = status[0];
			
		    si_multiIO_request(&u.req, 0, 2);
	        multiIO_spi_wait_part_done();
		}
	}
	else
	{
		/* 1) read byte 1 of status register */
	    u.req_byte[0] = SPI_CMD_RDSR;
	    si_multiIO_request(&u.req, 1, 1);
	    si_multiIO_response(&status[0], 1);
	    /* 2) read byte 2 of status register */
	    u.req_byte[0] = SPI_CMD_RDSR_EXT;
	    si_multiIO_request(&u.req, 1, 1);
	    si_multiIO_response(&status[1], 1);

	    quad_enable_on = status[1] & SR_QE;

	    if ((quad_enable_on && !quad_mode) || (!quad_enable_on && quad_mode))
	    {
	    /* 3) Set Write enable for status */
	        u.req_byte[0] = SPI_CMD_WREN;
	        si_multiIO_request(&u.req, 0, 1);
	        multiIO_spi_wait_part_done();
	    /* 4) Write QE bit in status register */
	        if (quad_mode)
	          status[1] |= SR_QE;
	        else
	          status[1] &= ~SR_QE;

	        u.req_byte[0] = SPI_CMD_WRT_STATUS_4MB;
	        u.req_byte[1] = status[1];
	        si_multiIO_request(&u.req, 0, 2);
	        multiIO_spi_wait_part_done();

		    /* 2) read byte 2 of status register */
	    	u.req_byte[0] = SPI_CMD_RDSR_EXT;
	    	si_multiIO_request(&u.req, 1, 1);
	    	si_multiIO_response(&wr_status, 1);

			if (status[1] != wr_status) {

		    /* 3) Set Write enable for status */
		        u.req_byte[0] = SPI_CMD_WREN;
		        si_multiIO_request(&u.req, 0, 1);
		        multiIO_spi_wait_part_done();
		    /* 4) Write QE bit in status register */
		        if (quad_mode)
		          status[1] |= SR_QE;
		        else
		          status[1] &= ~SR_QE;

		        u.req_byte[0] = SPI_CMD_WRT_STATUS;
		        u.req_byte[1] = status[0];
		        u.req_byte[2] = status[1];
		        si_multiIO_request(&u.req, 0, 3);
		        multiIO_spi_wait_part_done();
			}
	    }
	}
}


void patch_spi_nvramr_module_install()
{
   SpiNvramInternalIndirectionTable._multiIO_spi_write_QE = patch_multiIO_spi_write_QE;

}
A_INT32
validate_flash(A_UINT32 start, A_UINT8 *pSrc, A_UINT32 length)
{
#define BUF_SZ (256)
    A_UCHAR buf[BUF_SZ];    
    A_UINT32 offset,size;
    A_PRINTF_ALWAYS("Validate flash\n");
    offset = start;

    while(offset-start < length){
	size = length - (offset-start);

	if(size > BUF_SZ) size = BUF_SZ;

        nvram_read(buf, offset, size);
	if(A_MEMCMP(buf, &pSrc[offset-start], size)){
		A_PRINTF_ALWAYS("bad read @ %d\n", offset);
		return 0;//failure
	}
	offset += size;
    }

    return 1;//success 
}
#if 0
unsigned int flash_image_bytes[] =
{
#include "flashimage.inc"
};

unsigned int part_table_bytes[] = 
{
#include "parttable.inc"
};
#endif

/*
 * This application supports two run-time modes:
 *        normal mode
 *        debugger mode
 * In normal mode, a segmented flash image with meta-data
 * is appended to the base sdk_flash.out image in order to
 * form a binary that can be loaded into RAM (e.g. from a
 * Host).  That binary programs flash accordingly.
 *
 * In debugger mode, the user is expected to use xt-gdb to
 * drive flash programming. The BASE version of sdk_flash.out
 * is executed from xt-gdb using a special sdk_flash xt-gdb
 * macro. The macro loads successive chunks of the RAW flash
 * image and allows the sdk_flash app to program each chunk
 * to flash.  In this mode, there is no need to rebuild
 * anything in order to program different bytes, no need to
 * use the Host or ART.  All that's needed is Target-side
 * JTAG and the raw bytes that you would like to program
 * into flash.
 */
A_UINT32 normal_mode = 1;

/**************************************
 * For debugger-driven flash, below is the sdk_flash macro.
 * To use it, split a 256KB flash image in raw.bin:
 *     split -b 65536 raw.bin
 * This creates 64KB chunks: xaa, xab, xac, xad.
 * Run "xt-gdb sdk_flash.out".
 * In xt-gdb, connect to your JTAG controller
 *    "target remote ...."
 * Then use "sdk_flash" to make it go.
 * When you hit a breakpoint, flash has been programmed.

define sdk_flash
  reset
  set pagination off
  set confirm no
  delete
  add-symbol-file sw.rom.out 0x940000
  reset
  hbreak athos_init_part1
  cont
  delete
  hbreak *0xa02800
  set *((int *)0x42881c)=0xa02800
  set *((int *)0x180c0) |= 0xcf
  cont
  delete
  load
  set $a2=1
  set normal_mode=0
  watch *&db_signal
  set db_signal=0xff
  cont
  cont
  set $buff=db_buffer
  set db_offset=0
  set db_length=65536
  restore xaa binary $buff
  set db_signal=0
  cont
  restore xab binary $buff
  set db_offset+=db_length
  set db_signal=0
  cont
  restore xac binary $buff
  set db_offset+=db_length
  set db_signal=0
  cont
  restore xad binary $buff
  set db_offset+=db_length
  set db_signal=0
  cont
  set db_signal=2
  cont
  delete
  set confirm yes
end

**************************************/


/* Variables for use in debugger mode */
A_UINT8 *db_buffer;
A_UINT32 db_offset;
A_UINT32 db_length;
volatile A_UINT32 db_signal;

/*****************************************************************************
 * write_flash: write bytes[] to serial flash part
 *  param: not used.
 *  returns: IOTFLASHOTP_RESULT_FLASH_... return value indicating success or 
 *      failure of operation.
 *****************************************************************************/
A_INT32
write_flash(A_UINT32 param)
{
    A_INT32 result = IOTFLASHOTP_RESULT_FLASH_SUCCESS;
// nvram_state is global variable
    struct nvram_state_s *nv_state;

    A_PRINTF_ALWAYS("SERFLASH start\n");
    
    /* Setup up nvram configuration for default SPI Flash */
    nv_state = (struct nvram_state_s *)(HOST_INTEREST->hi_nvram_state);
	//A_MEMCPY(&(nvram_state->_nvram_config), &nv_config_mio_single, sizeof(struct nvram_config_s));

#ifdef	FPGA
    A_MEMCPY(&(nv_state->_nvram_config), &nv_config_mio_quad_68pin, sizeof(struct nvram_config_s));	
#else
    #if defined(AR6002_REV75) || defined(AR6002_REV76)
    A_MEMCPY(&(nv_state->_nvram_config), &nv_config_mio_quad_68pin, sizeof(struct nvram_config_s));	
    #else
    if(A_GPIO_REG_READ(IOT_BOOTSTRAP_ADDRESS) & IOT_BOOTSTRAP_QFN56_PKG_STATUS_MASK){	
        A_MEMCPY(&(nv_state->_nvram_config), &nv_config_legacy, sizeof(struct nvram_config_s));	
    }else {
        A_MEMCPY(&(nv_state->_nvram_config), &nv_config_legacy_68pin, sizeof(struct nvram_config_s));	
    }
    #endif /*AR6002_REV75*/
#endif
    nv_state->_config_found = 1;
    
    /* Setup for SPI FLASH */
    spi_nvramr_module_install(&nvramr_api);
    spi_nvramw_module_install(&nvramw_api);
    flash_nvramw_module_install(nvramw_api);
 
#if defined(AR6002_REV75) || defined(AR6002_REV76)
    patch_spi_nvramr_module_install();
#endif

        
    //A_PRINTF_ALWAYS("Initialize\n");
    nvram_init();

    A_PRINTF_ALWAYS("Power up flash\n");
    nvram_power_up();
    nvram_wait_part_done();
#ifndef  FPGA
    #if (!defined(AR6002_REV75) && !defined(AR6002_REV76))
    if(A_GPIO_REG_READ(IOT_BOOTSTRAP_ADDRESS) & IOT_BOOTSTRAP_QFN56_PKG_STATUS_MASK){

//  configure GPIO22/23 for SPI141 board's flash PIN control
        gpio_configure_pin(22,GPIO_PIN_FUNCTION,GPIO_PIN_DIR_OUTPUT,GPIO_PIN_PULLUP); 
        gpio_configure_pin(23,GPIO_PIN_FUNCTION,GPIO_PIN_DIR_OUTPUT,GPIO_PIN_PULLUP); 

// output high

        A_GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, (0x1 << 22));
        A_GPIO_REG_READ(GPIO_OUT_ADDRESS);				// output high

        A_GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, (0x1 << 23));
        A_GPIO_REG_READ(GPIO_OUT_ADDRESS);				// output high
    } else 
    #endif /*!defined(AR6002_REV75)*/
    {
//  configure GPIO22/23 for SPI141 board's flash PIN control
        gpio_configure_pin(19,GPIO_PIN_FUNCTION,GPIO_PIN_DIR_OUTPUT,GPIO_PIN_PULLUP); 
        gpio_configure_pin(20,GPIO_PIN_FUNCTION,GPIO_PIN_DIR_OUTPUT,GPIO_PIN_PULLUP); 

// output high

        A_GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, (0x1 << 19));
        A_GPIO_REG_READ(GPIO_OUT_ADDRESS);				// output high

        A_GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, (0x1 << 20));
        A_GPIO_REG_READ(GPIO_OUT_ADDRESS);				// output high
    }
#endif

    A_PRINTF_ALWAYS("Erase entire flash\n");
    nvram_erase();
    if (normal_mode) {
        /*
         * Look for flash segments and commit each of them to flash.
         * A flash segment starts with a flash_segment_hdr and contains
         * the bytes to be programmed into flash.  Flash segments are
         * located in RAM at the "_end" of the binary.  A gross assumption
         * made here is that we won't dynamically allocate RAM in this
         * app, since that might wipe out the source of flash content.
         */
        #define FLASH_MAGIC 0x48534c46 /* "FLSH" */
        struct flash_segment_hdr {
            A_UINT32 flash_magic;
            A_UINT32 flash_addr;
            A_UINT32 flash_nbytes;
        };
        struct flash_segment_hdr *hdr;

        extern char _end;
        A_UINT8 *mem_ptr = (A_UINT8 *)&_end;      

        for (;;) {
            hdr = (struct flash_segment_hdr *)mem_ptr;
            if (hdr->flash_magic != FLASH_MAGIC) {
                break;
            }

            if (hdr->flash_nbytes == 0) {
                break;
            }

            A_PRINTF_ALWAYS("Program flash: %d bytes to 0x%x\n", hdr->flash_nbytes, hdr->flash_addr);
            mem_ptr += sizeof(*hdr);
            nvram_write(mem_ptr, hdr->flash_addr, hdr->flash_nbytes);
            if (validate_flash(hdr->flash_addr, mem_ptr, hdr->flash_nbytes) == 0) {
	            result = IOTFLASHOTP_RESULT_FLASH_VALIDATE_FAILED;
                    A_PRINTF_ALWAYS("Flash validation FAILED (0x%x, %d)\n", hdr->flash_addr, hdr->flash_nbytes);
                    break;
            }

            mem_ptr += A_ROUND_UP_PWR2(hdr->flash_nbytes, 4);
        }
    } else { /* Driven via gdb */

        A_ALLOCRAM_INIT(0,0);
        db_buffer = A_ALLOCRAM(64*1024);

        for (;;) {
            db_signal = 1;
            while (db_signal == 1) {
                ;
            }

            if (db_signal == 2) {
                goto done; /* user is done */
            }

            /* gdb has placed new data in buffer, set offset, set length, and cleared signal */

            A_PRINTF_ALWAYS("program flash offset:0x%08X, length %d\n", db_offset, db_length);
            nvram_write(db_buffer, db_offset, db_length);
            A_PRINTF_ALWAYS("Validate flash image...");
            if (validate_flash(db_offset, db_buffer, db_length) == 0) {
	        result = IOTFLASHOTP_RESULT_FLASH_VALIDATE_FAILED;
                A_PRINTF_ALWAYS("Flash validation FAILED (0x%x, %d)\n", db_offset, db_length);
                goto done;
            }
        }
    }

done:
    nvram_fini();

    A_PRINTF_ALWAYS("FLASH IS UPDATED\n");
    db_signal = 1;

    return result;
}




A_INT32
read_flash(A_UINT32 param)
{
    A_INT32 result = IOTFLASHOTP_RESULT_FLASH_SUCCESS;
// nvram_state is global variable
    struct nvram_state_s *nv_state;

    A_PRINTF_ALWAYS("SERFLASH start\n");
    
    /* Setup up nvram configuration for default SPI Flash */
    nv_state = (struct nvram_state_s *)(HOST_INTEREST->hi_nvram_state);
	//A_MEMCPY(&(nvram_state->_nvram_config), &nv_config_mio_single, sizeof(struct nvram_config_s));

#ifdef	FPGA
    A_MEMCPY(&(nv_state->_nvram_config), &nv_config_mio_quad_68pin, sizeof(struct nvram_config_s));	
#else
    #if defined(AR6002_REV75) || defined(AR6002_REV76)
    A_MEMCPY(&(nv_state->_nvram_config), &nv_config_mio_quad_68pin, sizeof(struct nvram_config_s));	
    #else
    if(A_GPIO_REG_READ(IOT_BOOTSTRAP_ADDRESS) & IOT_BOOTSTRAP_QFN56_PKG_STATUS_MASK){	
        A_MEMCPY(&(nv_state->_nvram_config), &nv_config_legacy, sizeof(struct nvram_config_s));	
    }else {
        A_MEMCPY(&(nv_state->_nvram_config), &nv_config_legacy_68pin, sizeof(struct nvram_config_s));	
    }
    #endif /*AR6002_REV75*/
#endif
    nv_state->_config_found = 1;
    
    /* Setup for SPI FLASH */
    spi_nvramr_module_install(&nvramr_api);
    spi_nvramw_module_install(&nvramw_api);
    flash_nvramw_module_install(nvramw_api);

#if defined(AR6002_REV75) || defined(AR6002_REV76)
    patch_spi_nvramr_module_install();
#endif
        
    //A_PRINTF_ALWAYS("Initialize\n");
    nvram_init();

    A_PRINTF_ALWAYS("Power up flash\n");
    nvram_power_up();
    nvram_wait_part_done();
#ifndef  FPGA
    #if (!defined(AR6002_REV75)) && (!defined(AR6002_REV76))
    if(A_GPIO_REG_READ(IOT_BOOTSTRAP_ADDRESS) & IOT_BOOTSTRAP_QFN56_PKG_STATUS_MASK){

//  configure GPIO22/23 for SPI141 board's flash PIN control
        gpio_configure_pin(22,GPIO_PIN_FUNCTION,GPIO_PIN_DIR_OUTPUT,GPIO_PIN_PULLUP); 
        gpio_configure_pin(23,GPIO_PIN_FUNCTION,GPIO_PIN_DIR_OUTPUT,GPIO_PIN_PULLUP); 

// output high

        A_GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, (0x1 << 22));
        A_GPIO_REG_READ(GPIO_OUT_ADDRESS);				// output high

        A_GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, (0x1 << 23));
        A_GPIO_REG_READ(GPIO_OUT_ADDRESS);				// output high
    } else 
    #endif /*!defined(AR6002_REV75)*/
    {
//  configure GPIO22/23 for SPI141 board's flash PIN control
        gpio_configure_pin(19,GPIO_PIN_FUNCTION,GPIO_PIN_DIR_OUTPUT,GPIO_PIN_PULLUP); 
        gpio_configure_pin(20,GPIO_PIN_FUNCTION,GPIO_PIN_DIR_OUTPUT,GPIO_PIN_PULLUP); 

// output high

        A_GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, (0x1 << 19));
        A_GPIO_REG_READ(GPIO_OUT_ADDRESS);				// output high

        A_GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, (0x1 << 20));
        A_GPIO_REG_READ(GPIO_OUT_ADDRESS);				// output high
    }
#endif

    A_PRINTF_ALWAYS("Erase entire flash\n");
//    nvram_erase();
    if (normal_mode) {
        /*
         * Look for flash segments and commit each of them to flash.
         * A flash segment starts with a flash_segment_hdr and contains
         * the bytes to be programmed into flash.  Flash segments are
         * located in RAM at the "_end" of the binary.  A gross assumption
         * made here is that we won't dynamically allocate RAM in this
         * app, since that might wipe out the source of flash content.
         */
        #define FLASH_MAGIC 0x48534c46 /* "FLSH" */
        struct flash_segment_hdr {
            A_UINT32 flash_magic;
            A_UINT32 flash_addr;
            A_UINT32 flash_nbytes;
        };
        struct flash_segment_hdr *hdr;

        extern char _end;
        A_UINT8 *mem_ptr = (A_UINT8 *)&_end;      

        for (;;) {
            hdr = (struct flash_segment_hdr *)mem_ptr;
            if (hdr->flash_magic != FLASH_MAGIC) {
                break;
            }

            if (hdr->flash_nbytes == 0) {
                break;
            }

            A_PRINTF_ALWAYS("Program flash: %d bytes to 0x%x\n", hdr->flash_nbytes, hdr->flash_addr);
            mem_ptr += sizeof(*hdr);
            nvram_write(mem_ptr, hdr->flash_addr, hdr->flash_nbytes);
            if (validate_flash(hdr->flash_addr, mem_ptr, hdr->flash_nbytes) == 0) {
	            result = IOTFLASHOTP_RESULT_FLASH_VALIDATE_FAILED;
                    A_PRINTF_ALWAYS("Flash validation FAILED (0x%x, %d)\n", hdr->flash_addr, hdr->flash_nbytes);
                    break;
            }

            mem_ptr += A_ROUND_UP_PWR2(hdr->flash_nbytes, 4);
        }
    } else { /* Driven via gdb */

        A_ALLOCRAM_INIT(0,0);
        db_buffer = A_ALLOCRAM(64*1024);

        for (;;) {
            db_signal = 1;
            while (db_signal == 1) {
                ;
            }

            if (db_signal == 2) {
                goto done; /* user is done */
            }

            /* gdb has placed new data in buffer, set offset, set length, and cleared signal */

            A_PRINTF_ALWAYS("read flash offset:0x%08X, length %d\n", db_offset, db_length);
            nvram_read(db_buffer, db_offset, db_length);
			//db_signal = 1;
            //A_PRINTF_ALWAYS("Validate flash image...");
            //if (validate_flash(db_offset, db_buffer, db_length) == 0) {
	        //result = IOTFLASHOTP_RESULT_FLASH_VALIDATE_FAILED;
            //    A_PRINTF_ALWAYS("Flash validation FAILED (0x%x, %d)\n", db_offset, db_length);
            //    goto done;
            //}
        }

//nvram_read(db_buffer, 0x35fe4, 64*1024);


    }

done:
    nvram_fini();

    A_PRINTF_ALWAYS("FLASH IS UPDATED\n");
    db_signal = 1;

    return result;
}


/*****************************************************************************
 * iotflashotp_main: main entry for the sequence of operations.  Relies on 
 *  param for routing information. The value of param can effect which 
 *  operations are performed.  param is passed into each operation function
 *  to allow for configuration of that operation. Collects results of each
 *  operations to be used as a return value.
 *  param: controls sequence of operations.  can be used to skip undesired
 *      operations.
 *  returns: Combination of results from operations. Each operation result
 *      is stored in a sub-field of the 32 bits.
 *****************************************************************************/
int flash_op_type=0;
A_INT32
sdkflash_main(A_UINT32 param)
{
    A_INT32 result = 0;

    if((param & IOTFLASHOTP_PARAM_SKIP_OTP) == 0){
        result |= iototpcfg_main(param);
    }

if(!flash_op_type)
{
    if((param & IOTFLASHOTP_PARAM_SKIP_FLASH) == 0){
        result |= write_flash(param);
//        result |= read_flash(param);
    }
}
else
{
    if((param & IOTFLASHOTP_PARAM_SKIP_FLASH) == 0){
//        result |= write_flash(param);
        result |= read_flash(param);
    }

}
    return result;
}

