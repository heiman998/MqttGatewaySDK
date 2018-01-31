/** 
* @file 	qca_uart_handle.c	
* @brief	
* @author	
* @version	V0.1.0
* @date		2017.3.01
* @details	
* @copyright 
* @note		
*/


/* Includes ----------------------------------------------------------------*/
#include "ltm3210.h"
#include "qca_uart_handle.h"

#define USER_UART_PACKET_MAX_LEN	256
#define UART_BUF_SIZE				512
char	uart_buf[UART_BUF_SIZE];
/**
* @brief: message snyc header
*/
#define UART_PKT_SYNC			"AT+"

/**
* @brief: message tail
*/
#define UART_PKT_TAIL1			'\r'
#define UART_PKT_TAIL2			'\n'

/**
* @brief: user uart packet base length
*/
#define PACKET_BASE_LEN			3 

extern int qca_uart_get_buf_full_status(void);

/**
* @fn int user_uart_reslove_packet(unsigned char **packet_start, unsigned char **packet_end, \
*			unsigned char *buf, int buf_len, unsigned char *data_start, unsigned char *data_end)
* @brief user reslove uart data
* @param		*buf				receive cache buffer
*				buf_len				receive cache buffer size
*				*data_start			receive data start address in cache buffer
*				*data_end			receive data end address in cache buffer
*				**packet_start		packet start address
*				**packet_end		packet end address
* @return 
*/
int user_uart_reslove_packet(unsigned char **packet_start, unsigned char **packet_end, \
	unsigned char *buf, int buf_len, unsigned char *data_start, unsigned char *data_end)
{
	int							i;
	int							len;
	char						snyc[4];			// back sync header for compare

	//printf("==%s\n", __FUNCTION__);

start:
	*packet_start = NULL;
	*packet_end = NULL;

	//== continuation address
	if (data_end >= data_start)
	{
		//= calc data length
		len = data_end - data_start;

		//= is the header completed ?
		if (len < PACKET_BASE_LEN)
		{
			if (qca_uart_get_buf_full_status())
			{
				return -3;
			}

			return -1;
		}

		//= 1.search the sync
		for (i = 0; i < len; i++)
		{
			if (len - i < PACKET_BASE_LEN)
			{
				return -1;	// search sync header failed
			}

			memcpy((unsigned char *)snyc, data_start, 3);
			snyc[3] = 0;

			if (strncmp(snyc, UART_PKT_SYNC, strlen(UART_PKT_SYNC)) == 0)
			{
				break;
			}

			data_start++;
		}
		if (i == len)
		{
			if (qca_uart_get_buf_full_status())
			{
				return -3;
			}

			return -1;		// search sync header failed
		}

		// save paceket start address
		*packet_start = data_start;				

		//= 2.calc data length
		len = data_end - data_start;

		//= is the header completed ?
		if (len < PACKET_BASE_LEN)
		{
			return -1;		
		}

		//= 3.search the tail
		data_start++;
		for (i = 0; i < len; i++)
		{
			// compare tail
			if ((*data_start == UART_PKT_TAIL1)
				|| (*data_start == UART_PKT_TAIL2))
			{
				break;
			}

			// compare snyc header(�����ٴ��ҵ�ͬ��ͷ������֮ǰ��ͷ������ƥ��)
			if (len - i >= PACKET_BASE_LEN)
			{
				memcpy((unsigned char *)snyc, data_start, 3);
				snyc[3] = 0;
				if (strncmp(snyc, UART_PKT_SYNC, strlen(UART_PKT_SYNC)) == 0)
				{
					goto start;
				}
			}
			data_start++;
		}
		if (i == len)
		{
			if (qca_uart_get_buf_full_status())
			{
				return -3;
			}

			return -1;		// search tail failed
		}

		// save paceket end address
		*packet_end = data_start;

		return 0;
	}
	else //if(data_end < data_start)//== the address of discontinuity
	{
		//= calc data length
		len = buf_len  - (data_start - data_end);

		//= is the header completed ?
		if (len < PACKET_BASE_LEN)
		{
			return -1;
		}

		//= 1.search the sync
		for (i = 0; i < len; i++)
		{
			if (len - i < PACKET_BASE_LEN)
			{
				return -1;	// search sync header failed
			}

			snyc[0] = (char)*data_start;
			snyc[1] = (char)*qca_uart_get_next_addr(data_start, 1);
			snyc[2] = (char)*qca_uart_get_next_addr(data_start, 2);
			snyc[3] = 0;
			if (strncmp(snyc, UART_PKT_SYNC, strlen(UART_PKT_SYNC)) == 0)
			{
				break;
			}

			data_start = qca_uart_get_next_addr(data_start, 1);	//==>data_start++, cycle
		}
		if (i == len)
		{
			if (qca_uart_get_buf_full_status())
			{
				return -3;
			}

			return -1;		// search sync header failed
		}

		// save paceket start address
		*packet_start = data_start;

		//= exchanged to continuation address
		if (data_end >= data_start)
		{
			goto start;
		}

		//= 2.calc data length
		len = buf_len - (data_start - data_end);

		//= is the header completed ?
		if (len < PACKET_BASE_LEN)
		{
			return -1;
		}

		//= 3.search the tail
		data_start = qca_uart_get_next_addr(data_start, 1);		//==>data_start++, cycle
		for (i = 0; i < len; i++)
		{
			// compare tail
			if ((*data_start == UART_PKT_TAIL1)
				|| (*data_start == UART_PKT_TAIL2))
			{
				break;
			}

			// compare snyc header(�����ٴ��ҵ�ͬ��ͷ������֮ǰ��ͷ������ƥ��)
			if (len - i >= PACKET_BASE_LEN)
			{
				snyc[0] = (char)*data_start;
				snyc[1] = (char)*qca_uart_get_next_addr(data_start, 1);
				snyc[2] = (char)*qca_uart_get_next_addr(data_start, 2);
				snyc[3] = 0;
				if (strncmp(snyc, UART_PKT_SYNC, strlen(UART_PKT_SYNC)) == 0)
				{
					goto start;
				}
			}

			data_start = qca_uart_get_next_addr(data_start, 1);		//==>data_start++, cycle
		}
		if (i == len)
		{
			if (qca_uart_get_buf_full_status())
			{
				return -3;
			}

			return -1;		// search tail failed
		}

		// save paceket end address
		*packet_end = data_start;

		return 0;
	}
	return -1;
}

/**
* @fn void qca_uart_recv_cmd(void)
* @brief get packet data from uart queue 
* @param none
* @return none
* @note This api is unblock. It would return if there is not any message
*/
void qca_uart_recv_cmd(void)
{
	int		ret, len;

	//== receive one packet
	memset(uart_buf, 0, USER_UART_PACKET_MAX_LEN);
	ret = qca_uart_get_one_recv_packet((unsigned char *)uart_buf, &len);
	if (ret < 0)
	{
		return;		// not any message
	}

	printf("[UART]recv: %s\n", uart_buf);

	// handle uart data
	if (qca_handle_cmd(UART_Channel, (unsigned char *)uart_buf, len) == NONE_AT_COMMAND)
	{
		if (memcmp(uart_buf, "DP+", 3) == 0)
		{

		}
		else if (memcmp(uart_buf, "RE=", 3) == 0)
		{

		}
		else if (memcmp(uart_buf, "L", 1) == 0)
		{
			if (memcmp(uart_buf + 2, "+", 1) == 0)
			{

			}
		}
	}
}


/**
* @fn void qca_uart_handle_task(unsigned int arg)
* @brief uart handle task
* @param arg entry input
* @return none
*/
void qca_uart_handle_task(unsigned int arg)
{
	while (1)
	{
		qca_uart_recv_cmd();

		qcom_thread_msleep(10);
	}
}


/**
* @fn void qca_start_uart_task(void)
* @brief uart receive and handle test
* @param none
* @return none
*/
void qca_start_uart_task(void)
{
	t_save_param_type *param = &t_save_param;

	qca_uart_task_start(&param->uart_para, 1024, user_uart_reslove_packet);

	// uart data handle task
	qca_task_start(qca_uart_handle_task, 0, 4096);

	qcom_thread_msleep(100);
}



/***************** (C) COPYRIGHT 2017 longsys *****END OF FILE***************/

