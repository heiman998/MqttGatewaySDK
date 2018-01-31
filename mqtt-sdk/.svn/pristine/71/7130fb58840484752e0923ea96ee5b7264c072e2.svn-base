#ifndef _USER_OTA2_H_KJSDKJKSDLFJKLJSDFKJDKF
#define _USER_OTA2_H_KJSDKJKSDLFJKLJSDFKJDKF

typedef enum{
    USER_OTA_RESULT_SUCCEED = 0,
    USER_OTA_RESULT_FAILED = 1,
}user_ota_result_t;

typedef int (*user_ota_result_cb_t)(user_ota_result_t result);

int user_ota_start(char *url, unsigned short port, user_ota_result_cb_t cb);


#endif
