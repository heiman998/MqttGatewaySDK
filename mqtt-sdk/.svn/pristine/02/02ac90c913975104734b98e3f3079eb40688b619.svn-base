#include "qcom_common.h"
#include "string.h"

#include "qca_base64.h"

void test_base64(void)
{
	unsigned char input[] = "pleasure.";
	unsigned char decode[13];
	char output[13];

	memset(output, 0, sizeof(output));

	base64_encode(input, sizeof(input) - 1, output);
	base64_decode(output, decode);

    if(memcmp(decode, input, strlen(input)) == 0){
        printf("test base64 succeed\n");
    }else{
        printf("test base64 failed\n");
    }
}
