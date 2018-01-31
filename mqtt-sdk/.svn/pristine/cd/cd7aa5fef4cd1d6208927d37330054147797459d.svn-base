

#include "qcom_security.h"

static unsigned char md5_src1[] = "323556787456789012956789012056789012=67890123456789012" \
      "345678901234567890";
static unsigned char md5_src2[] = "123456789512345652345678601234567890123450123456789082" \
      "345678901234565890";

static const char md5_digest1[] = { 0x77, 0x4D, 0xCB, 0xDF, 0x65, 0x9B, 0xFD, 0x3A,
	  0x90, 0x29, 0x92, 0xFA, 0xC0, 0xA4, 0xD2, 0x87 };

void test_md5(void)
{
    char result[16];

       qcom_sec_md5_init();
	qcom_sec_md5_update(md5_src1, strlen(md5_src1));
	qcom_sec_md5_update(md5_src2, strlen(md5_src2));
	qcom_sec_md5_final(result);

    if(memcmp(result, md5_digest1, 16) == 0){
        printf("test md5 succeed\n");
    }else{
        printf("test md5 failed\n");
    }
}

