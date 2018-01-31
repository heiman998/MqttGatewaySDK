/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#include "osapi.h"
#include "efuse_api.h"
#if defined(EFUSE_SUPPORT)
#include "hw/efuse_reg.h"

/*
 * eFuse must not be enabled for WRITE for more than one second,
 * or it will be PERMANENTLY damaged.  AR6003 hardware is designed
 * to enable/disable power to eFuse around each write in order to
 * avoid this damage.  Software provides an additional safety check.
 */

/* 
 * A writemask of 0x00 allows all bits to be written.
 * A writemask of 0xff prevents any bits from being written.
 */
#define EFUSE_WRITEMASK_DEFAULT 0x00

/* Number of times to try to write each byte */
#define EFUSE_WRITE_COUNT 3

int efuse_wrenb_ref_count; /* write enable reference count */

#if defined(CONFIG_EFUSE_CACHE_SUPPORT)
extern A_UINT8 *efuse_cache;
#endif

LOCAL void
_efuse_enable_write(void)
{
    A_UINT32 enb_mask;
    A_UINT32 status_mask;
    A_UINT32 otp_status;
#if defined(AR6002_REV74)
    A_UINT32 pg_strobe_pw;
    A_UINT32 vddq_settle_time;
    unsigned int efuse_freq_100khz;
#endif // defined(AR6002_REV74)

    efuse_debug_status = EFUSE_DEBUG_STATUS_ENABLING;

#if defined(AR6002_REV74)
#define EFUSE_DESIRED_PG_PULSE_WIDTH_NS 5500    /* in nanoseconds */
#define EFUSE_DESIRED_VDDQ_SETTLE_TIME_NS 20000 /* in nanoseconds */
    /*
     * Set "program (write) strobe pulse width" to just under 6us.
     * A short write pulse width may result in write errors, including "wobbly" bits.
     * An excessively long pulse width is also inadvisable.
     */
    efuse_freq_100khz = A_CORE_FREQUENCY() / (A_USECS_PER_SECOND / 10);
    pg_strobe_pw = (EFUSE_DESIRED_PG_PULSE_WIDTH_NS*efuse_freq_100khz) / (1000*10);
    vddq_settle_time = (EFUSE_DESIRED_VDDQ_SETTLE_TIME_NS*efuse_freq_100khz) / (1000*10);
#endif /* AR6002_REV74 */
#if defined(AR6002_REV75) || defined(AR6002_REV76)
    /* divide CPU PLL by 4 to get 32.5MHz core clock needed for reliable OTP access */
    A_RTC_SOC_REG_WRITE(SOC_CORE_CLK_CTRL_ADDRESS, SOC_CORE_CLK_CTRL_DIV_SET(2));
    A_SYSCLK_CHANGE();
    /* program OTP for 40MHz timing, which also works for < 40MHz */
    A_EFUSE_REG_WRITE(TRS_CSRST_REG_OFFSET, 40);
    A_EFUSE_REG_WRITE(TCPS_REG_OFFSET, 120);
    A_EFUSE_REG_WRITE(TCPH_REG_OFFSET, 80);
    A_EFUSE_REG_WRITE(TPW_REG_OFFSET, 160);
    A_EFUSE_REG_WRITE(TPGSV_REG_OFFSET, 4);
    A_EFUSE_REG_WRITE(TPVSR_REG_OFFSET, 36);
    A_EFUSE_REG_WRITE(TPVSA_REG_OFFSET, 4);
    A_EFUSE_REG_WRITE(TPEH_REG_OFFSET, 120);
    A_EFUSE_REG_WRITE(TPES_REG_OFFSET, 40);
    A_EFUSE_REG_WRITE(TACC_REG_OFFSET, 3);
    A_EFUSE_REG_WRITE(TRW_REG_OFFSET, 1);
    A_EFUSE_REG_WRITE(TRWH_REG_OFFSET, 4);
    A_EFUSE_REG_WRITE(TPVHR_REG_OFFSET, 4);
    A_EFUSE_REG_WRITE(TPRYCEB_REG_OFFSET, 4);
#endif /* AR6002_REV75 || AR6002_REV76 */

    /* enable read/write */
    enb_mask    = OTP_VDD12_EN_SET(1) | OTP_LDO25_EN_SET(1);
    status_mask = OTP_STATUS_VDD12_EN_READY_SET(1) | OTP_STATUS_LDO25_EN_READY_SET(1);

    A_INTR_DISABLE(&efuse_old_intr);
    A_RTC_SOC_REG_WRITE(OTP_OFFSET, enb_mask);
    do {
        otp_status = A_RTC_SOC_REG_READ(OTP_STATUS_OFFSET);
    } while ((otp_status & status_mask) != status_mask);

#if defined(AR6002_REV74)
    /* Settle Time and Pulse Width must be written AFTER power is enabled */
    A_EFUSE_REG_WRITE(VDDQ_SETTLE_TIME_REG_OFFSET, vddq_settle_time);
    (void)A_EFUSE_REG_READ(VDDQ_SETTLE_TIME_REG_OFFSET); /* Flush */

    A_EFUSE_REG_WRITE(PG_STROBE_PW_REG_OFFSET, pg_strobe_pw);
    (void)A_EFUSE_REG_READ(PG_STROBE_PW_REG_OFFSET); /* Flush */
#endif /* AR6002_REV74 */

    /* Enable eFuse for write */
    A_EFUSE_REG_WRITE(EFUSE_WR_ENABLE_REG_OFFSET, EFUSE_WR_ENABLE_REG_V_SET(1));
    efuse_writemask_set(EFUSE_WRITEMASK_DEFAULT);

    efuse_debug_status = -efuse_debug_status;
}

LOCAL void
_efuse_writemask_set(A_UINT8 writemask)
{
    A_EFUSE_REG_WRITE(BITMASK_WR_REG_OFFSET, writemask);
    (void)A_EFUSE_REG_READ(BITMASK_WR_REG_OFFSET); /* Flush */
}

LOCAL A_STATUS
_efuse_write(unsigned int offset, A_UINT8 *buf, unsigned int buflen, unsigned int *nbytes)
{
    unsigned int curr_byte;
    A_STATUS status;

    efuse_debug_status = EFUSE_DEBUG_STATUS_WRITING;

    if (offset + buflen > efuse_num_bytes) {
        efuse_disable();
        A_ASSERT(0);
    }

#if defined(CONFIG_EFUSE_CACHE_SUPPORT)
    /*
     * During normal WLAN operation, efuse is read-only
     * so we read it once during efuse_init and cache
     * the contents for good performance. In an application
     * that writes to efuse, we disable that cache.
     */
    {
        efuse_cache = NULL;
    }
#endif

    status = A_OK;
    for (curr_byte=0; curr_byte<buflen; curr_byte++, offset++) {
        int i;
        A_UINT8 readback;

        /*
         * Try to write each byte several times.
         * Read back the byte written in order to 1) verify that it
         * was written correctly and 2) flush the previous write.
         *
         * For the most part, a single write is sufficient to flip each
         * bit to 1; but sometimes bits are "wobbly" after a single write.
         * Additinal writes may convert a marginal 1 into a solid 1.
         */
        for (i=0; i<EFUSE_WRITE_COUNT; i++) {
            A_EFUSE_REG_WRITE(EFUSE_INTF_OFFSET + (offset<<2), (A_UINT32)buf[curr_byte]);
            readback = (A_UINT8)A_EFUSE_REG_READ(EFUSE_INTF_OFFSET + (offset<<2));
        }
        if (readback != buf[curr_byte]) {
                status = A_EINVAL;
                break; /* failed to write */
        }
    }

    efuse_debug_status = -efuse_debug_status;

    if (nbytes) {
        *nbytes = curr_byte;
    }

    return status;
}

void (*orig_efuse_disable_fn)(void);
extern int cmnos_core_clk_div;

LOCAL void
_efusew_disable(void)
{
    orig_efuse_disable_fn();
    /* restore core clock to what it was before _efuse_enable_write */
    A_RTC_SOC_REG_WRITE(SOC_CORE_CLK_CTRL_ADDRESS, SOC_CORE_CLK_CTRL_DIV_SET(cmnos_core_clk_div));
    A_SYSCLK_CHANGE();
}

/* NB: eFuse READ functions are supplied by rom/otp/efuse.c */
void
efusew_module_install(struct efuse_api_s *tbl)
{
    
    tbl->_efuse_write           = _efuse_write;
    tbl->_efuse_enable_write    = _efuse_enable_write;
    tbl->_efuse_writemask_set   = _efuse_writemask_set;
    orig_efuse_disable_fn       = tbl->_efuse_disable;
    A_ASSERT(orig_efuse_disable_fn);
    tbl->_efuse_disable         = _efusew_disable;
}
#endif /* EFUSE_SUPPORT */
