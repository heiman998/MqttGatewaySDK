/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef _QCOM_SSL_H__
#define _QCOM_SSL_H__

#define SSL_CIPHERSUITE_LIST_DEPTH 8

#define SSL_PROTOCOL_UNKNOWN                  0x00
#define SSL_PROTOCOL_SSL_3_0                  0x30
#define SSL_PROTOCOL_TLS_1_0                  0x31
#define SSL_PROTOCOL_TLS_1_1                  0x32
#define SSL_PROTOCOL_TLS_1_2                  0x33
#define SSL_PROTOCOL_DTLS_1_0                 0xEF
#define SSL_PROTOCOL_DTLS_1_2                 0xED


/* SSL cipher suites. To be used with SSL_configure(). */
#define TLS_NULL_WITH_NULL_NULL                    0x0000
#define TLS_RSA_WITH_NULL_MD5                      0x0001
#define TLS_RSA_WITH_NULL_SHA                      0x0002
#define TLS_RSA_WITH_RC4_128_MD5                   0x0004
#define TLS_RSA_WITH_RC4_128_SHA                   0x0005
#define TLS_RSA_WITH_DES_CBC_SHA                   0x0009
#define TLS_RSA_WITH_3DES_EDE_CBC_SHA              0x000A
#define TLS_DHE_RSA_WITH_DES_CBC_SHA               0x0015
#define TLS_DHE_RSA_WITH_3DES_EDE_CBC_SHA          0x0016
#define TLS_RSA_WITH_AES_128_CBC_SHA               0x002F
#define TLS_DHE_RSA_WITH_AES_128_CBC_SHA           0x0033
#define TLS_RSA_WITH_AES_256_CBC_SHA               0x0035
#define TLS_DHE_RSA_WITH_AES_256_CBC_SHA           0x0039
#define TLS_RSA_WITH_NULL_SHA256                   0x003B
#define TLS_RSA_WITH_AES_128_CBC_SHA256            0x003C
#define TLS_RSA_WITH_AES_256_CBC_SHA256            0x003D
#define TLS_DHE_RSA_WITH_AES_128_CBC_SHA256        0x0067
#define TLS_DHE_RSA_WITH_AES_256_CBC_SHA256        0x006B
#define TLS_PSK_WITH_RC4_128_SHA                   0x008A
#define TLS_PSK_WITH_3DES_EDE_CBC_SHA              0x008B
#define TLS_PSK_WITH_AES_128_CBC_SHA               0x008C
#define TLS_PSK_WITH_AES_256_CBC_SHA               0x008D
#define TLS_RSA_WITH_AES_128_GCM_SHA256            0x009C
#define TLS_RSA_WITH_AES_256_GCM_SHA384            0x009D
#define TLS_DHE_RSA_WITH_AES_128_GCM_SHA256        0x009E
#define TLS_DHE_RSA_WITH_AES_256_GCM_SHA384        0x009F
#define TLS_ECDH_ECDSA_WITH_NULL_SHA               0xC001
#define TLS_ECDH_ECDSA_WITH_RC4_128_SHA            0xC002
#define TLS_ECDH_ECDSA_WITH_3DES_EDE_CBC_SHA       0xC003
#define TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA        0xC004
#define TLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA        0xC005
#define TLS_ECDHE_ECDSA_WITH_NULL_SHA              0xC006
#define TLS_ECDHE_ECDSA_WITH_RC4_128_SHA           0xC007
#define TLS_ECDHE_ECDSA_WITH_3DES_EDE_CBC_SHA      0xC008
#define TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA       0xC009
#define TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA       0xC00A
#define TLS_ECDH_RSA_WITH_NULL_SHA                 0xC00B
#define TLS_ECDH_RSA_WITH_RC4_128_SHA              0xC00C
#define TLS_ECDH_RSA_WITH_3DES_EDE_CBC_SHA         0xC00D
#define TLS_ECDH_RSA_WITH_AES_128_CBC_SHA          0xC00E
#define TLS_ECDH_RSA_WITH_AES_256_CBC_SHA          0xC00F
#define TLS_ECDHE_RSA_WITH_NULL_SHA                0xC010
#define TLS_ECDHE_RSA_WITH_RC4_128_SHA             0xC011
#define TLS_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA        0xC012
#define TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA         0xC013
#define TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA         0xC014
#define TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256    0xC023
#define TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384    0xC024
#define TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256     0xC025
#define TLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA384     0xC026
#define TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256      0xC027
#define TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384      0xC028
#define TLS_ECDH_RSA_WITH_AES_128_CBC_SHA256       0xC029
#define TLS_ECDH_RSA_WITH_AES_256_CBC_SHA384       0xC02A
#define TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256    0xC02B
#define TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384    0xC02C
#define TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256     0xC02D
#define TLS_ECDH_ECDSA_WITH_AES_256_GCM_SHA384     0xC02E
#define TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256      0xC02F
#define TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384      0xC030
#define TLS_ECDH_RSA_WITH_AES_128_GCM_SHA256       0xC031
#define TLS_ECDH_RSA_WITH_AES_256_GCM_SHA384       0xC032
#define TLS_RSA_WITH_AES_128_CCM                   0xC09C
#define TLS_RSA_WITH_AES_256_CCM                   0xC09D
#define TLS_DHE_RSA_WITH_AES_128_CCM               0xC09E
#define TLS_DHE_RSA_WITH_AES_256_CCM               0xC09F
#define TLS_RSA_WITH_AES_128_CCM_8                 0xC0A0
#define TLS_RSA_WITH_AES_256_CCM_8                 0xC0A1
#define TLS_DHE_RSA_WITH_AES_128_CCM_8             0xC0A2
#define TLS_DHE_RSA_WITH_AES_256_CCM_8             0xC0A3
#define TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8         0xC0AE
#define TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256   0xCC13
#define TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256 0xCC14
#define TLS_DHE_RSA_WITH_CHACHA20_POLY1305_SHA256     0xCC15


typedef struct sslVerifyPolicy {
    /** True to verify certificate commonName against peer's domain name */
    A_UINT8 domain ;
    /** True to verify certificate time validity */
    A_UINT8 timeValidity ;
    /** True to immediately send a fatal alert on detection of untrusted certificate */
    A_UINT8 sendAlert ;
    /** Reserved2 */
    A_UINT8 reserved2 ;
}  SSL_VERIFY_POLICY;

typedef  struct SSL_config {
    A_UINT16 protocol ;
    A_UINT16 cipher[SSL_CIPHERSUITE_LIST_DEPTH] ;
    SSL_VERIFY_POLICY verify ;
    char matchName[40] ;
}  SSL_CONFIG;

enum SslErrors {
    ESSL_OK =              0, // success
    ESSL_DATA_READY =      1, // ssl_decrypt() has decrypted data
    ESSL_INVAL =          -1, // Invalid argument
    ESSL_NOSOCKET =       -2, // No more SSL socket descriptors available
    ESSL_HSNOTDONE =      -3, // Handshake not done
    ESSL_HSDONE =         -4, // Handshake already done
    ESSL_NOMEM =          -5, // Out of memory
    ESSL_CONN =           -6, // SharkSslCon_Error
    ESSL_CERT =           -7, // SharkSslCon_CertificateError
    ESSL_ALERTRECV =      -8, // SharkSslCon_AlertRecv
    ESSL_ALERTFATAL =     -9, // SharkSslCon_AlertSend FATAL received. Connection must be closed.
    ESSL_TIMEOUT =       -10, // Timeout during handshake
    ESSL_NOT_SSL =       -11, // SSL not enabled on connection
    ESSL_OOPS =          -29, // Oops (something is terribly wrong)
    ESSL_OK_HANDSHAKE =  -32, // handshake complete (internal reason code, not an error)

    // Following TRUST reason codes are returned by sslValidate()

    /** The peer's SSL certificate is trusted, CN matches the host name, time is valid */
    ESSL_TRUST_CertCnTime = -32, // Same as ESSL_OK_HANDSHAKE
    /** The peer's SSL certificate is trusted, CN matches the host name, time is expired */
    ESSL_TRUST_CertCn =     -33, // name    OK, time NOTOK
    /** The peer's SSL certificate is trusted, CN does NOT match the host name, time is valid */
    ESSL_TRUST_CertTime =   -34, // name NOTOK, time    OK
    /** The peer's SSL certificate is trusted, CN does NOT match host name, time is expired */
    ESSL_TRUST_Cert =       -35, // name NOTOK, time NOTOK
    /** The peer's SSL certificate is NOT trusted */
    ESSL_TRUST_None =       -36,
};
typedef struct ssl_file_name_list {
    A_UINT8  name[10][20] ; // The file names of the certificates or CA lists
} SSL_FILE_NAME_LIST;

typedef enum
{
  SSL_SERVER = 1,
  SSL_CLIENT = 2
} SSL_ROLE_T;

typedef enum
{
  SSL_CERTIFICATE = 1,
  SSL_CA_LIST = 2
} SSL_CERT_TYPE_T;

typedef enum
{
  SSL_TLS  = 1,
  SSL_DTLS = 2,
} SSL_PROTOCOL_T;

typedef enum
{
  SSL_SHUTDOWN = 1,
}SSL_STATE_T;


typedef void SSL_CTX;
typedef void SSL;
typedef const A_UINT8 *SslCert;
typedef const A_UINT8 *SslCAList;

typedef struct ssl_inst
{
    SSL_CTX*     sslCtx;
    SSL*         ssl;
    SSL_CONFIG   config;
    A_UINT8      config_set;
    SSL_ROLE_T   role;
    SSL_STATE_T state;
} SSL_INST;

typedef struct tmx {
    int sec;  // seconds after the minute 0-61*
    int min;  // minutes after the hour   0-59
    int hour; // hours since midnight     0-23
    int mday; // day of the month         1-31
    int mon;  // months since January     0-11
    int year; // years since 1900
} tmx_t;

extern SSL_INST ssl_inst[];
#define MAX_SSL_INST 1
#define SSL_SERVER_INST              0
#define SSL_CLIENT_INST              0

#endif
