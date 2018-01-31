/*
 *
 * Copyright  2013 Qualcomm Atheros, Inc.  All Rights Reserved.
 *
 * Qualcomm Atheros Confidential and Proprietary.
 */
 #ifndef _HWCRYPTO_H
#define _HWCRYPTO_H

#ifndef dprintf
#define dprintf     A_PRINTF_ALWAYS
#endif
#define  CRYPTO_PBASE               0x000C0000    /* crypto register space start */
#define  CRYPTO_BAM_PBASE       CRYPTO_PBASE + 0x4000  /* crypto bam register space start */
#define  CRYPTO_BAM_EE                       0     /* crypto bam execution environment */

#define CRYPTO_MAX_IDXS 32			/**< Max supported sessions */
#define CRYPTO_BAM_PP 2			/**< BAM Pipe Pairs */
#define CRYPTO_MAX_QDEPTH 1	/* H/w queue depth per pipe */
#define CONFIG_CRYPTO_FORCE_UNCACHE 1
#define CRYPTO_MAX_CACHED_IDXS	2	/**< Max supported sessions */


/*RUBY Soc*/
#define CRYPTO_RTC_SOC_CLOCK_ADDRESS        0x4028
#define CRYPTO_WLAN_AHB_CONFIG_ADDRESS    0x10028


/**
 * H/W registers & values
 */
#define CRYPTO_MASK_ALL				~((uint32_t)(0))
#define CRYPTO_MAX_BURST			64

#define CRYPTO_BURST2BEATS(x)			((x >> 3) - 1)

#define CRYPTO_CONFIG_REQ_SIZE(x)		(x << 17)
#define CRYPTO_CONFIG_DOUT_INTR			(0x1 << 3)
#define CRYPTO_CONFIG_DIN_INTR			(0x1 << 2)
#define CRYPTO_CONFIG_DOP_INTR			(0x1 << 1)
#define CRYPTO_CONFIG_HIGH_SPEED_EN		(0x1 << 4)
#define CRYPTO_CONFIG_PIPE_SEL(x)		(x << 5)
#define CRYPTO_CONFIG_LITTLE_ENDIAN		(0x1 << 9)
#define CRYPTO_CONFIG_MAX_REQS(x)		(x << 14)
#define CRYPTO_CONFIG_RESET			0xE001F

#define CRYPTO_ENCR_SEG_CFG_ALG_DES		0x1
#define CRYPTO_ENCR_SEG_CFG_ALG_AES		0x2
#define CRYPTO_ENCR_SEG_CFG_KEY_AES128		(0x0 << 3)
#define CRYPTO_ENCR_SEG_CFG_KEY_AES256		(0x2 << 3)
#define CRYPTO_ENCR_SEG_CFG_KEY_SINGLE_DES	(0x0 << 3)
#define CRYPTO_ENCR_SEG_CFG_KEY_TRIPLE_DES	(0x1 << 3)
#define CRYPTO_ENCR_SEG_CFG_MODE_ECB		(0x0 << 6)
#define CRYPTO_ENCR_SEG_CFG_MODE_CBC		(0x1 << 6)
#define CRYPTO_ENCR_SEG_CFG_MODE_CTR		(0x2 << 6)
#define CRYPTO_ENCR_SEG_CFG_MODE_XTS		(0x3 << 6)
#define CRYPTO_ENCR_SEG_CFG_MODE_CCM		(0x4 << 6)
#define CRYPTO_ENCR_SEG_CFG_ENC			(0x1 << 10)
#define CRYPTO_ENCR_SEG_CFG_LAST			(0x1 << 13)
#define CRYPTO_ENCR_SEG_CFG_HW_KEY			(0x1 << 14)
#define CRYPTO_ENCR_SEG_CFG_PIPE_KEYS		(0x1 << 15)

#define CRYPTO_SET_ENCRYPT(cfg)			((cfg) |= CRYPTO_ENCR_SEG_CFG_ENC)
#define CRYPTO_SET_DECRYPT(cfg)			((cfg) &= ~CRYPTO_ENCR_SEG_CFG_ENC)

#define CRYPTO_GOPROC_SET			0x1
#define CRYPTO_GOPROC_CLR_CNTXT			(0x1 << 1)
#define CRYPTO_GOPROC_RESULTS_DUMP		(0x1 << 2)

#define CRYPTO_AUTH_SEG_CFG_ALG_SHA		0x1
#define CRYPTO_AUTH_SEG_CFG_ALG_AES		0x2
#define CRYPTO_AUTH_SEG_CFG_KEY_AES128		(0x0<<3)
#define CRYPTO_AUTH_SEG_CFG_KEY_AES256		(0x2<<3)
#define CRYPTO_AUTH_SEG_CFG_MODE_HMAC		(0x1 << 6)
#define CRYPTO_AUTH_SEG_CFG_MODE_CMAC         (0x1 << 6)
#define CRYPTO_AUTH_SEG_CFG_SIZE_SHA1		(0x0 << 9)
#define CRYPTO_AUTH_SEG_CFG_SIZE_SHA2		(0x1 << 9)
#define CRYPTO_AUTH_SEG_CFG_SIZE_AES(x)		((x-1) << 9)
#define CRYPTO_AUTH_SEG_CFG_POS_BEFORE		(0x0 << 14)
#define CRYPTO_AUTH_SEG_CFG_POS_AFTER		(0x1 << 14)
#define CRYPTO_AUTH_SEG_CFG_LAST		(0x1 << 16)
#define CRYPTO_AUTH_SEG_CFG_FIRST		(0x1 << 17)
#define CRYPTO_AUTH_SEG_CFG_PIPE_KEYS		(0x1 << 19)
#define CRYPTO_AUTH_SEG_CFG_NONCE_NUM(x)		((x/4) << 20)   //x is the bytes of nonce.
#define CRYPTO_AUTH_SEG_CFG_COMP_EXP_MAC		(0x1 << 24)

#define CRYPTO_BAM_DESC_INT			(0x1 << 15)
#define CRYPTO_BAM_DESC_EOT			(0x1 << 14)
#define CRYPTO_BAM_DESC_EOB			(0x1 << 13)
#define CRYPTO_BAM_DESC_NWD			(0x1 << 12)
#define CRYPTO_BAM_DESC_CMD			(0x1 << 11)
#define CRYPTO_BAM_DESC_LOCK			(0x1 << 10)
#define CRYPTO_BAM_DESC_UNLOCK			(0x1 << 9)

#define CRYPTO_BAM_P_CTRL_EN			(0x1 << 1)
#define CRYPTO_BAM_P_CTRL_SYS_MODE		(0x1 << 5)
#define CRYPTO_BAM_P_CTRL_DIRECTION(n)		((n & 0x1) << 3)
#define CRYPTO_BAM_P_CTRL_LOCK_GROUP(n)		(n << 16)
#define CRYPTO_BAM_DESC_CNT_TRSHLD_VAL		64
#define CRYPTO_BAM_CNFG_BITS_BAM_FULL_PIPE	(1 << 11)
#define CRYPTO_STATUS_MAC_FAILED   (0x1 << 31)

#define CRYPTO_ADDR_MSK				0x00FFFFFF
#define CRYPTO_CMD_ADDR(addr)		((addr) & CRYPTO_ADDR_MSK)

#define CRYPTO_BASE			0x1A000
#define CRYPTO_SEG_SIZE			(CRYPTO_BASE + 0x110)
#define CRYPTO_GO_PROC			(CRYPTO_BASE + 0x120)
#define CRYPTO_ENCR_SEG_CFG		(CRYPTO_BASE + 0x200)
#define CRYPTO_ENCR_SEG_SIZE		(CRYPTO_BASE + 0x204)
#define CRYPTO_ENCR_SEG_START		(CRYPTO_BASE + 0x208)
#define CRYPTO_ENCR_IVn(n)		(CRYPTO_BASE + 0x20C + (0x4 * n))
#define CRYPTO_ENCR_CCM_INIT_CNTRn(n)     (CRYPTO_BASE + 0x220 + (0x4 * n))
#define CRYPTO_ENCR_CNTR_MASK		(CRYPTO_BASE + 0x21c)
#define CRYPTO_AUTH_SEG_CFG		(CRYPTO_BASE + 0x300)
#define CRYPTO_AUTH_SEG_SIZE 		(CRYPTO_BASE + 0x304)
#define CRYPTO_AUTH_SEG_START		(CRYPTO_BASE + 0X308)
#define CRYPTO_AUTH_IVn(n)		(CRYPTO_BASE + 0x310 + (0x4 * n))
#define CRYPTO_AUTH_INFO_NONCEn(n)   (CRYPTO_BASE + 0x350 + (0x4 * n))
#define CRYPTO_AUTH_BYTECNTn(n)		(CRYPTO_BASE + 0x390+(0x4 * n))
#define CRYPTO_AUTH_EXP_MACn(n)		(CRYPTO_BASE + 0x3a0+(0x4 * n))

#define CRYPTO_CONFIG			(CRYPTO_BASE + 0x400)
#define CRYPTO_ENCR_KEYn(n)		(CRYPTO_BASE + 0x3000 + (0x4 * n))
#define CRYPTO_AUTH_KEYn(n)		(CRYPTO_BASE + 0x3040 + (0x4 * n))
#define CRYPTO_ENCR_PIPEm_KEYn(m, n)	(CRYPTO_BASE + 0x4000 + (0x20 * m) + (0x4 * n))
#define CRYPTO_AUTH_PIPEm_KEYn(m, n)	(CRYPTO_BASE + 0x4800 + (0x80 * m) + (0x4 * n))
#define CRYPTO_STATUS			(CRYPTO_BASE + 0x100)
#define CRYPTO_STATUS2			(CRYPTO_BASE + 0x104)

#define CRYPTO_GO_PROC_QC_KEY			(CRYPTO_BASE + 0x1000)
#define CRYPTO_GO_PROC_OEM_KEY			(CRYPTO_BASE + 0x2000)

#define CRYPTO_DEBUG_ENABLE		(CRYPTO_BASE + 0x5000)
#define CRYPTO_DEBUG_STATUS		(CRYPTO_BASE + 0x5004)

#define CRYPTO_BAM_P_EVNT_REG(n)	(0x1818 + (0x1000 * n))
#define CRYPTO_BAM_P_DESC_FIFO_ADDR(n)	(0x181c + (0x1000 * n))
#define CRYPTO_BAM_P_FIFO_SIZES(n)	(0x1820 + (0x1000 * n))
#define CRYPTO_BAM_P_SW_OFSTS(n)	(0x1800 + (0x1000 * n))
#define CRYPTO_BAM_P_CTRL(n)            (0x1000 + (0x1000 * n))
#define CRYPTO_BAM_P_RST(n)             (0x1004 + (0x1000 * n))

#define CRYPTO_BAM_CTRL			0x0
#define CRYPTO_BAM_DESC_CNT_TRSHLD	0x8
#define CRYPTO_BAM_CNFG_BITS		0x7c

#define CRYPTO_BAM_CTRL_SW_RST		(0x1 << 0)
#define CRYPTO_BAM_CTRL_BAM_EN		(0x1 << 1)

/**
 * H/W specific information
 */
#define CRYPTO_CIPHER_IV_REGS	4 /**< cipher IV regs*/
#define CRYPTO_AUTH_IV_REGS		8 /**< auth IV regs*/
#define CRYPTO_ENCR_CNTR_REGS	4 /**< counter value regs*/
#define CRYPTO_AUTH_NONCE_REGS	4 /**< formatted control information and NONCE regs*/

#define CRYPTO_CKEY_REGS		8 /**< cipher key regs*/
#define CRYPTO_AKEY_REGS		8 /**< auth key regs*/

#define CRYPTO_BCNT_REGS		2

#define CRYPTO_CKEY_SZ		(CRYPTO_CKEY_REGS * sizeof(uint32_t))
#define CRYPTO_AKEY_SZ		(CRYPTO_AKEY_REGS * sizeof(uint32_t))

#define offsetoff(TYPE, MEMBER) ((uint32_t) &((TYPE *)0)->MEMBER)

#define CRYPTO_RESULTS_SZ		sizeof(struct crypto_res_dump)
#define CRYPTO_INDESC_SZ		sizeof(struct crypto_in_trans)
#define CRYPTO_OUTDESC_SZ		sizeof(struct crypto_out_trans)
#define CRYPTO_BAM_DESC_SZ		sizeof(struct crypto_bam_desc)
#define CRYPTO_BAM_CMD_SZ		sizeof(struct crypto_bam_cmd)
#define CRYPTO_DESC_SZ		sizeof(struct crypto_desc)
#define CRYPTO_DESC_ALIGN		8
#define CRYPTO_CACHE_CMD_SZ		offsetoff(struct crypto_cmd_config, keys)
#define CRYPTO_UNCACHE_CMD_SZ	sizeof(struct crypto_cmd_config)
#define CRYPTO_CMD_REQ_SZ		offsetoff(struct crypto_cmd_request, unlock)
#define CRYPTO_CMD_UNLOCK_SZ	CRYPTO_BAM_CMD_SZ

#define CRYPTO_NUM_INDESC		(CRYPTO_INDESC_SZ / CRYPTO_BAM_DESC_SZ)
#define CRYPTO_NUM_OUTDESC		(CRYPTO_OUTDESC_SZ / CRYPTO_BAM_DESC_SZ)
#define CRYPTO_NUM_AUTH_IV		16
#define CRYPTO_NUM_AUTH_BYTECNT	4
#define CRYPTO_NUM_ENCR_CTR		4
#define CRYPTO_NUM_STATUS		2

 #define CRYPTO_BAM_IRQ_SRCS_MSK  0xc4010

#define CRYPTO_INPIPE(_pipe)	((_pipe) - ((_pipe) & 0x1))
#define CRYPTO_OUTPIPE(_pipe)	(CRYPTO_INPIPE((_pipe)) + 1)

/**
 * @brief crypto BAM descriptor
 */
struct crypto_bam_desc {
	uint32_t data_start;	/**< Start of Data*/
	uint16_t data_len;	/**< Length of Data*/
	uint16_t flags;		/**< Status Flags*/
};

/**
 * @brief crypto BAM cmd descriptor
 */
struct crypto_bam_cmd {
	uint32_t addr;		/**< Address to access*/
	uint32_t value;		/**< Addr (for read) or Data (for write)*/
	uint32_t mask;		/**< mask to identify valid bits*/
	uint32_t reserve;	/**< reserved*/
};

/**
 * @brief keys command block, both encryption and authentication keys
 * 	  are kept for uncached mode
 */
struct crypto_cmd_keys {
	struct crypto_bam_cmd encr[CRYPTO_CKEY_REGS];	/**< encryption keys */
	struct crypto_bam_cmd auth[CRYPTO_AKEY_REGS];	/**< authentication keys */
};

/**
 * @brief common configuration command block
 */
struct crypto_cmd_config {
	struct crypto_bam_cmd config_0;				/**< config */

	struct crypto_bam_cmd encr_seg_cfg;				/**< encryption config */
	struct crypto_bam_cmd auth_seg_cfg;				/**< authentication config */

	struct crypto_bam_cmd encr_seg_start;			/**< encryption start offset */
	struct crypto_bam_cmd auth_seg_start;			/**< authentication start offset */

	struct crypto_bam_cmd encr_ctr_msk;				/**< encryption counter mask */
	struct crypto_bam_cmd auth_iv[CRYPTO_AUTH_IV_REGS];	/**< authentication IVs */
	struct crypto_bam_cmd auth_bcnt[CRYPTO_BCNT_REGS];	   /**< authentication bytes count */
	struct crypto_cmd_keys keys;				/**< cipher & auth keys for uncached */
};

/**
 * @brief per request configuration command block
 */
struct crypto_cmd_request {
	struct crypto_bam_cmd seg_size;				/**< total segment size */
	struct crypto_bam_cmd encr_seg_size;			/**< encryption size */
	struct crypto_bam_cmd auth_seg_size;			/**< authentication size */

	struct crypto_bam_cmd encr_iv[CRYPTO_CIPHER_IV_REGS];	/**< encryption IVs */
	struct crypto_bam_cmd encr_cntr[CRYPTO_ENCR_CNTR_REGS];	/**<encryption counter value */
	struct crypto_bam_cmd auth_nonce[CRYPTO_AUTH_NONCE_REGS];	/**< authentication NONCE */   

	struct crypto_bam_cmd config_1;				/**< config, used to switch into little endian */
	struct crypto_bam_cmd go_proc;				/**< crypto trigger, marking config loaded */
	struct crypto_bam_cmd unlock;				/**< dummy write for unlock*/
};

/**
 * @brief crypto command block structure, there is '1' instance of common configuration as it
 * 	  doesn't change per request and there is 'n' request command blocks that change on
 * 	  each request
 */
struct crypto_cmd_block {
	struct crypto_cmd_config cfg;
	struct crypto_cmd_request req[CRYPTO_MAX_QDEPTH];
};


/**
 * @brief results dump format, generated at the end of the operation
 * 	  as an optimization we place the results dump at the end of
 * 	  the packet. The IV contains the generated hash of the operation
 */
struct crypto_res_dump {
	uint32_t auth_iv[CRYPTO_NUM_AUTH_IV];
	uint32_t byte_cnt[CRYPTO_NUM_AUTH_BYTECNT];
	uint32_t encr_ctr[CRYPTO_NUM_ENCR_CTR];
	uint32_t status[CRYPTO_NUM_STATUS];
	uint8_t burst_pad[24]; 				/**< XXX:pad based upon the burst size*/
};


#define crypto_idx_to_inpipe(_idx)		((_idx) << 1)
#define crypto_idx_to_outpipe(_idx)		(((_idx) << 1) + 1)
#define crypto_inpipe_to_idx(_in)		((_in) >> 1)

#define CRYPTO_CMD_CONFIG_SZ		sizeof(struct crypto_cmd_config)
#define CRYPTO_CMD_REQUEST_SZ		sizeof(struct crypto_cmd_request)
#define CRYPTO_INDESC_SZ		sizeof(struct crypto_in_trans)
#define CRYPTO_OUTDESC_SZ		sizeof(struct crypto_out_trans)
#define crypto_inc_idx(_idx)			(((_idx) + 1) % CRYPTO_MAX_QDEPTH)
#define crypto_bam_ring_offset(_idx, _sz)	(crypto_inc_idx(_idx) * _sz)
#define crypto_bam_ring_idx(_bytes, _sz)	((_bytes) / (_sz))
#endif

