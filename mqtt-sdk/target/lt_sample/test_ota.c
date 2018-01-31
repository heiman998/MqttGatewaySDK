#include "qcom_common.h"

#include "qca_ota.h"
#include "qca_http_client.h"
#include "qca_task_manage.h"

#define OTA_URL "http://192.168.0.2/ltm3210_lt_sample_ota.bin"
#define OTA_SERVER_PORT 80

unsigned char ota_flag = 0;

static char tmp_buf[32];
unsigned int tmp_len = 0;

int flag = 0;

typedef enum{
    USER_OTA_RESULT_SUCCEED = 0,
    USER_OTA_RESULT_FAILED = 1,
}user_ota_result_t;

typedef int (*user_ota_result_cb_t)(user_ota_result_t result);

static int user_ota_result_cb(user_ota_result_t result)
{
	if(result == USER_OTA_RESULT_SUCCEED){
		printf("ota succeed\n");
		qcom_sys_reset();
	}else{
		printf("ota failed\n");
	}
}

static void user_ota_value_reset()
{
	tmp_len = 0;

	flag = 0;
}

static void user_ota_get_cb(unsigned char *buf, unsigned int len)
{
	int ret = 0;

	// failed
	if(flag == 1){
		return;
	}

	// First block must more than 24 bytes
	if(tmp_len != 32){
		if(len < 32 - tmp_len){
			memcpy(tmp_buf + tmp_len, buf, len);
			tmp_len += len;
			return;
		}else{
			memcpy(tmp_buf + tmp_len, buf, 32 - tmp_len);

			len -= (32 - tmp_len);
			buf += (32 - tmp_len);
			tmp_len = 32;

			ret = qca_ota_write((unsigned char *)tmp_buf, tmp_len);
			if(ret != 0){
				printf("write first block failed(%d)\n", ret);
				flag = 1;
			}
		}
	}

	ret = qca_ota_write(buf, len);
	if(ret != 0){
		printf("write failed(%d)\n", ret);
		flag = 1;
	}
}

int user_ota_start(char *url, unsigned short port, user_ota_result_cb_t cb)
{
    int ret = 0;
	char *pFind;
	static char	url_without_domain[128];
    user_ota_result_t result = USER_OTA_RESULT_SUCCEED;

    pFind = strstr(url, "//");

	pFind = strchr(pFind != NULL ? pFind + 2 : url, '/');

	if (pFind == NULL){
		result = USER_OTA_RESULT_FAILED;
        goto out;
	}

	memset(url_without_domain, 0, sizeof(url_without_domain));
	strcpy(url_without_domain, pFind);
    *pFind = '\0';

	if(qca_ota_start() == -1){
		printf("qca ota start failed\n");
		return -1;
	}

    ret = http_client_startup((unsigned char *)url, port);
	if(ret == 0){
		ret = http_client_get(url_without_domain, NULL, user_ota_get_cb);
		if(ret == 0 && flag == 0){
			printf("HTTP finish.\n");
			ret = qca_ota_finish();
			if (ret == 0){
				printf("OTA success.\n");
				result = USER_OTA_RESULT_SUCCEED;
			}
		}else{
			if(flag == 0)
				printf("user ota failed(get failed)\r\n");
			else
				printf("user ota failed(write failed)\r\n");

			result = USER_OTA_RESULT_FAILED;
            goto out;
		}
	}else{
        printf("user ota failed(startup failed)\r\n");
		result = USER_OTA_RESULT_FAILED;
        goto out;
	}

out:
    http_client_stop();

	user_ota_value_reset();

	ret = qca_ota_finish();
	if(ret != 0){
		result = USER_OTA_RESULT_FAILED;
	}else{
		result = USER_OTA_RESULT_SUCCEED;
	}

    cb(result);

    return -1;
}


static void ota_task(unsigned int arg)
{
	int part = 0;

	part = qca_ota_get_current_partition();

	printf("OTA will begin after 10 second\r\n");
	printf("Current partition is %d\r\n", part);
	qcom_thread_msleep(10000);

	printf("OTA start, url: %s, port: %d\r\n", OTA_URL, OTA_SERVER_PORT);
	
	user_ota_start(OTA_URL, OTA_SERVER_PORT, user_ota_result_cb);
}

void test_ota()
{
	qca_task_start(ota_task, 0, 2048);
}
