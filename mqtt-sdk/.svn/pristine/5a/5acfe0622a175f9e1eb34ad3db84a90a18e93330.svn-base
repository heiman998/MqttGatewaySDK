/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#ifndef __QCOM_OTA_API_H__
#define __QCOM_OTA_API_H__


extern void qcom_task_exit();


#define QCOM_OTA_PROTOCOL_ID 0xFF
#define QCOM_OTA_MSG_BUF_SIZE 384


#define OTA_POOL_SIZE (4*1024)
#define OTA_STACK_SIZE (3*1024)
#define MD5_CHECKSUM_LEN 16
/* Set to upgrade Target FW. Otherwise, update Host FW */
#define	QCOM_OTA_TARGET_FIRMWARE_UPGRADE	(1<<0)
/* Set to preserve the last active FW */
#define QCOM_OTA_PRESERVE_LAST				(1<<1)
/* Set to erase upgrade partition rw dset */
#define QCOM_OTA_ERASING_RW_DSET            (1<<2)
/* Set to upgrade Module FW */
#define QCOM_OTA_TARGET_MODULE_UPGRADE      (1<<3)

/*
 * OTA Upgrade partitions
 */
#define PARTITION_AUTO 			0	/* Auto-select OTA partition */


typedef enum {
    QCOM_OTA_OK = 0,
    QCOM_OTA_COMPLETED=1,
    QCOM_OTA_START = 2,
    QCOM_OTA_FLASH_AUTH_PENDING=3,
 
    QCOM_OTA_ERR_UNKOWN_MSG = 1000, 
    QCOM_OTA_ERR_IMAGE_NOT_FOUND = 1001, 
    QCOM_OTA_ERR_IMAGE_DOWNLOAD_FAIL = 1002, 
    QCOM_OTA_ERR_IMAGE_CHECKSUM_INCORRECT = 1003,
    QCOM_OTA_ERR_SERVER_RSP_TIMEOUT = 1004,
    QCOM_OTA_ERR_INVALID_FILENAME = 1005,
    QCOM_OTA_ERR_UNSUPPORT_PROTOCOL = 1006,
    QCOM_OTA_ERR_INVALID_PARTITION_INDEX = 1007,
    QCOM_OTA_ERR_IMAGE_HDR_INCORRECT = 1008,
    QCOM_OTA_ERR_INSUFFICIENT_MEMORY=1009,
    QCOM_OTA_ERR_PRESERVE_LAST_FAILED=1010,
    QCOM_OTA_ERR_NO_ACTIVE_OTA_SESSION=1011,
    QCOM_OTA_ERR_INVALID_PARTITION_ACESS=1012,
    QCOM_OTA_ERR_OTA_SESS_IN_PROGRESS=1013,
    QCOM_OTA_ERR_FLASH_READ_TIMEOUT=1014, 
    QCOM_OTA_ERR_FLASH_ERASE_ERROR = 1015,
    QCOM_OTA_ERR_IMAGE_OVERFLOW = 1016,
    QCOM_OTA_ERR_IMAGE_UNDERFLOW = 1017,
    QCOM_OTA_ERR_WRITE_DATA_ERROR = 1018,
    QCOM_OTA_ERR_IMAGE_UPLOAD_TIMEOUT = 1019,
    QCOM_OTA_ERR_IMAGE_UPLOAD_FAIL = 1020,
    QCOM_OTA_ERR_HTTPSVR_HDR_INCORRECT = 1021,
    QCOM_OTA_ERR_PARAMETER_INCORRECT = 1022,

} QCOM_OTA_STATUS_CODE_t;


#define QCOM_OTA_REQUEST 0
#define QCOM_OTA_RESPONSE 1
#define QCOM_OTA_MAX_FILENAME_LEN 256

typedef enum {
    QCOM_OTA_PROTOCOL_TFTP = 0,
    QCOM_OTA_PROTOCOL_FTP,
    QCOM_OTA_PROTOCOL_HTTP,
    QCOM_OTA_PROTOCOL_HTTPS,
    QCOM_OTA_PROTOCOL_MAX = QCOM_OTA_PROTOCOL_TFTP,
}QCOM_OTA_PROTOCOL_t;

typedef struct {
    unsigned char qcom_code;
    unsigned char opcode;
    unsigned char length;
    unsigned char payload[0];
} QCOM_OTA_MSG_HDR_t;

typedef struct{
    A_UINT32 server_ip;
    A_CHAR file_name[QCOM_OTA_MAX_FILENAME_LEN];
    A_UCHAR mode;
    A_UCHAR preserve_last;
    A_UINT8 protocol;
    A_CHAR ota_msg_checksum[MD5_CHECKSUM_LEN];
} QCOM_OTA_MSG_REQUEST_t;

typedef struct{
    int response_code;
    char ota_image_checksum[MD5_CHECKSUM_LEN];
    char ota_msg_checksum[MD5_CHECKSUM_LEN];
} QCOM_OTA_MSG_RESPONSE_t;


void qcom_ota_request_daemon(A_UINT32 device_id, A_UINT32 *resp_code,A_UINT32 *length);
int qcom_ota_daemon_start(A_UINT32 currentDeviceId , A_UINT32 *resp_code,A_UINT32 *ength);
int qcom_ota_daemon_stop(void);


#endif /*__QCOM_OTA_API_H__*/


