/*
 *
 * Copyright  2013 Qualcomm Atheros, Inc.  All Rights Reserved.
 *
 * Qualcomm Atheros Confidential and Proprietary.
 */
#ifndef __HWCRYPTO_CTRL_H
#define __HWCRYPTO_CTRL_H

#define CRYPTO_IDX_BITS	~(0x1 << CRYPTO_MAX_IDXS)

#define CRYPTO_BITS2BYTES(x)	(x / 8)	/**< Bits to Bytes */
#define CRYPTO_BYTES2BITS(x)	(x * 8)	/**< Bytes to bits */

#define CRYPTO_BUF_REQ_MASK  (CRYPTO_BUF_REQ_DECRYPT |CRYPTO_BUF_REQ_ENCRYPT|CRYPTO_BUF_REQ_AUTH |CRYPTO_BUF_SHA_FIRST|CRYPTO_BUF_SHA_PART|CRYPTO_BUF_SHA_LAST)

#define crypto_pp_to_inpipe(_pp)	((_pp) << 1)
#define crypto_pp_to_outpipe(_pp)	(((_pp) << 1) + 1)

extern const uint32_t null_iv[CRYPTO_AUTH_IV_REGS];
extern const uint8_t null_ckey[CRYPTO_CKEY_SZ];
extern const uint8_t null_akey[CRYPTO_AKEY_SZ];

enum crypto_algo {
	CRYPTO_ALGO_NONE = 0,	      /**< Cipher not required*/
	CRYPTO_ALGO_AES,	     /**< AES, CBC for 128-bit & 256-bit key sizes*/
	CRYPTO_ALGO_DES,	    /**< DES, CBC for 64-bit key size */
	CRYPTO_ALGO_SHA1,   /**< SHA1, 160-bit key*/
	CRYPTO_ALGO_SHA256,	   /**< SHA256, 256-bit key*/
	CRYPTO_ALGO_SHA1_HMAC,		/**< SHA1_HMAC,160-bit key*/
	CRYPTO_ALGO_SHA256_HMAC,		/**< SHA256_HMAC,256-bit key*/
	CRYPTO_AUTH_AES128_CCM,        /**< SHA128_CCM,128-bit key*/
	CRYPTO_AUTH_AES256_CCM,        /**< SHA256_CCM,256-bit key*/
	CRYPTO_AUTH_AES128_CMAC,        /**< SHA128_CMAC,128-bit key*/
	CRYPTO_AUTH_AES256_CMAC,        /**< SHA256_CMAC,256-bit key*/	
	CRYPTO_ALGO_HMAC,                /*SHA1_HMAC and SHA256_HMAC*/
	CRYPTO_ALGO_AES_CMAC,          /*SHA128_CMAC and SHA256_CMAC*/
	CRYPTO_ALGO_MAC_CMP,           /*Include HMAC and CMAC MAC compare*/
	CRYPTO_ALGO_MAX
};

/**
 * @brief max key lengths supported for various algorithms
 */
enum crypto_keylen_supp {
	CRYPTO_KEYLEN_AES128 = 16,		/**< AES-128 bit */
	CRYPTO_KEYLEN_AES256 = 32,		/**< AES-256 bit */
	CRYPTO_KEYLEN_SHA1HMAC = 32,	/**< SHA1-HMAC */
	CRYPTO_KEYLEN_SHA256HMAC = 32,	/**< SHA256-HMAC */
	CRYPTO_KEYLEN_DES = 8,		/**< DES-64 bit */
	CRYPTO_KEYLEN_3DES = 24,		/**< 3DES-192 bit */
};

/**
 * @brief session states
 */
enum crypto_session_state {
	CRYPTO_SESSION_STATE_NONE = 0,	/**< session state none */
	CRYPTO_SESSION_STATE_ALLOC = 1,	/**< session state is alloc */
	CRYPTO_SESSION_STATE_FREE = 2	/**< session state is free */
};


/**
 * @brief Crypto status for all crypto_XXX api's
 */
typedef enum crypto_status {
	CRYPTO_STATUS_OK,		/**< OK */
	CRYPTO_STATUS_FAIL,		/**< general failure status */
	CRYPTO_STATUS_EINVAL,	/**< invalid parameters */
	CRYPTO_STATUS_EBUSY,	/**< resource unavailable */
	CRYPTO_STATUS_ERESTART,	/**< resource unavailable, defer the operation */
	CRYPTO_STATUS_ENOMEM,	/**< out of memory */
	CRYPTO_STATUS_ENOSUPP,	/**< unsupported configuration */
	CRYPTO_STATUS_MAC_CMP_FAIL,  /*mac compare fail*/
} crypto_status_t;


/**
 * @brief maximum size of the block for a given cipher
 */
enum crypto_max_blk_len {
	CRYPTO_MAX_BLK_LEN_DES = 8,	/**< maximum block length for DES & 3DES */
	CRYPTO_MAX_BLK_LEN_AES = 16,	/**< maximum block length for AES_CBC */
};


/**
 * @brief max key lengths for supported algorithms
 */
enum crypto_max_keylen {
	CRYPTO_MAX_KEYLEN_AES = 32,		/**< max key size for AES (bytes) */
	CRYPTO_MAX_KEYLEN_SHA1 = 20,	/**< max key size for SHA1 (bytes) */
	CRYPTO_MAX_KEYLEN_SHA256 = 32,	/**< max key size for SHA256 (bytes) */
	CRYPTO_MAX_KEYLEN_DES = 24,		/**< max key size for DES */
};

/**
 * @brief max IV lengths for algorithms supported by the H/W
 */
enum crypto_max_ivlen {
       CRYPTO_MAX_IVLEN_SHA = 0,
	CRYPTO_MAX_IVLEN_DES = 8,		/**< max IV size for DES (bytes) */
	CRYPTO_MAX_IVLEN_AES = 16,		/**< max IV size for AES (bytes) */
};

/**
 * @brief max hash generated for a HMAC algorithm.
 */
enum crypto_max_hashlen {
	CRYPTO_MAX_HASHLEN_SHA1 = 20,	/**< max hash size for SHA1 (bytes) */
	CRYPTO_MAX_HASHLEN_SHA256 = 32,	/**< max hash size for SHA256 (bytes) */
};

/**
 * @brief hash sizes supported by H/W.
 */
enum crypto_hash {
	CRYPTO_HASH_SHA96 = 12,		/**< 96-bit hash size */
	CRYPTO_HASH_SHA128 = 16,		/**< 128-bit hash size */
	CRYPTO_HASH_SHA160 = 20,		/**< 160-bit hash size */
	CRYPTO_HASH_SHA256 = 32		/**< 256-bit hash size */
};

/**
 * @brief supported cipher algorithms
 */
enum crypto_cipher {
	CRYPTO_CIPHER_NONE = 0,		/**< Cipher not required*/
	CRYPTO_CIPHER_AES,			/**< AES, CBC for 128-bit & 256-bit key sizes*/
	CRYPTO_CIPHER_DES,			/**< DES, CBC for 64-bit key size */
	CRYPTO_CIPHER_MAX
};

/**
 * @brief supported authentication algorithms
 */
enum crypto_auth {
	CRYPTO_AUTH_NONE = 0,		/**< Authentication not required*/
	CRYPTO_AUTH_SHA1_HMAC,		/**< SHA1_HMAC,160-bit key*/
	CRYPTO_AUTH_SHA256_HMAC,		/**< SHA256_HMAC,256-bit key*/
	CRYPTO_AUTH_MAX
};

/**
 * @brief crypto buffer request type
 */
enum crypto_buf_req_type {
	CRYPTO_BUF_REQ_DECRYPT = 0x0001,		/**< decryption request*/
	CRYPTO_BUF_REQ_ENCRYPT = 0x0002,		/**< encryption request*/
	CRYPTO_BUF_REQ_AUTH = 0x0004,		/**< authentication request */
	CRYPTO_BUF_SHA_FIRST = 0x0008,    /**< partial SHA1,  first segment*/
	CRYPTO_BUF_SHA_PART = 0x0010,    /**< partial SHA1,  subsequence segment*/
	CRYPTO_BUF_SHA_LAST = 0x0020,    /**< partial SHA1,  last segment*/
};

/**
 * @brief crypto buffer request type
 */
enum crypto_encr_mode {
	CRYPTO_ENCR_MODE_NONE = 0,
	CRYPTO_ENCR_MODE_ECB = CRYPTO_ENCR_SEG_CFG_MODE_ECB,
	CRYPTO_ENCR_MODE_CBC = CRYPTO_ENCR_SEG_CFG_MODE_CBC,
	CRYPTO_ENCR_MODE_CTR = CRYPTO_ENCR_SEG_CFG_MODE_CTR,
	CRYPTO_ENCR_MODE_XTS = CRYPTO_ENCR_SEG_CFG_MODE_XTS,
	CRYPTO_ENCR_MODE_CCM = CRYPTO_ENCR_SEG_CFG_MODE_CCM
};

enum
{
	CRYPTO_KEY_TYPE_SOFTWARE = 0, // default
	CRYPTO_KEY_TYPE_HW_QC,
	CRYPTO_KEY_TYPE_HW_OEM,
};

/**
 * @brief crypto config msg type
 */
enum crypto_config_type {
	CRYPTO_CONFIG_TYPE_NONE = 0,		/**< No config */
	CRYPTO_CONFIG_TYPE_OPEN_ENG,		/**< open engine config */
	CRYPTO_CONFIG_TYPE_CLOSE_ENG,		/**< close engine config */
	CRYPTO_CONFIG_TYPE_RESET_SESSION,		/**< reset session state config */
	CRYPTO_CONFIG_TYPE_MAX
};

enum crypto_sync_type {
	CRYPTO_SYNC_TYPE_NONE = 0,			/**< sync type none */
	CRYPTO_SYNC_TYPE_OPEN_ENG = 1,		/**< open engine sync */
	CRYPTO_SYNC_TYPE_CLOSE_ENG = 2,		/**< close engine sync */
	CRYPTO_SYNC_TYPE_STATS = 3,			/**< stats sync */
	CRYPTO_SYNC_TYPE_MAX
};


/**
 * @brief describe a cipher key
 */
struct crypto_key {
	uint32_t algo;			/**< algorithm for Cipher or Auth*/
	uint32_t key_len;		/**< key length */
	uint8_t *key;			/**< location of the key stored in memory */
       enum crypto_encr_mode mode;  /*encryption mode when algorithm is Cipher*/
};


struct crypto_encr_cfg {
	uint32_t cfg;
	uint8_t key[CRYPTO_CKEY_SZ];
};

struct crypto_auth_cfg {
	uint32_t cfg;
	uint32_t *iv;
	uint8_t key[CRYPTO_AKEY_SZ];
};

struct crypto_ctrl_idx {
	uint16_t pp_num;	/**< pipe pair index */
	uint16_t cmd_len;	/**< command block length to program */
	struct crypto_cmd_block *cblk;
	struct crypto_res_dump  result;
};


/**
 * @brief input transaction set
 */
struct crypto_in_trans {
	struct crypto_bam_desc cmd0_lock;	/**< main cmds & lock pipe*/
	struct crypto_bam_desc cmd1;	/**< secondary commands */
	struct crypto_bam_desc data;	/**< data*/
	struct crypto_bam_desc cmd2_unlock;	/**< unlock pipe */
};

/**
 * @brief output transaction set
 */
struct crypto_out_trans {
	struct crypto_bam_desc data;
	struct crypto_bam_desc results;
};


/**
 * @brief set of descriptors, this gets allocated in one chunk
 *
 * @note  future implementation will decouple all this into separate memory
 *        allocations done from various memories (like DDR, NSS_TCM, Krait L2)
 */
struct crypto_desc {
	struct crypto_in_trans in[CRYPTO_MAX_QDEPTH];
	struct crypto_out_trans out[CRYPTO_MAX_QDEPTH];
};

struct crypto_pipe {
	uint32_t qdepth;	/**< queue depth*/
	uint32_t pidx;		/**< producer index*/
	uint32_t cidx;		/**< consumer index*/
};

/**
 * @brief Crypto control specific structure that describes an Engine
 */
struct crypto_ctrl_eng {
	uint32_t crypto_base;	/**< base address for command descriptors (BAM prespective) */
	uint32_t bam_base;	/**< physical base address for BAM register writes */
	uint32_t bam_ee;	/**< BAM execution enivironment for the crypto engine */

       struct crypto_pipe ctrl_pipe[CRYPTO_BAM_PP];
	struct crypto_desc *hw_desc[CRYPTO_BAM_PP]; 		/**< H/W descriptors BAM rings, command descriptors */
	struct crypto_ctrl_idx idx_tbl[CRYPTO_MAX_IDXS];	/**< index table */
};

/**
 * @brief Main Crypto Control structure, holds information about number of session indexes
 * number of engines etc.,
 *
 * @note currently we support 4 indexes, in future it will allocate more
 */
struct crypto_ctrl {
	uint32_t idx_bitmap;		/**< session allocation bitmap, upto CRYPTO_MAX_IDXS can be used */
	uint32_t idx_state_bitmap;	/**< session state bitmap, upto CRYPTO_MAX_IDXS can be used */
	uint32_t num_idxs;	/**< number of allocated indexes */
       int initFlag;

	struct crypto_ctrl_eng eng;		/**< per engine information */
};

/**
 * @brief crypto buffer state used to describe the current state whether
 * 	  waiting to be processed by the H/W or completed by the H/W
 */
enum crypto_buf_state {
	CRYPTO_BUF_STATE_QUEUED = 0x1,	/**< waiting for processing */
	CRYPTO_BUF_STATE_DONE = 0x2	/**< H/W processing done */
};


/**
 * @brief crypto request buffer for doing operation with the crypto driver
 *
 *        Buffer elements and its use within data
 *
 *        <-- skip ---><-- cipher len --><-- hash len ->
 *        +------+----+-----------------+--------------+----------------------+
 *        |      | IV |     CIPHER      |     HASH     | extra space for H/W  |
 *        +------+----+-----------------+--------------+----------------------+
 *        <-------- hash offset ------->
 *        <----------------- data len ----------------->
 *      				<------- 128 bytes of tailroom ------->
 */
 
struct crypto_buf {
	/* private fields*/
	struct crypto_buf *next;	/**< next buffer */
	uint32_t state;			/**< buffer operation specific state */

	uint32_t session_idx;		/**< session index */
	uint8_t *data;			/**< Data address (virtual) */
    
	uint32_t key_len;		/**< encryption key length */
	uint8_t *key;			/**< location of the encryption key stored in memory */
	uint32_t hash_algo;
      
	uint8_t *hash_buf;		/**< location where HASH is generated */
	uint8_t *iv_buf;		/**< loation where encrypt IV is available */
	uint32_t auth_iv[8];   /**< loation where partial hash IV is available */
	uint32_t auth_bcnt[2];		/**partial hash bytes*/

       
	uint16_t data_len;		/**< Data length */
	uint16_t hash_len;		/**< hash length */

	uint16_t iv_len;		/**< encrypt IV length */
	uint16_t cipher_len;		/**< Length of data to encrypt */

	uint16_t cipher_skip;		/**< start encrypt/decrypt from here */
	uint16_t auth_len;		/**< bytes to authenticate inside data */

	uint16_t auth_skip;		/**< skip bytes from data to start authenticating */
	uint16_t req_type;		/**< crypto_req_type */
    
	uint16_t hash_iv_len;	     /**< partial hash IV length */
	uint8_t pad[6];			/**< pad for 32-byte cacheline alignment */	
    
	uint8_t *nonce;		/**< location where auth info nouce */
	uint8_t *init_cntr;		/**< loation where encrypt ccm init cntr is available */
	uint8_t *auth_key;		 /**< location of the authentication key stored in memory */ 
	uint32_t auth_key_len;		/**< auth key length */

	uint16_t nonce_len;		/**< Data length */
	uint16_t init_cntr_len;		/**< hash length */   
};


static inline A_BOOL crypto_check_idx_state(uint32_t map, uint32_t idx)
{
	return !!(map & (0x1 << idx));
}

static inline void crypto_set_idx_state(uint32_t *map, uint32_t idx)
{
	*map |= (0x1 << idx);
}

static inline void crypto_clear_idx_state(uint32_t *map, uint32_t idx)
{
	*map &= ~(0x1 << idx);
}

#define crypto_swab32(x) \
(\
 	((uint32_t)( \
		(((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) | \
		(((uint32_t)(x) & (uint32_t)0x0000ff00UL) <<  8) | \
		(((uint32_t)(x) & (uint32_t)0x00ff0000UL) >>  8) | \
		(((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24) )) \
)


/**
 * @brief Initialize and allocate descriptor memory for a given pipe
 *
 * @param eng[IN] Engine context for control operation
 * @param idx[IN] Pipe pair index number
 * @param desc_paddr[IN] physical address of H/W descriptor
 * @param desc_vaddr[IN] virtual address of H/W descriptor
 *
 */

crypto_status_t crypto_pipe_init(struct crypto_ctrl_eng *eng, uint32_t idx, struct crypto_desc **desc_vaddr);
/**
 * @brief initiallize the index table per engine
 *
 * @param eng[IN] per engine state
 * @param msg[OUT] message to NSS for each allocated index
 *
 * @return status of the call
 */

crypto_status_t crypto_idx_init(struct crypto_ctrl_eng *eng);
/**
 * @brief update the session with the new buffer parameters
 *
 * @param ctrl[IN] control
 * @param idx[IN] session index
 * @param buf[IN] crypto buf for configuration
 */
void crypto_session_update(struct crypto_ctrl *ctrl, struct crypto_buf *buf);

/**
 * @brief Initiallize the generic control entities in crypto_ctrl
 */
void crypto_ctrl_init(void);

/**
 * @brief Reset session specific parameteres.
 *
 * @param session_idx[IN] session index
 * @param state[IN] session stats (ALLOC/FREE)
 */
void crypto_reset_session(uint32_t session_idx, enum crypto_session_state state);\

crypto_status_t crypto_validate_cipher(struct crypto_key *cipher, struct crypto_encr_cfg *encr_cfg);

crypto_status_t crypto_validate_auth(struct crypto_key *auth, struct crypto_auth_cfg *auth_cfg);


void crypto_key_update(struct crypto_ctrl_eng *eng, uint32_t idx, struct crypto_encr_cfg *encr_cfg,
					struct crypto_auth_cfg *auth_cfg);
					
void crypto_enqueue_hw(struct crypto_ctrl_eng *eng, struct crypto_ctrl_idx *ses_idx, struct crypto_buf *buf);

uint32_t crypto_enqueue_pbuf(struct crypto_buf *crypto_buf);

uint16_t crypto_get_pp_num(uint32_t idx);

void crypto_save_result_dump(struct crypto_buf *buf);

#endif /* __CRYPTO_CTRL_H*/

