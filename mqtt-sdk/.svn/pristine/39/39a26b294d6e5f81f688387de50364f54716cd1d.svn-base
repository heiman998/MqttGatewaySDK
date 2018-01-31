/*
 * Copyright (c) 2016 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#ifndef __QCOM_CRYPTO_H__
#define __QCOM_CRYPTO_H__
/** @file qcom_crypto.h
 * This file contains the unified security QAPIs
 */

/** @typedef
 * Object handle. This can be a cryptographic keypair, public key or simply data
 */
typedef A_UINT32 qcom_crypto_obj_hdl_t;


/** @typedef
 * Handle to the crypto operation
 */
typedef A_UINT32 qcom_crypto_op_hdl_t;


/** @typedef
 * See qcom_crypto_obj_info_get
 */
typedef struct {
    A_UINT32 object_type; 
    A_UINT32 key_size; /* bits */
    A_UINT32 max_key_size; /* bits */
    A_UINT32 object_usage;
    A_UINT32 handle_flags;
} qcom_crypto_obj_info_t;

/** @typedef
 *  * Used in qcom_crypto_op_info_get
 *   */
typedef struct { 
    A_UINT32 algorithm;
    A_UINT32 mode;
    A_UINT32 digest_len;
    A_UINT32 max_key_size;
    A_UINT32 key_size;
    A_UINT32 handle_state;
} qcom_crypto_op_info_t;

/** @enum
 * Return status values
 */
typedef enum {
    A_CRYPTO_ERROR = -1,               /* Generic error return */
    A_CRYPTO_OK = 0,                   /* success */
    A_CRYPTO_ERROR_MAC_INVALID = 0x3071,  /*computed tag does not match the supplied tag*/
} A_CRYPTO_STATUS;

/** @enum
 * Object type. Denotes the contents of the object
 */
typedef enum {
    QCOM_CRYPTO_OBJ_TYPE_AES = 0xA0000010,
    QCOM_CRYPTO_OBJ_TYPE_HMAC_MD5 = 0xA0000001,
    QCOM_CRYPTO_OBJ_TYPE_HMAC_SHA1 = 0xA0000002,
    QCOM_CRYPTO_OBJ_TYPE_HMAC_SHA224 = 0xA0000003,
    QCOM_CRYPTO_OBJ_TYPE_HMAC_SHA256 = 0xA0000004,
    QCOM_CRYPTO_OBJ_TYPE_HMAC_SHA384 = 0xA0000005,
    QCOM_CRYPTO_OBJ_TYPE_HMAC_SHA512 = 0xA0000006,
    QCOM_CRYPTO_OBJ_TYPE_RSA_PUBLIC_KEY = 0xA0000030,
    QCOM_CRYPTO_OBJ_TYPE_RSA_KEYPAIR = 0xA1000030,
    QCOM_CRYPTO_OBJ_TYPE_DH_KEYPAIR = 0xA1000032,
    QCOM_CRYPTO_OBJ_TYPE_ECDSA_PUBLIC_KEY = 0xA0000041,
    QCOM_CRYPTO_OBJ_TYPE_ECDSA_KEYPAIR = 0xA1000041,
    QCOM_CRYPTO_OBJ_TYPE_ECDH_KEYPAIR  = 0xA1000042,
    QCOM_CRYPTO_OBJ_TYPE_GENERIC_SECRET= 0xA0000000,
    QCOM_CRYPTO_OBJ_TYPE_ED25519_PUBLIC_KEY = 0xA00000C0,
    QCOM_CRYPTO_OBJ_TYPE_ED25519_KEYPAIR = 0xA10000C0,
    QCOM_CRYPTO_OBJ_TYPE_CURVE25519_KEYPAIR = 0xA10000C1,
    QCOM_CRYPTO_OBJ_TYPE_SRP_KEYPAIR = 0xA10000C2,
    QCOM_CRYPTO_OBJ_TYPE_CHACHA20 = 0xA00000C3,
} QCOM_CRYPTO_OBJ_TYPE;

/** @enum
 * Algorithm type
 * TODO: May have to delete some depending on what is supported in Ruby/SharkSSL
 */
typedef enum {
    QCOM_CRYPTO_ALG_AES_CBC_NOPAD    = 0x10000110,  
    QCOM_CRYPTO_ALG_AES_CTR  = 0x10000210, 
    QCOM_CRYPTO_ALG_AES_CTS  = 0x10000310,  
    QCOM_CRYPTO_ALG_AES_CBC_MAC_NOPAD    = 0x30000110,  
    QCOM_CRYPTO_ALG_AES_CBC_MAC_PKCS5    = 0x30000510,  
    QCOM_CRYPTO_ALG_AES_CMAC     = 0x30000610,  
    QCOM_CRYPTO_ALG_AES_CCM  = 0x40000710,  
    QCOM_CRYPTO_ALG_AES_GCM  = 0x40000810,  
    QCOM_CRYPTO_ALG_RSASSA_PKCS1_V1_5_SHA1   = 0x70002830,  
    QCOM_CRYPTO_ALG_RSASSA_PKCS1_V1_5_SHA256     = 0x70004830,  
    QCOM_CRYPTO_ALG_RSASSA_PKCS1_V1_5_SHA384     = 0x70005830,  
    QCOM_CRYPTO_ALG_RSASSA_PKCS1_V1_5_SHA512     = 0x70006830,  
    QCOM_CRYPTO_ALG_RSAES_PKCS1_V1_5     = 0x60000130,  
    QCOM_CRYPTO_ALG_RSA_NOPAD    = 0x60000030,  
    QCOM_CRYPTO_ALG_DH_DERIVE_SHARED_SECRET  = 0x80000032,  
    QCOM_CRYPTO_ALG_MD5  = 0x50000001,  
    QCOM_CRYPTO_ALG_SHA1     = 0x50000002,  
    QCOM_CRYPTO_ALG_SHA224   = 0x50000003,  
    QCOM_CRYPTO_ALG_SHA256   = 0x50000004,  
    QCOM_CRYPTO_ALG_SHA384   = 0x50000005,  
    QCOM_CRYPTO_ALG_SHA512   = 0x50000006,  
    QCOM_CRYPTO_ALG_HMAC_MD5     = 0x30000001,  
    QCOM_CRYPTO_ALG_HMAC_SHA1    = 0x30000002,  
    QCOM_CRYPTO_ALG_HMAC_SHA224  = 0x30000003,  
    QCOM_CRYPTO_ALG_HMAC_SHA256  = 0x30000004,  
    QCOM_CRYPTO_ALG_HMAC_SHA384  = 0x30000005,  
    QCOM_CRYPTO_ALG_HMAC_SHA512  = 0x30000006,  
    QCOM_CRYPTO_ALG_ECDSA_P192   = 0x70001042,
    QCOM_CRYPTO_ALG_ECDSA_P224   = 0x70002042,
    QCOM_CRYPTO_ALG_ECDSA_P256   = 0x70003042,
    QCOM_CRYPTO_ALG_ECDSA_P384   = 0x70004042,
    QCOM_CRYPTO_ALG_ECDSA_P521   = 0x70005042,
    QCOM_CRYPTO_ALG_ECDH_P192    = 0x80001042,
    QCOM_CRYPTO_ALG_ECDH_P224    = 0x80002042,
    QCOM_CRYPTO_ALG_ECDH_P256    = 0x80003042,
    QCOM_CRYPTO_ALG_ECDH_P384    = 0x80004042,
    QCOM_CRYPTO_ALG_ECDH_P521    = 0x80005042,
    QCOM_CRYPTO_ALG_ED25519  = 0x700000C0,
    QCOM_CRYPTO_ALG_CURVE25519_DERIVE_SHARED_SECRET  = 0x800000C1,
    QCOM_CRYPTO_ALG_SRP_DERIVE_SHARED_SECRET     = 0x800000C2,
    QCOM_CRYPTO_ALG_CHACHA20_POLY1305    = 0x400000C3,
} QCOM_CRYPTO_ALG;
 
/** @enum
 * Return status values
 */
typedef enum {
    QCOM_CRYPTO_SRP_CLIENT,
    QCOM_CRYPTO_SRP_SERVER,
} QCOM_CRYPTO_SRP_TYPE;

#define QCOM_CRYPTO_SRP_UNAME_MAX_BYTES (64)
#define QCOM_CRYPTO_SRP_PWD_MAX_BYTES (64)
#define QCOM_CRYPTO_SRP_SALT_BYTES (16)

/** @enum
 * Operation mode
 */
typedef enum {
    QCOM_CRYPTO_MODE_ENCRYPT,
    QCOM_CRYPTO_MODE_DECRYPT,
    QCOM_CRYPTO_MODE_SIGN,
    QCOM_CRYPTO_MODE_VERIFY,
    QCOM_CRYPTO_MODE_MAC,
    QCOM_CRYPTO_MODE_DIGEST,
    QCOM_CRYPTO_MODE_DERIVE,
} QCOM_CRYPTO_MODE;

/** @enum
 * Operation hardware key
 */
typedef enum
{
	QCOM_CRYPTO_HW_KEY_QC = 1,
	QCOM_CRYPTO_HW_KEY_OEM,
} QCOM_CRYPTO_HW_KEY;

/** @enum
 * Attribute type
 */
typedef enum {
    QCOM_CRYPTO_ATTR_SECRET_VALUE = 0xC0000000,
    QCOM_CRYPTO_ATTR_ED25519_PUBLIC_VALUE = 0xD00001C0,
    QCOM_CRYPTO_ATTR_ED25519_PRIVATE_VALUE = 0xC00001C0,
    QCOM_CRYPTO_ATTR_CURVE25519_PUBLIC_VALUE = 0xD00001C1,
    QCOM_CRYPTO_ATTR_CURVE25519_PRIVATE_VALUE = 0xC00002C1,
    QCOM_CRYPTO_ATTR_ECC_PUBLIC_VALUE_X = 0xD0000141,
    QCOM_CRYPTO_ATTR_ECC_PUBLIC_VALUE_Y = 0xD0000241,
    QCOM_CRYPTO_ATTR_ECC_PRIVATE_VALUE = 0xC0000341,
    QCOM_CRYPTO_ATTR_ECC_CURVE = 0xF0000441,
    QCOM_CRYPTO_ATTR_DH_PRIME = 0xD0001032,
    QCOM_CRYPTO_ATTR_DH_BASE = 0xD0001232,
    QCOM_CRYPTO_ATTR_DH_PUBLIC_VALUE = 0xD0000132,
    QCOM_CRYPTO_ATTR_DH_PRIVATE_VALUE = 0xC0000232,
    QCOM_CRYPTO_ATTR_RSA_MODULUS = 0xD0000130,
    QCOM_CRYPTO_ATTR_RSA_PUBLIC_EXPONENT = 0xD0000230,
    QCOM_CRYPTO_ATTR_RSA_PRIVATE_EXPONENT = 0xC0000330,
    QCOM_CRYPTO_ATTR_RSA_PRIME1 = 0xC0000430,
    QCOM_CRYPTO_ATTR_RSA_PRIME2 = 0xC0000530,
    QCOM_CRYPTO_ATTR_RSA_EXPONENT1 = 0xC0000630,
    QCOM_CRYPTO_ATTR_RSA_EXPONENT2 = 0xC0000730,
    QCOM_CRYPTO_ATTR_RSA_COEFFICIENT = 0xC0000830,
    QCOM_CRYPTO_ATTR_SRP_PRIME = 0xD00001C2, 
    QCOM_CRYPTO_ATTR_SRP_GEN   = 0xD00002C2, 
    QCOM_CRYPTO_ATTR_SRP_VERIFIER  = 0xC00003C2, 
    QCOM_CRYPTO_ATTR_SRP_USERNAME  = 0xD00004C2, 
    QCOM_CRYPTO_ATTR_SRP_PASSWORD  = 0xC00005C2, 
    QCOM_CRYPTO_ATTR_SRP_TYPE  = 0xD00006C2, 
    QCOM_CRYPTO_ATTR_SRP_HASH  = 0xD00007C2, 
    QCOM_CRYPTO_ATTR_SRP_SALT  = 0xD00008C2, 
    QCOM_CRYPTO_ATTR_SRP_PUBLIC_VALUE  = 0xD00009C2, 
    QCOM_CRYPTO_ATTR_SRP_PRIVATE_VALUE = 0xC0000AC2, 
} QCOM_CRYPTO_ATTR_TYPE;

/** @typedef
 * An attribute can be either a buffer attribute or a value attribute. This is
 * determined by bit [29] of the attribute identifier. If this bit is set to 0,
 * then the attribute is a buffer attribute and the field ref MUST be selected.
 * If the bit is set to 1, then it is a value attribute and the field val
 * MUST be selected.
 */
typedef struct {
    A_UINT32 attrib_id;
    union {
        struct {
            void *buf;
            A_UINT32 len;
        } ref;
        struct {
            A_UINT32 a;
            A_UINT32 b;
        } val;
    } u;
} qcom_crypto_attrib_t;

/** @typedef
 * Object usage flags
 */
typedef enum {
    QCOM_CRYPTO_USAGE_EXTRACTABLE = 0x1,
    QCOM_CRYPTO_USAGE_ENCRYPT = 0x2,
    QCOM_CRYPTO_USAGE_DECRYPT = 0x4,
    QCOM_CRYPTO_USAGE_MAC = 0x8,
    QCOM_CRYPTO_USAGE_SIGN = 0x10,
    QCOM_CRYPTO_USAGE_VERIFY = 0x20,
    QCOM_CRYPTO_USAGE_DERIVE = 0x40,
    QCOM_CRYPTO_USAGE_ALL = (~0),
} QCOM_CRYPTO_USAGE_FLAGS;

/** @typedef
 * Operation class
 */
typedef enum {
    QCOM_CRYPTO_OP_CIPHER,
    QCOM_CRYPTO_OP_MAC,
    QCOM_CRYPTO_OP_AE,
    QCOM_CRYPTO_OP_DIGEST,
    QCOM_CRYPTO_OP_ASYMMETRIC_CIPHER,
    QCOM_CRYPTO_OP_ASYMMETRIC_SIGNATURE,
    QCOM_CRYPTO_OP_KEY_DERIVATION,
} QCOM_CRYPTO_OP_CLASS;

/** @typedef
 * Handle flags
 */
typedef enum {
    QCOM_CRYPTO_HANDLE_INITIALIZED = 0x20000,
    QCOM_CRYPTO_HANDLE_KEY_SET = 0x40000,
} QCOM_CRYPTO_HANDLE_FLAGS;

/* ECC curve types and key sizes */

/* Used in ECC keygen operations and objects */
#define QCOM_CRYPTO_ECC_P192_KEYPAIR_BITS (192 * 2)
#define QCOM_CRYPTO_ECC_P224_KEYPAIR_BITS (224 * 2)
#define QCOM_CRYPTO_ECC_P256_KEYPAIR_BITS (256 * 2)
#define QCOM_CRYPTO_ECC_P384_KEYPAIR_BITS (384 * 2)
#define QCOM_CRYPTO_ECC_P521_KEYPAIR_BITS (528 * 2)

/* Used in ECDH shared secret operations and objects */
#define QCOM_CRYPTO_ECC_P192_SHARED_SECRET_BITS (192)
#define QCOM_CRYPTO_ECC_P224_SHARED_SECRET_BITS (224)
#define QCOM_CRYPTO_ECC_P256_SHARED_SECRET_BITS (256)
#define QCOM_CRYPTO_ECC_P384_SHARED_SECRET_BITS (384)
#define QCOM_CRYPTO_ECC_P521_SHARED_SECRET_BITS (528)

#define QCOM_CRYPTO_ECC_P192_SHARED_SECRET_BYTES (QCOM_CRYPTO_ECC_P192_SHARED_SECRET_BITS/8)
#define QCOM_CRYPTO_ECC_P224_SHARED_SECRET_BYTES (QCOM_CRYPTO_ECC_P224_SHARED_SECRET_BITS/8)
#define QCOM_CRYPTO_ECC_P256_SHARED_SECRET_BYTES (QCOM_CRYPTO_ECC_P256_SHARED_SECRET_BITS/8)
#define QCOM_CRYPTO_ECC_P384_SHARED_SECRET_BYTES (QCOM_CRYPTO_ECC_P384_SHARED_SECRET_BITS/8)
#define QCOM_CRYPTO_ECC_P521_SHARED_SECRET_BYTES (QCOM_CRYPTO_ECC_P521_SHARED_SECRET_BITS/8)

/* Used in ECC public key operations and objects */
#define QCOM_CRYPTO_ECC_P192_PUBLIC_KEY_BITS (192 * 2)
#define QCOM_CRYPTO_ECC_P224_PUBLIC_KEY_BITS (224 * 2)
#define QCOM_CRYPTO_ECC_P256_PUBLIC_KEY_BITS (256 * 2)
#define QCOM_CRYPTO_ECC_P384_PUBLIC_KEY_BITS (384 * 2)
#define QCOM_CRYPTO_ECC_P521_PUBLIC_KEY_BITS (528 * 2)

#define QCOM_CRYPTO_ECC_P192_PUB_VAL_X_BYTES (192/8)
#define QCOM_CRYPTO_ECC_P224_PUB_VAL_X_BYTES (224/8)
#define QCOM_CRYPTO_ECC_P256_PUB_VAL_X_BYTES (256/8)
#define QCOM_CRYPTO_ECC_P384_PUB_VAL_X_BYTES (384/8)
#define QCOM_CRYPTO_ECC_P521_PUB_VAL_X_BYTES (528/8)

#define QCOM_CRYPTO_ECC_P192_PUB_VAL_Y_BYTES (QCOM_CRYPTO_ECC_P192_PUB_VAL_X_BYTES)
#define QCOM_CRYPTO_ECC_P224_PUB_VAL_Y_BYTES (QCOM_CRYPTO_ECC_P224_PUB_VAL_X_BYTES)
#define QCOM_CRYPTO_ECC_P256_PUB_VAL_Y_BYTES (QCOM_CRYPTO_ECC_P256_PUB_VAL_X_BYTES)
#define QCOM_CRYPTO_ECC_P384_PUB_VAL_Y_BYTES (QCOM_CRYPTO_ECC_P384_PUB_VAL_X_BYTES)
#define QCOM_CRYPTO_ECC_P521_PUB_VAL_Y_BYTES (QCOM_CRYPTO_ECC_P521_PUB_VAL_X_BYTES)

/* Used in sign operation */
#define QCOM_CRYPTO_ECC_P192_PRIVATE_KEY_BITS (192)
#define QCOM_CRYPTO_ECC_P224_PRIVATE_KEY_BITS (224)
#define QCOM_CRYPTO_ECC_P256_PRIVATE_KEY_BITS (256)
#define QCOM_CRYPTO_ECC_P384_PRIVATE_KEY_BITS (384)
#define QCOM_CRYPTO_ECC_P521_PRIVATE_KEY_BITS (528)

#define QCOM_CRYPTO_ECC_P192_PRIVATE_KEY_BYTES (QCOM_CRYPTO_ECC_P192_PRIVATE_KEY_BITS/8)
#define QCOM_CRYPTO_ECC_P224_PRIVATE_KEY_BYTES (QCOM_CRYPTO_ECC_P224_PRIVATE_KEY_BITS/8)
#define QCOM_CRYPTO_ECC_P256_PRIVATE_KEY_BYTES (QCOM_CRYPTO_ECC_P256_PRIVATE_KEY_BITS/8)
#define QCOM_CRYPTO_ECC_P384_PRIVATE_KEY_BYTES (QCOM_CRYPTO_ECC_P384_PRIVATE_KEY_BITS/8)
#define QCOM_CRYPTO_ECC_P521_PRIVATE_KEY_BYTES (QCOM_CRYPTO_ECC_P521_PRIVATE_KEY_BITS/8)

#define QCOM_CRYPTO_ED25519_PUBLIC_KEY_BYTES (32)
#define QCOM_CRYPTO_ED25519_PRIVATE_KEY_BYTES (64)
#define QCOM_CRYPTO_ED25519_PUBLIC_KEY_BITS (QCOM_CRYPTO_ED25519_PUBLIC_KEY_BYTES * 8)
#define QCOM_CRYPTO_ED25519_PRIVATE_KEY_BITS (QCOM_CRYPTO_ED25519_PRIVATE_KEY_BYTES * 8)

#define QCOM_CRYPTO_CURVE25519_PUBLIC_KEY_BYTES (32)
#define QCOM_CRYPTO_CURVE25519_PRIVATE_KEY_BYTES (32)
#define QCOM_CRYPTO_CURVE25519_SHARED_SECRET_BYTES (32)
#define QCOM_CRYPTO_CURVE25519_KEYPAIR_BYTES (32)

#define QCOM_CRYPTO_CURVE25519_PUBLIC_KEY_BITS (QCOM_CRYPTO_CURVE25519_PUBLIC_KEY_BYTES * 8)
#define QCOM_CRYPTO_CURVE25519_PRIVATE_KEY_BITS (QCOM_CRYPTO_CURVE25519_PRIVATE_KEY_BYTES * 8)
#define QCOM_CRYPTO_CURVE25519_SHARED_SECRET_BITS (QCOM_CRYPTO_CURVE25519_SHARED_SECRET_BYTES * 8)
#define QCOM_CRYPTO_CURVE25519_KEYPAIR_BITS (QCOM_CRYPTO_CURVE25519_KEYPAIR_BYTES * 8)

/**
 * these values MUST be the same as TLS_NAMEDCURVE_SECPXXXR1 
 * in SharkSslCon.h -- see RFC 4492 sect. 5.1.1
 */
#define QCOM_CRYPTO_ECC_CURVE_NIST_P192 19
#define QCOM_CRYPTO_ECC_CURVE_NIST_P224 21
#define QCOM_CRYPTO_ECC_CURVE_NIST_P256 23
#define QCOM_CRYPTO_ECC_CURVE_NIST_P384 24
#define QCOM_CRYPTO_ECC_CURVE_NIST_P521 25

/* Number of attributes that MUST be provided in *_populate API */
#define QCOM_CRYPTO_OBJ_ATTRIB_COUNT_ECC_KEYPAIR 4
#define QCOM_CRYPTO_OBJ_ATTRIB_COUNT_ECC_PUBLIC_KEY 3
#define QCOM_CRYPTO_OBJ_ATTRIB_COUNT_ED25519_KEYPAIR 2
#define QCOM_CRYPTO_OBJ_ATTRIB_COUNT_ED25519_PUBLIC_KEY 1
#define QCOM_CRYPTO_OBJ_ATTRIB_COUNT_CURVE25519_KEYPAIR 2
#define QCOM_CRYPTO_OBJ_ATTRIB_COUNT_CURVE25519_PUBLIC_KEY 1
#define QCOM_CRYPTO_OBJ_ATTRIB_COUNT_ECDH 2
#define QCOM_CRYPTO_OBJ_ATTRIB_COUNT_DH_POP 4
#define QCOM_CRYPTO_OBJ_ATTRIB_COUNT_RSA_KEYPAIR 8
#define QCOM_CRYPTO_OBJ_ATTRIB_COUNT_RSA_PUBLIC_KEY 2
#define QCOM_CRYPTO_OBJ_ATTRIB_COUNT_SRP_CLIENT 3
#define QCOM_CRYPTO_OBJ_ATTRIB_COUNT_SRP_SERVER 4 /* Does not include optional attributes */

#define QCOM_CRYPTO_OBJ_ATTRIB_COUNT_DH_KEYGEN 2

/*Cipher AES key size, IV size and block size*/
#define QCOM_CRYPTO_AES128_KEY_BYTES (16)
#define QCOM_CRYPTO_AES256_KEY_BYTES (32)
#define QCOM_CRYPTO_AES_IV_BYTES (16)
#define QCOM_CRYPTO_AES_BLOCK_BYTES (16)

#define QCOM_CRYPTO_AES128_KEY_BITS (QCOM_CRYPTO_AES128_KEY_BYTES*8)
#define QCOM_CRYPTO_AES256_KEY_BITS (QCOM_CRYPTO_AES256_KEY_BYTES*8)

/*Cipher CHACHA20_POLY key size, nonce size*/
#define QCOM_CRYPTO_CHACHA20_POLY1305_KEY_BYTES (32)
#define QCOM_CRYPTO_CHACHA20_POLY1305_NONCE_BYTES (12)
#define QCOM_CRYPTO_CHACHA20_POLY1305_KEY_BITS (QCOM_CRYPTO_CHACHA20_POLY1305_KEY_BYTES*8)

/*HMAC-SHA1 key size, block size and mac size*/
#define QCOM_CRYPTO_HMAC_SHA1_MIN_KEY_BYTES (10)
#define QCOM_CRYPTO_HMAC_SHA1_MAX_KEY_BYTES (64)
#define QCOM_CRYPTO_HMAC_SHA1_BLOCK_BYTES (64)
#define QCOM_CRYPTO_HMAC_SHA1_MAC_BYTES (20)

#define QCOM_CRYPTO_HMAC_SHA1_MIN_KEY_BITS (QCOM_CRYPTO_HMAC_SHA1_MIN_KEY_BYTES*8)
#define QCOM_CRYPTO_HMAC_SHA1_MAX_KEY_BITS (QCOM_CRYPTO_HMAC_SHA1_MAX_KEY_BYTES*8)
#define QCOM_CRYPTO_HMAC_SHA1_BLOCK_BITS (QCOM_CRYPTO_HMAC_SHA1_BLOCK_BYTES*8)
#define QCOM_CRYPTO_HMAC_SHA1_MAC_BITS (QCOM_CRYPTO_HMAC_SHA1_MAC_BYTES*8)

/*HMAC-SHA256 key size, block size and mac size*/
#define QCOM_CRYPTO_HMAC_SHA256_MIN_KEY_BYTES (24)
#define QCOM_CRYPTO_HMAC_SHA256_MAX_KEY_BYTES (128)
#define QCOM_CRYPTO_HMAC_SHA256_BLOCK_BYTES (64)
#define QCOM_CRYPTO_HMAC_SHA256_MAC_BYTES (32)

#define QCOM_CRYPTO_HMAC_SHA256_MIN_KEY_BITS (QCOM_CRYPTO_HMAC_SHA256_MIN_KEY_BYTES*8)
#define QCOM_CRYPTO_HMAC_SHA256_MAX_KEY_BITS (QCOM_CRYPTO_HMAC_SHA256_MAX_KEY_BYTES*8)
#define QCOM_CRYPTO_HMAC_SHA256_BLOCK_BITS (QCOM_CRYPTO_HMAC_SHA256_BLOCK_BYTES*8)
#define QCOM_CRYPTO_HMAC_SHA256_MAC_BITS (QCOM_CRYPTO_HMAC_SHA256_MAC_BYTES*8)

/*HMAC-SHA384 key size, block size and mac size*/
#define QCOM_CRYPTO_HMAC_SHA384_MIN_KEY_BYTES (32)
#define QCOM_CRYPTO_HMAC_SHA384_MAX_KEY_BYTES (128)
#define QCOM_CRYPTO_HMAC_SHA384_BLOCK_BYTES (128)
#define QCOM_CRYPTO_HMAC_SHA384_MAC_BYTES (48)

#define QCOM_CRYPTO_HMAC_SHA384_MIN_KEY_BITS (QCOM_CRYPTO_HMAC_SHA384_MIN_KEY_BYTES*8)
#define QCOM_CRYPTO_HMAC_SHA384_MAX_KEY_BITS (QCOM_CRYPTO_HMAC_SHA384_MAX_KEY_BYTES*8)
#define QCOM_CRYPTO_HMAC_SHA384_BLOCK_BITS (QCOM_CRYPTO_HMAC_SHA384_BLOCK_BYTES*8)
#define QCOM_CRYPTO_HMAC_SHA384_MAC_BITS (QCOM_CRYPTO_HMAC_SHA384_MAC_BYTES*8)

/*HMAC-SHA512 key size, block size and mac size*/
#define QCOM_CRYPTO_HMAC_SHA512_MIN_KEY_BYTES (32)
#define QCOM_CRYPTO_HMAC_SHA512_MAX_KEY_BYTES (128)
#define QCOM_CRYPTO_HMAC_SHA512_BLOCK_BYTES (128)
#define QCOM_CRYPTO_HMAC_SHA512_MAC_BYTES (64)

#define QCOM_CRYPTO_HMAC_SHA512_MIN_KEY_BITS (QCOM_CRYPTO_HMAC_SHA512_MIN_KEY_BYTES*8)
#define QCOM_CRYPTO_HMAC_SHA512_MAX_KEY_BITS (QCOM_CRYPTO_HMAC_SHA512_MAX_KEY_BYTES*8)
#define QCOM_CRYPTO_HMAC_SHA512_BLOCK_BITS (QCOM_CRYPTO_HMAC_SHA512_BLOCK_BYTES*8)
#define QCOM_CRYPTO_HMAC_SHA512_MAC_BITS (QCOM_CRYPTO_HMAC_SHA512_MAC_BYTES*8)

/*HMAC-MD5 key size, block size and mac size*/
#define QCOM_CRYPTO_HMAC_MD5_MIN_KEY_BYTES (8)
#define QCOM_CRYPTO_HMAC_MD5_MAX_KEY_BYTES (64)
#define QCOM_CRYPTO_HMAC_MD5_BLOCK_BYTES (64)
#define QCOM_CRYPTO_HMAC_MD5_MAC_BYTES (16)

#define QCOM_CRYPTO_HMAC_MD5_MIN_KEY_BITS (QCOM_CRYPTO_HMAC_MD5_MIN_KEY_BYTES*8)
#define QCOM_CRYPTO_HMAC_MD5_MAX_KEY_BITS (QCOM_CRYPTO_HMAC_MD5_MAX_KEY_BYTES*8)
#define QCOM_CRYPTO_HMAC_MD5_BLOCK_BITS (QCOM_CRYPTO_HMAC_MD5_BLOCK_BYTES*8)
#define QCOM_CRYPTO_HMAC_MD5_MAC_BITS (QCOM_CRYPTO_HMAC_MD5_MAC_BYTES*8)

/*SHA1 block size and mac size*/
#define QCOM_CRYPTO_SHA1_BLOCK_BYTES (64)
#define QCOM_CRYPTO_SHA1_DIGEST_BYTES (20)

/*SHA256 block size and mac size*/
#define QCOM_CRYPTO_SHA256_BLOCK_BYTES (64)
#define QCOM_CRYPTO_SHA256_DIGEST_BYTES (32)

/*SHA384 block size and digest size*/
#define QCOM_CRYPTO_SHA384_BLOCK_BYTES (128)
#define QCOM_CRYPTO_SHA384_DIGEST_BYTES (48)

/*SHA512 block size and digest size*/
#define QCOM_CRYPTO_SHA512_BLOCK_BYTES (128)
#define QCOM_CRYPTO_SHA512_DIGEST_BYTES (64)

/*MD5 block size and digest size*/
#define QCOM_CRYPTO_MD5_BLOCK_BYTES (64)
#define QCOM_CRYPTO_MD5_DIGEST_BYTES (16)


/** @fn qcom_crypto_obj_usage_restrict
 *
 * The qcom_crypto_obj_usage_restrict function restricts the object usage flags
 * of an object handle to  contain at most the flags passed in the  obj_usage
 * parameter.
 * For each bit in the parameter obj_usage:
 * 1. If the bit is set to 1, the corresponding usage flag in the object is
 * left unchanged.
 * 2. If the bit is set to 0, the corresponding usage flag in the object is
 * cleared.
 * For example, if the usage flags of the object are set to
 * QCOM_CRYPTO_USAGE_ENCRYPT | QCOM_CRYPTO_USAGE_DECRYPT and if obj_usage is set
 * to QCOM_CRYPTO_USAGE_ENCRYPT | QCOM_CRYPTO_USAGE_EXTRACTABLE, then the only
 * remaining usage flag in the object after calling the function
 * qcom_crypto_obj_usage_restrict is QCOM_CRYPTO_USAGE_ENCRYPT.
 *
 * Note that an object usage flag can only be cleared. 
 *
 * A transient object's object usage flags are reset to 1 using the
 * qcom_crypto_transient_obj_reset function. 
 *
 * @param hdl [in] Object handle
 * @param obj_usage [in] QCOM_CRYPTO_USAGE_FLAGS
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_obj_usage_restrict(qcom_crypto_obj_hdl_t hdl, A_UINT32 obj_usage);


/** @fn qcom_crypto_transient_obj_reset
 *
 * The  qcom_crypto_transient_obj_reset function resets a transient object to
 * its initial state after allocation.
 * If the object is currently initialized, the function clears the object of all
 * its material. The object is then uninitialized again.
 * In any case, the function resets the key usage of the container to
 * 0xFFFFFFFFF. 
 *
 * @param hdl [in] Object handle
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_transient_obj_reset(qcom_crypto_obj_hdl_t hdl);


/** @fn qcom_crypto_obj_info_get
 *
 * The qcom_crypto_obj_info_get function returns the characteristics of an
 * object. It fills in the following fields in the structure
 * qcom_crypto_object_info_t: 
 * 1. object_type: The parameter object_type passed when the object was
 * created
 * 2. key_size: The current size in bits of the object as determined by its
 * attributes. This will always be less than or equal to max_key_size. Set to 0
 * for uninitialized objects.
 * 3. max_key_size: The maximum key_size which this object can represent.
 * Set to the parameter max_key_size passed to qcom_crypto_transient_obj_alloc
 * 4. obj_usage: A bit vector of the QCOM_CRYPTO_USAGE_FLAGS bits
 * 5. handle_flags: Has QCOM_CRYPTO_HANDLE_INITIALIZED set when the transient object
 * becomes initialized using key_gen or populate API.
 *
 * @param hdl [in] Object handle
 * @param info [out] Buffer to hold object info
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_obj_info_get(qcom_crypto_obj_hdl_t hdl, qcom_crypto_obj_info_t *info);


/** @fn qcom_crypto_transient_obj_alloc
 * Allocate a transient object
 * The qcom_crypto_transient_obj_alloc function allocates an uninitialized
 * transient object, i.e. a container for attributes. Transient objects are used
 * to hold a cryptographic object (key or key-pair). The object type and the
 * maximum key size MUST be specified so that all the container resources can be
 * pre-allocated.
 * As allocated, the container is uninitialized. It can be initialized by
 * subsequently importing the object material, generating an object, deriving an
 * object, or loading an object from the Trusted Storage.
 * The initial value of the key usage associated with the container is
 * 0xFFFFFFFF,  which  means  that it contains all usage flags. You can use the
 * function qcom_crypto_obj_usage_restrict to restrict the usage of the
 * container.
 * The returned handle is used to refer to the newly-created container in all
 * subsequent functions that require an object container: key management and
 * operation functions. The handle remains valid until the  container is
 * deallocated using the function qcom_crypto_transient_obj_free.
 *
 * @param obj_type [in] QCOM_CRYPTO_OBJ_TYPE
 * @param max_key_size [in] Maximum key size
 * @param obj [out] Handle to object allocated
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_transient_obj_alloc(A_UINT32 obj_type, A_UINT32 max_key_size, 
        qcom_crypto_obj_hdl_t *obj);


/* @fn qcom_crypto_transient_obj_keygen
 * The qcom_crypto_key_gen function generates a random key or a key-pair and 
 * populates  a transient  key object with the generated key material.
 * The size of the desired key is passed in the key_size parameter and MUST be 
 * less than or equal to the maximum key size specified when the transient object
 * was created.
 *
 * @param hdl [in] Object handle
 * @param key_size [in] Key size to be generated
 * @param attrs [in] Key attributes
 * @param attr_count [in] Number of attributes
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_transient_obj_keygen(qcom_crypto_obj_hdl_t hdl, A_UINT32 key_size, 
        qcom_crypto_attrib_t *attrs, A_UINT32 attr_count);


/* @fn qcom_crypto_transient_obj_populate
 * The qcom_crypto_transient_obj_populate function populates an uninitialized
 * object container with object attributes passed by the TA in the  attrs
 * parameter.
 * When this function is called, the object MUST be uninitialized. If the object
 * is initialized, the caller MUST first clear it using the function
 * qcom_crypto_transient_obj_reset.
 * Note that if the object type is a key-pair, then this function sets both the
 * private and public parts of the key- pair.
 *
 * @param hdl [in] Object handle
 * @param attrs [in] Key attributes
 * @param attr_count [in] Number of attributes
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_transient_obj_populate(qcom_crypto_obj_hdl_t hdl, qcom_crypto_attrib_t *attrs, A_UINT32 attr_count);

/* @fn qcom_crypto_transient_obj_free
 *
 * The qcom_crypto_transient_obj_free function deallocates a transient object
 * previously allocated with qcom_crypto_transient_obj_alloc. After this function 
 * has been called, the object handle is no longer valid and all resources associated 
 * with the transient object MUST have been reclaimed.
 * If the object is initialized, the object attributes are cleared before the object 
 * is deallocated. 
 *
 * @param hdl [in] Object handle
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_transient_obj_free(qcom_crypto_obj_hdl_t hdl);


/* @fn qcom_crypto_op_info_get
 *
 * The qcom_crypto_op_info_get function returns information about an operation handle. 
 * It fills the following fields in the structure qcom_crypto_op_info_t:
 *
 * 1. algorithm, mode, max_key_size: The parameters passed to the function qcom_crypto_op_alloc
 * 2. key_size: If a key is programmed in the operation, the actual size of this key. 
 * 3. digestLength: For a MAC, AE, or Digest digest, describes the number of bytes in the digest or tag
 * 4. handleState: A bit vector describing the current state of the operation. 
 * Can contain any combination of the following flags or 0 if no flags are appropriate:
 *    o  QCOM_CRYPTO_HANDLE_FLAG_KEY_SET: Set if the operation key has been set. Always set for digest operations.
 *    o  QCOM_CRYPTO_HANDLE_FLAG_INITIALIZED: For multi-stage operations, whether the operation has been initialized 
 *    using one of the qcom_crypto_op_XXX_init functions. This flag is always set for Digest operations.
 *
 * @param hdl [in] Operation handle
 * @param info [out] info
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_info_get(qcom_crypto_obj_hdl_t hdl, qcom_crypto_op_info_t *info);


/* @fn qcom_crypto_op_alloc
 *
 * The qcom_crypto_op_alloc function allocates a handle for a new cryptographic
 * operation and sets the mode and algorithm type. If this function does not
 * return with A_CRYPTO_SUCESS then there is no valid handle value.
 *
 * @param alg [in] QCOM_CRYPTO_ALG
 * @param mode [in] QCOM_CRYPTO_MODE
 * @param max_key_size [in] maximum key size used in the operation
 * @param hdl [out] Operation handle
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_alloc(A_UINT32 alg, A_UINT32 mode, A_UINT32 max_key_size, qcom_crypto_op_hdl_t *hdl);

/* @fn qcom_crypto_op_free
 *
 * The qcom_crypto_op_free function deallocates all resources associated with an
 * operation handle. After this function is called, the operation handle is no
 * longer valid. All cryptographic material in the operation is destroyed.
 *
 * @param hdl [in] Operation handle
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_free(qcom_crypto_op_hdl_t hdl);

/* @fn qcom_crypto_op_reset
 *
 * The qcom_crypto_op_reset function resets the operation to initial state 
 * before initialization, but after the key has been set.This function can be
 * called on any operation and at any time after the key is set, but is 
 * meaningful only for the multi-stage operations, i.e. symmetric ciphers,
 * MACs, AEs, and digests.
 *
 * @param hdl [in] Operation handle
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_reset(qcom_crypto_op_hdl_t hdl);

/* @fn qcom_crypto_op_key_set
 *
 * The qcom_crypto_op_key_set function programs the key of an operation; that is,
 * it associates an operation with a key.
 * The key material is copied from the key object handle into the operation.
 * After the key has been set, there is no longer any link between the operation
 * and the key object. The object handle can be closed or reset and this will
 * not affect the operation. This copied material exists until the operation is
 * freed using qcom_crypto_op_free or another key is set into the operation.
 *
 * The key object type and size MUST be compatible with the type and size of the
 * operation. The operation mode MUST be compatible with key usage:
 *
 * @param op_hdl [in] Operation handle
 * @param obj_hdl [in] Key handle
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_key_set(qcom_crypto_op_hdl_t op_hdl, qcom_crypto_obj_hdl_t obj_hdl);

/* @fn qcom_crypto_op_enable_hw_key
 *
 * The qcom_crypto_op_enable_hw_key function programs the hw key of an operation.
 * No need to call this if with software key. 
 * 
 * The hardware key MUST be compatible with the algo and mode of the
 * operation.
 *
 * @param op_hdl [in] Operation handle
 * @param hw_key [in] Hardware Key
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_enable_hw_key(qcom_crypto_op_hdl_t hdl, QCOM_CRYPTO_HW_KEY hw_key);

/* @fn qcom_crypto_op_copy
 *
 * The qcom_crypto_op_copy function copies an operation state from one 
 * operation handle into another operation handle. This also copies the 
 * key material associated with the source operation.
 *
 * This function is useful in the following use cases:
 * 1. "Forking" a digest operation after feeding some amount of initial data
 * 2. Computing intermediate digests
 *
 * @param dst_hdl [in] Handle on the destination operation
 * @param src_hdl [in] Handle on the source operation
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_copy(qcom_crypto_op_hdl_t dst_hdl, qcom_crypto_op_hdl_t src_hdl);

/* @fn qcom_crypto_op_verify_digest
 *
 * The qcom_crypto_op_verify_digest function verifies a message digest signature
 * within an asymmetric operation.
 *
 * @param hdl [in] Operation handle
 * @param params [in] 
 * @param param_count [in]
 * @param digest [in] Message digest
 * @param digest_len [in]
 * @param sign [in] Signature 
 * @param sign_len [in] 
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_verify_digest(qcom_crypto_op_hdl_t hdl, qcom_crypto_attrib_t *params, A_UINT32 param_count,
                    void *digest, A_UINT32 digest_len, void *sign,  A_UINT32 sign_len);

/* @fn qcom_crypto_op_sign_digest
 *
 * The qcom_crypto_op_sign_digest function signs a message digest within an
 * asymmetric operation. Note that only an already hashed message can be signed.
 *
 * @param hdl [in] Operation handle
 * @param params [in] 
 * @param param_count [in]
 * @param digest [in] Message digest
 * @param digest_len [in]
 * @param sign [out] Signature 
 * @param sign_len [out] 
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_sign_digest(qcom_crypto_op_hdl_t hdl, qcom_crypto_attrib_t *params, A_UINT32 param_count,
                    void *digest, A_UINT32 digest_len, void *sign,  A_UINT32 *sign_len);


/* @fn qcom_crypto_op_cipher_init
 *
 * The qcom_crypto_op_cipher_init function starts the symmetric cipher operation, 
 * The operation MUST have been associated with a key.
 *
 * The supported Symmetric Cipher algorithms are listed below:
 * 1. QCOM_CRYPTO_ALG_AES_CBC_NOPAD
 * 2. QCOM_CRYPTO_ALG_AES_CTR
 * 
 * @param hdl [in] Operation handle
 * @param IV [in]   Buffer containing the operation Initialization Vector or the initial counter value as appropriate
 * @param IVLen [in]  Length in bytes of IV
 * @return A_CRYPTO_STATUS
 */
 A_CRYPTO_STATUS qcom_crypto_op_cipher_init(qcom_crypto_op_hdl_t hdl, void *IV, A_UINT32 IVLen);


/* @fn qcom_crypto_op_cipher_update
 *
 * The qcom_crypto_op_cipher_update function encrypts or decrypts input data.
 * Input data does not have to be a multiple of block size. Subsequent calls to this 
 * function are possible. Unless one or more calls of this function have supplied sufficient
 * input data, no output is generated.The cipher operation is finalized with a call to 
 * qcom_crypto_op_cipher_dofinal. 
 *
 *
 * Note that for the  "*_NOPAD" symmetric algorithms, it is the 
 * responsibility of the Application to do the padding, so input 
 * data must be a multiple of block size.
 *
 * Note that for the QCOM_CRYPTO_ALG_AES_CTR, If one of the
 * chunks supplied to the qcom_crypto_op_cipher_update function
 * was not a multiple of 16 bytes, the destination buffer length
 * have to be at least ( ceil(srcLen/16) * 16 ) bytes long.
 *
 * The supported Symmetric Cipher algorithms are listed below:
 * 1. QCOM_CRYPTO_ALG_AES_CBC_NOPAD
 * 2. QCOM_CRYPTO_ALG_AES_CTR
 *
 * @param hdl [in] Operation handle
 * @param srcData [in]   Input data buffer to be encrypted or decrypted
 * @param srcLen [in]  Legnth of input data
 * @param destData [out]   Output buffer
 * @param destLen [out]  Length in bytes of destData
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_cipher_update(qcom_crypto_op_hdl_t hdl, void *srcData, A_UINT32 srcLen, void *destData, A_UINT32 *destLen);


/* @fn qcom_crypto_op_cipher_dofinal
 *
 * The qcom_crypto_op_cipher_dofinal function finalizes the cipher operation, processing 
 * data that has not been processed by previous calls to
 * qcom_crypto_op_cipher_update as well as data
 * supplied in srcData. The operation handle can be reused or re-initialized. 
 *
 * Note that for the  "*_NOPAD" symmetric algorithms, it is the 
 * responsibility of the Application to do the padding, so input 
 * data must be a multiple of block size.
 *
 * Note that for the QCOM_CRYPTO_ALG_AES_CTR, If one of the
 * chunks supplied to the qcom_crypto_op_cipher_update function
 * was not a multiple of 16 bytes, the destination buffer length
 * have to be at least ( ceil(srcLen/16) * 16 ) bytes long.
 *
 * The supported Symmetric Cipher algorithms are listed below:
 * 1. QCOM_CRYPTO_ALG_AES_CBC_NOPAD
 * 2. QCOM_CRYPTO_ALG_AES_CTR
 * 
 * @param hdl [in] Operation handle
 * @param srcData [in]   Reference to final chunk of input data to be encrypted or decrypted
 * @param srcLen [in]  Length in bytes of srcData
 * @param destData [out]   Output buffer, Can be omitted if the output is to be discarded, e.g. because it is known to be empty
 * @param destLen [out]  Length in bytes of destData
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_cipher_dofinal(qcom_crypto_op_hdl_t hdl, void *srcData, A_UINT32 srcLen, void *destData, A_UINT32 *destLen);

/* @fn qcom_crypto_obj_buf_attrib_get
 *
 * The qcom_crypto_obj_buff_attrib_get function extracts one buffer attribute
 * from an object.
 * The attribute is identified by the argument attribute_id.  
 *
 * There are two kinds of object attributes, which are identified by bit 28 in
 * their handle value. If bit 28 is set to 1, attribute is public. If set to 0,
 * attribute is protected.
 * Public object attributes can always be extracted whatever the status of
 * the container.
 * Protected attributes can be extracted only if the object's key usage
 * contains the QCOM_CRYPTO_USAGE_EXTRACTABLE flag.
 * 
 * @param hdl [in] Object handle
 * @param attrib_id [in] See QCOM_CRYPTO_ATTR_TYPE
 * @param buffer [out] Output buffer that will hold the attribute
 * @param size [out] Size of the output buffer
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_obj_buf_attrib_get(qcom_crypto_obj_hdl_t hdl, A_UINT32 attrib_id, void *buffer, A_UINT32 size);

/* @fn qcom_crypto_obj_val_attrib_get
 *
 * The  qcom_crypto_obj_val_attrib_get function extracts a value attribute from
 * an object. 
 * The attribute is identified by the argument attribute_id.  
 *
 * There are two kinds of object attributes, which are identified by bit 28 in
 * their handle value. If bit 28 is set to 1, attribute is public. If set to 0,
 * attribute is protected.
 * Public object attributes can always be extracted whatever the status of
 * the container.
 * Protected attributes can be extracted only if the object's key usage
 * contains the QCOM_CRYPTO_USAGE_EXTRACTABLE flag.
 *
 * @param hdl [in] Object handle
 * @param attrib_id [in] See QCOM_CRYPTO_ATTR_TYPE
 * @param a [out] Attribute value 
 * @param b [out] Attribute value if size > 4 bytes
 * @return A_CRYPTO_STATUS
 *
 */
A_CRYPTO_STATUS qcom_crypto_obj_val_attrib_get(qcom_crypto_obj_hdl_t hdl, A_UINT32 attrib_id, A_UINT32 *a, A_UINT32 *b);

/** @fn qcom_crypto_rng_get
 * Retrieve len random bytes into *ptr. Uses hardware RNG. 
 * @param buffer pointer to buffer to return random bytes
 * @param len number of random bytes to return
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_rng_get(void *buffer, A_UINT16 len);

/* @fn qcom_crypto_op_ae_init
 *
 * The qcom_crypto_op_ae_init function initializes an Authentication Encryption operation.
 *
 * The supported Authenticated Encryption algorithms are listed below:
 * 1. QCOM_CRYPTO_ALG_AES_CCM
 * 2. QCOM_CRYPTO_ALG_AES_GCM
 * 3. QCOM_CRYPTO_ALG_CHACHA20_POLY1305
 *
 * @param hdl [in] Operation handle
 * @param nonce [in]   The operation nonce or IV
 * @param nonceLen [in]   Length in bytes of  nonce. For AES-GCM, must be 12
 * @param tagLen [in]   Size in bits of the tag.   For AES-CCM, can be 128, 112, 96, 80, 64, 48 or 32. For AES-GCM, must be 128
 * @param AADLen [in]   Length in bytes of the AAD.
 * @param payloadLen: [in]  Length in bytes of the payload
 * @return A_CRYPTO_STATUS
 */
 A_CRYPTO_STATUS qcom_crypto_op_ae_init(qcom_crypto_op_hdl_t hdl, void *nonce, A_UINT32 nonceLen, A_UINT32 tagLen, A_UINT32 AADLen, A_UINT32 payloadLen);

/* @fn qcom_crypto_op_ae_update_aad
 *
 * The qcom_crypto_op_ae_update_aad function feeds a new chunk of Additional Authentication 
 * Data (AAD) to the AE operation.  
 *
 * Note that Subsequent calls to this function are available ONLY for the QCOM_CRYPTO_ALG_CHACHA20_POLY1305 algorithm.
 *
 * The supported Authenticated Encryption algorithms are listed below:
 * 1. QCOM_CRYPTO_ALG_AES_CCM
 * 2. QCOM_CRYPTO_ALG_AES_GCM
 * 3. QCOM_CRYPTO_ALG_CHACHA20_POLY1305
 *
 * @param hdl [in] Operation handle
 * @param AAData [in]   Input buffer containing the chunk of AAD
 * @param AADataLen [in]   Length in bytes of the AAD
 * @return A_CRYPTO_STATUS
 */
 A_CRYPTO_STATUS qcom_crypto_op_ae_update_aad(qcom_crypto_op_hdl_t hdl, void *AAData, A_UINT32 AADataLen);

/* @fn qcom_crypto_op_ae_update
 *
 * The qcom_crypto_op_ae_update function accumulates data for an Authentication Encryption 
 * operation.Input data does not have to be a multiple of block size. Subsequent calls to this 
 * function are possible. Unless one or more calls of this function have supplied sufficient input
 * data, no output is generated. Warning: when using this routine to decrypt the returned data 
 * may be corrupt since the integrity check is not performed until all the data has been processed. 
 * If this is a concern then only use the qcom_crypto_op_ae_*_final routine. 
 *
 * Note that this function is ONLY supported for QCOM_CRYPTO_ALG_CHACHA20_POLY1305 algorithm.
 *
 * Notes related to QCOM_CRYPTO_ALG_CHACHA20_POLY1305 algorithm:
 *  - For optimal performance it is preferable that the input data length is a multiple of 64 bytes.  If the input 
 *    data length of one of the chunks is not a multiple of 64 bytes, an internal buffer will be utilized and this
 *    will slightly impact the performance.
 *  - If the input data length of one of the chunks is not a multiple of 64 bytes, the destination buffer length
 *    for this chunk and subsequent chunks have to be at least ( ceil(srcLen/64) * 64 ) bytes long.
 *
 * @param hdl [in] Operation handle
 * @param srcData [in]  Input data buffer to be encrypted or decrypted
 * @param srcLen [in]  Length in bytes of srcData
 * @param destData [out]   Output buffer
 * @param destLen [out]  Length in bytes of destData
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_ae_update(qcom_crypto_op_hdl_t hdl, void *srcData, A_UINT32 srcLen, void *destData, A_UINT32 *destLen);

/* @fn qcom_crypto_op_ae_encrypt_final
 *
 * The qcom_crypto_op_ae_encrypt_final function processes data that has not been processed 
 * by previous calls to qcom_crypto_op_ae_update as well as data supplied in srcData. It completes 
 * the AE operation and computes the tag.
 *
 * The supported Authenticated Encryption algorithms are listed below:
 * 1. QCOM_CRYPTO_ALG_AES_CCM
 * 2. QCOM_CRYPTO_ALG_AES_GCM
 * 3. QCOM_CRYPTO_ALG_CHACHA20_POLY1305
 *
 * Note that for AES-CCM, the output buffer "destData" must be large enough to contain 
 * "srcLen+AADataLen+tagLen+17" bytes data, 17 is the max length of formatted 
 * AADataLen + formatted AAData padding. Hw crypto engine need the continuous buffer
 * to do AES-CCM algorithm.
 *
 * Note related to QCOM_CRYPTO_ALG_CHACHA20_POLY1305 algorithm:
 *  - If one of the chunks supplied to the qcom_crypto_op_ae_update function was not a multiple of 64 bytes, the destination
 *    buffer length have to be at least ( ceil(srcLen/64) * 64 ) bytes long.
 *  - If the op is to be re-used after the qcom_crypto_op_ae_encrypt_final() is called, the qcom_crypto_op_ae_init() and the
 *    qcom_crypto_op_ae_update_aad() have to be called prior to calling qcom_crypto_op_ae_update or qcom_crypto_op_ae*final()
 *
 * @param hdl [in] Operation handle
 * @param srcData [in]   Reference to final chunk of input data to be encrypted
 * @param srcLen [in]  Length in bytes of srcData
 * @param destData [out]   Output buffer. Can be omitted if the output is to be discarded, e.g. because it is known to be empty
 * @param destLen [out]  Length in bytes of destData.
 * @param tag [out]  Output buffer filled with the computed tag
 * @param tagLen [out]  Length in bits of of tag
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_ae_encrypt_final(qcom_crypto_op_hdl_t hdl, void *srcData, A_UINT32 srcLen, void *destData, A_UINT32 *destLen, void *tag, A_UINT32 *tagLen);

/* @fn qcom_crypto_op_ae_decrypt_final
 *
 * The qcom_crypto_op_ae_decrypt_final function processes data that has not been processed by 
 * previous calls to qcom_crypto_ae_update as well as data supplied in srcData. It completes the AE 
 * operation and compares the computed tag with the tag supplied in the parameter tag. 
 *
 * The supported Authenticated Encryption algorithms are listed below:
 * 1. QCOM_CRYPTO_ALG_AES_CCM
 * 2. QCOM_CRYPTO_ALG_AES_GCM
 * 3. QCOM_CRYPTO_ALG_CHACHA20_POLY1305
 *
 * Note that for AES-CCM, the output buffer "destData" must be large enough to contain 
 * "srcLen+AADataLen+tagLen+17" bytes data, 17 is the max length of formatted 
 * AADataLen + formatted AAData padding. Hw crypto engine need the continuous buffer
 * to do AES-CCM algorithm.
 *
 * Note related to QCOM_CRYPTO_ALG_CHACHA20_POLY1305 algorithm:
 *  - If one of the chunks supplied to the qcom_crypto_op_ae_update function was not a multiple of 64 bytes, the destination
 *    buffer length have to be at least ( ceil(srcLen/64) * 64 ) bytes long.
 *  - If the op is to be re-used after the qcom_crypto_op_ae_decrypt_final() is called, the qcom_crypto_op_ae_init() and the
 *    qcom_crypto_op_ae_update_aad() have to be called prior to calling qcom_crypto_op_ae_update or qcom_crypto_op_ae*final()
 *
 * @param hdl [in] Operation handle
 * @param srcData [in]   Reference to final chunk of input data to be encrypted
 * @param srcLen [in]  Length in bytes of srcData
 * @param destData [out]   Output buffer. Can be omitted if the output is to be discarded, e.g. because it is known to be empty
 * @param destLen [out]  Length in bytes of destData
 * @param tag [in]  Input buffer containing the tag to compare
 * @param tagLen [in]  Length in bits of of tag
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_ae_decrypt_final(qcom_crypto_op_hdl_t hdl, void *srcData, A_UINT32 srcLen, void *destData, A_UINT32 *destLen, void *tag, A_UINT32 tagLen);

/* @fn qcom_crypto_op_key_derive
 *
 * The qcom_crypto_op_key_derive is used to derive a key using a shared secret
 * algorithm. 
 *
 * The derived_key_hdl MUST refer to an object with type
 * QCOM_CRYPTO_OBJ_TYPE_GENERIC_SECRET. On completion the derived key is placed
 * into the QCOM_CRYPTO_ATTR_SECRET_VALUE  attribute of the derivedKey
 * handle.
 * 
 * The supported shared secret algorithms and the corresponding attributes are
 * listed below:
 * 1. QCOM_CRYPTO_ALG_CURVE25519_DERIVE_SHARED_SECRET - QCOM_CRYPTO_ATTR_CURVE25519_PUBLIC_VALUE
 * 2. QCOM_CRYPTO_ALG_ECDH_P* - QCOM_CRYPTO_ATTR_ECC_PUBLIC_VALUE_X and QCOM_CRYPTO_ATTR_ECC_PUBLIC_VALUE_Y 
 * 3. QCOM_CRYPTO_ALG_DH - QCOM_CRYPTO_ATTR_DH_PUBLIC_VALUE
 *
 * @param op_hdl [in] Operation handle
 * @param attrs [in] Attributes (see list above)
 * @param attr_count [in] Number of attributes
 * @param derived_key_hdl [in] Object where the derived key value will be stored
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_key_derive(qcom_crypto_op_hdl_t op_hdl, qcom_crypto_attrib_t *attrs, 
        A_UINT32 attr_count, qcom_crypto_obj_hdl_t derived_key_hdl);
		
/* @fn qcom_crypto_op_mac_init
 *
 * The qcom_crypto_op_mac_init function initializes a MAC operation, The operation MUST have
 * been associated with a key. Note that if IV for MAC operation is constant, IV is ignored and can be NULL.
 *
 * The supported MAC algorithms are listed below:
 * 1. QCOM_CRYPTO_ALG_HMAC_SHA1
 * 2. QCOM_CRYPTO_ALG_HMAC_SHA256
 * 3. QCOM_CRYPTO_ALG_HMAC_SHA384
 * 4. QCOM_CRYPTO_ALG_HMAC_SHA512
 * 5. QCOM_CRYPTO_ALG_HMAC_MD5
 * 6. QCOM_CRYPTO_ALG_AES_CMAC
 *
 * @param hdl [in] Operation handle
 * @param IV [in]   Buffer containing the operation Initialization Vector, if applicable
 * @param IVLen [in]  Length in bytes of IV
 * @return A_CRYPTO_STATUS
 */
 A_CRYPTO_STATUS qcom_crypto_op_mac_init(qcom_crypto_op_hdl_t hdl, void *IV, A_UINT32 IVLen);


/* @fn qcom_crypto_op_mac_update
 *
 * The qcom_crypto_op_mac_update function accumulates data for a MAC calculation.
 * Input data does not have to be a multiple of the block size. Subsequent calls to this
 * function are possible. qcom_crypto_op_mac_compute_final or 
 * qcom_crypto_op_mac_compare_final are called to complete the MAC operation.
 *
 * Note that This function can be called only with an operation of the following algorithms:
 * 1. QCOM_CRYPTO_ALG_HMAC_SHA1
 * 2. QCOM_CRYPTO_ALG_HMAC_SHA256
 * 3. QCOM_CRYPTO_ALG_HMAC_SHA384
 * 4. QCOM_CRYPTO_ALG_HMAC_SHA512
 * 5. QCOM_CRYPTO_ALG_HMAC_MD5
 * 6. QCOM_CRYPTO_ALG_AES_CMAC

 *
 * @param hdl [in] Operation handle
 * @param chunk [in]   Chunk of the message to be MACed
 * @param chunkSize [in]  Length in bytes of chunk
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_mac_update(qcom_crypto_op_hdl_t hdl, void *chunk, A_UINT32 chunkSize);


/* @fn qcom_crypto_op_mac_compute_final
 *
 * The qcom_crypto_op_mac_compute_final function finalizes the MAC operation with a 
 * last chunk of message, and computes the MAC.
 *
 * The supported MAC algorithms are listed below:
 * 1. QCOM_CRYPTO_ALG_HMAC_SHA1
 * 2. QCOM_CRYPTO_ALG_HMAC_SHA256
 * 3. QCOM_CRYPTO_ALG_HMAC_SHA384
 * 4. QCOM_CRYPTO_ALG_HMAC_SHA512
 * 5. QCOM_CRYPTO_ALG_HMAC_MD5
 * 6. QCOM_CRYPTO_ALG_AES_CMAC
 *
 * @param hdl [in] Operation handle
 * @param message [in]   Input buffer containing a last message chunk to MAC
 * @param messageLen [in]  Length in bytes of message
 * @param mac [out]   Output buffer filled with the computed MAC
 * @param macLen [out]  Length in bytes of mac
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_mac_compute_final(qcom_crypto_op_hdl_t hdl, void *message, A_UINT32 messageLen, void *mac, A_UINT32 *macLen);

/* @fn qcom_crypto_op_mac_compare_final
 *
 * The qcom_crypto_op_mac_compare_final function finalizes the MAC operation and 
 * compares the MAC with the buffer passed to the function.
 *
 * The supported MAC algorithms are listed below:
 * 1. QCOM_CRYPTO_ALG_HMAC_SHA1
 * 2. QCOM_CRYPTO_ALG_HMAC_SHA256
 * 3. QCOM_CRYPTO_ALG_HMAC_SHA384
 * 4. QCOM_CRYPTO_ALG_HMAC_SHA512
 * 5. QCOM_CRYPTO_ALG_HMAC_MD5
 * 6. QCOM_CRYPTO_ALG_AES_CMAC
 *
 * @param hdl [in] Operation handle
 * @param message [in]   Input buffer containing a last message chunk to MAC
 * @param messageLen [in]  Length in bytes of message
 * @param mac [in]   Input buffer containing the MAC to check
 * @param macLen [in]  Length in bytes of mac
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_mac_compare_final(qcom_crypto_op_hdl_t hdl, void *message, A_UINT32 messageLen, void *mac, A_UINT32 macLen);

/* @fn qcom_crypto_op_digest_update
 *
 * The qcom_crypto_op_digest_update function accumulates message data for hashing. 
 * The message does not have to be block aligned. Subsequent calls to this function are possible.
 *
 * The supported digest algorithms are listed below:
 * 1. QCOM_CRYPTO_ALG_SHA1
 * 2. QCOM_CRYPTO_ALG_SHA256
 * 3. QCOM_CRYPTO_ALG_SHA384
 * 4. QCOM_CRYPTO_ALG_SHA512
 * 5. QCOM_CRYPTO_ALG_MD5
 *
 * @param hdl [in] Operation handle
 * @param chunk [in]   Chunk of data to be hashed
 * @param chunkSize [in]  Length in bytes of chunk
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_digest_update(qcom_crypto_op_hdl_t hdl, void *chunk, A_UINT32 chunkSize);

/* @fn qcom_crypto_op_digest_dofinal
 *
 * The qcom_crypto_op_digest_dofinal function finalizes the message digest operation 
 * and produces the message hash
 *
 * The supported digest algorithms are listed below:
 * 1. QCOM_CRYPTO_ALG_SHA1
 * 2. QCOM_CRYPTO_ALG_SHA256
 * 3. QCOM_CRYPTO_ALG_SHA384
 * 4. QCOM_CRYPTO_ALG_SHA512
 * 5. QCOM_CRYPTO_ALG_MD5
 *
 * @param hdl [in] Operation handle
 * @param chunk [in]   last chunk of data to be hashed
 * @param chunkSize [in]  Length in bytes of chunk
 * @param hash [out]   Output buffer filled with the message hash
 * @param hashLen [out]  Length in bytes of hash
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_digest_dofinal(qcom_crypto_op_hdl_t hdl, void *chunk, A_UINT32 chunkSize, void *hash, A_UINT32 *hashLen);

/* @fn qcom_crypto_op_asym_encrypt
 *
 * The qcom_crypto_op_asym_encrypt function encrypts a message within an asymmetric operation.
 *
 * The supported asymmetirc encrypt algorithms are listed below:
 * 1. QCOM_CRYPTO_ALG_RSAES_PKCS1_V1_5
 * 2. QCOM_CRYPTO_ALG_RSA_NOPAD
 *
 * @param hdl [in] Operation handle
 * @param params [in] 
 * @param param_count [in]
 * @param srcData [in] Input buffer
 * @param srcLen [in]
 * @param destData [out]  Output buffer
 * @param destlen [out] 
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_asym_encrypt(qcom_crypto_op_hdl_t hdl, qcom_crypto_attrib_t *params, A_UINT32 param_count,
                    void *srcData, A_UINT32 srcLen, void *destData,  A_UINT32* destLen);

/* @fn qcom_crypto_op_asym_decrypt
 *
 * The qcom_crypto_op_asym_decrypt function decrypts a message within an asymmetric operation.
 *
 * The supported asymmetirc decrypt algorithms are listed below:
 * 1. QCOM_CRYPTO_ALG_RSAES_PKCS1_V1_5
 * 2. QCOM_CRYPTO_ALG_RSA_NOPAD
 *
 * @param hdl [in] Operation handle
 * @param params [in] 
 * @param param_count [in]
 * @param srcData [in] Input buffer
 * @param srcLen [in]
 * @param destData [out]  Output buffer
 * @param destlen [out] 
 * @return A_CRYPTO_STATUS
 */
A_CRYPTO_STATUS qcom_crypto_op_asym_decrypt(qcom_crypto_op_hdl_t hdl, qcom_crypto_attrib_t *params, A_UINT32 param_count,
                    void *srcData, A_UINT32 srcLen, void *destData,  A_UINT32* destLen);

#endif
