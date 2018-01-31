/*
    qca_ssl_client test case

    Add the code below to you project to test

>    extern void test_infrared_task(unsigned int arg);
>    test_infrared_task(arg);
 */
#include "sample_main.h"

#include "qca_infrared.h"

/**
* @brief: printf enable
*/
#define ITR_PIN		2				// the gpio connect with the infrared sensor
#define BUF_SIZE	512				// cache buffer
#define RC_TIMER	100				// ms, timer

/**
* @brief: private variables
*/
static unsigned short Remote_Bit_Cnt;
static unsigned short Remote_Bit_All_Cnt;
static unsigned char Remote_Flag_Head_Cnt;
static unsigned char Remote_Flag_Had_Head;
unsigned char Remote_Buf[3][80];
unsigned int irts[BUF_SIZE] = { 0 };


/**
***************************************************************************
*@brief		:	infrared parse
*@param[in]	:	buf		data buffer
*				len		data length
*@return	:	none
*@note		:	when debugging you can use printf, but release it must be disabled
***************************************************************************
*/
void test_infared_handle(unsigned int *buf, int len)
{
	int i = 0;
	unsigned int temp_irts = 0;
	unsigned char k=0, temp_byte=0, temp_bit=0, temp_index=0;

	printf("len : %d\n", len);
	for(i = 0; i < len; i++){
		printf("%d ", buf[i]);
	}
	printf("\n");
#if 1
	for (i = 0; i < len; i++)
	{
		if (buf[i] == 0)
		{
			PRT_T_INFR("i=%d, buf=%d\n", i, buf[i]);
		}
		else //if (buf[i] != 0)
		{
			if ((buf[i] > 12150) && (buf[i] < 14850))
			{
				for (k = 0; k < 80; k++)
				{
					Remote_Buf[Remote_Flag_Head_Cnt][k] = 0;
				}
				Remote_Bit_Cnt = 0;
				Remote_Flag_Head_Cnt++;
				Remote_Flag_Had_Head = 0xff;
			}
			else
			{
				if ((Remote_Flag_Head_Cnt) && (Remote_Flag_Had_Head))
				{
					temp_index = Remote_Flag_Head_Cnt - 1;
					temp_byte = Remote_Bit_Cnt >> 2;
					temp_bit = Remote_Bit_Cnt % 4;
					if ((buf[i] > 890) && (buf[i] < 1660))
					{
						PRT_T_INFR(" 0 ");
						Remote_Bit_Cnt++;
					}
					else if ((buf[i] > 1990) && (buf[i] < 2684))
					{
						PRT_T_INFR(" 1 ");
						Remote_Buf[temp_index][temp_byte] |= 1 << temp_bit;
						Remote_Bit_Cnt++;
					}
					else
					{
						if (Remote_Bit_Cnt == 35)
						{
							if ((buf[i] > 20130) && (buf[i] < 21130))
							{
								PRT_T_INFR(" 1 ");
								Remote_Buf[temp_index][temp_byte] |= 1 << temp_bit;
								Remote_Bit_Cnt++;
							}
							else
							{
								Remote_Flag_Head_Cnt--;
								Remote_Flag_Had_Head = 0x00;
								PRT_T_INFR("-%d-", buf[i]);
							}
						}
						else if (Remote_Bit_Cnt >= 68)
						{
							Remote_Flag_Had_Head = 0x00;
							PRT_T_INFR("\n-next-\n");
						}
						else
						{
							Remote_Flag_Head_Cnt--;
							Remote_Flag_Had_Head = 0x00;
							PRT_T_INFR("-%d-", buf[i]);
						}
					}
					Remote_Bit_All_Cnt++;
				}
				else
				{
					PRT_T_INFR("-%d-", buf[i]);
				}
			}
			//Remote_Bit_Cnt++;
			if ((Remote_Bit_Cnt % 16/*4*/) == 0)
			{
				PRT_T_INFR("\n");
			}
		}
		temp_irts = buf[i];
	}
	PRT_T_INFR("\nframe num=%d\n", Remote_Flag_Head_Cnt);
	PRT_T_INFR("one frame bit cnt=%d\n", Remote_Bit_Cnt);
	PRT_T_INFR("bit cnt=%d\n", Remote_Bit_All_Cnt);
#endif
}
static int flag = 0;

/**
***************************************************************************
*@brief		:	infrared sensor callback function
*@param[in]	:	state	INFRARED_START -> suspend all other threads
*						INFRARED_END -> resume all other threads
*@return	:	none
*@see		:
***************************************************************************
*/
void test_infared_callback(t_infrared_state_type state)
{
	int		ir_cnt;

	if (state == INFRARED_START)
	{
		//== suspend all other threads
		//@to do...

	}
	else if (state == INFRARED_END)
	{
		//== resume all other threads
		//@to do...
		flag = 1;

		ir_cnt = qca_infared_get_data(irts);

		if (ir_cnt > 0)
		{
			PRT_T_INFR("\n@ir_cnt=%d\n", ir_cnt);
			test_infared_handle(irts, ir_cnt);
			Remote_Flag_Head_Cnt = 0;
			Remote_Bit_Cnt = 0;
			Remote_Bit_All_Cnt = 0;
			Remote_Flag_Had_Head = 0;
		}
	}
}

/**
***************************************************************************
*@brief		:	infrared task
*@param[in]	:	arg
*@return	:	none
*@see		:
***************************************************************************
*/
void test_infrared_task(unsigned int arg)
{
	// int i = 0;
	qca_infared_install(ITR_PIN, BUF_SIZE, RC_TIMER, QCOM_GPIO_PIN_INT_FALLING_EDGE, test_infared_callback);

	while (1)
	{
		#if 0
		i++;
		if(i == 10000){
			printf("sleep\n");
			qcom_thread_msleep(1000);
			i = 0;
		}
		printf("i:%d\n", i);
		#endif
		qcom_thread_msleep(1000);
	}
}


