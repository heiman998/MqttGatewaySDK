/*
    qca_aes test case

    Add the code below to you project to test

>    extern void test_uart(void);
>    test_uart();
 */
#include "sample_main.h"
#include "qca_uart.h"

#define UART_PACKET_MAX_LEN	256
#define BUFFSIZE 512

unsigned char recv_uart_buf[BUFFSIZE];

static qcom_uart_para uart_paramer;

void uart_recv(void);

int uart_reslove_packet(unsigned char **packet_start, unsigned char **packet_end, 
    unsigned char *buf, int buf_len, unsigned char *data_start, unsigned char *data_end);

/*Each send should be ended by Enter key*/
void test_uart(void)
{
    int ret;
    
    uart_paramer.BaudRate = 115200;
    uart_paramer.number = 8;
    uart_paramer.parity = 0;
    uart_paramer.StopBits = 1;
    uart_paramer.FlowControl = 0;
    
    printf("uart testing......\r\n");
    ret = qca_uart_task_start(&uart_paramer, 1024, uart_reslove_packet);
    if(ret != 0){
        printf("uart start fail.\r\n");
        return;
    }

    while(1){
		uart_recv();

		qcom_thread_msleep(10);
	}

    ret = qca_uart_stop();
    if(ret != 0){
        printf("uart stop fail.\r\n");
        return;
    }
    printf("uart test done.\r\n");
}

void uart_recv(void)
{
    int ret, len;

	//== receive one packet
	memset(recv_uart_buf, 0, UART_PACKET_MAX_LEN);
	ret = qca_uart_get_one_recv_packet((unsigned char *)recv_uart_buf, &len);//
	
	if (ret < 0){
		return;
	}

	printf("[UART]recv: %s\n", recv_uart_buf);

    qca_uart_send(recv_uart_buf, len);
}

int uart_reslove_packet(unsigned char **packet_start, unsigned char **packet_end, 
    unsigned char *buf, int buf_len, unsigned char *data_start, unsigned char *data_end)
{   
	int	i;
	int	len;

start:
	*packet_start = NULL;
	*packet_end = NULL;

	if(data_end >= data_start)
	{
		len = data_end - data_start;

		if(len <= 0)
		{
			if(qca_uart_get_buf_full_status())
			{
				return -3;
			}

			return -1;
		}

		*packet_start = data_start;				

		data_start++;
		for(i = 0; i < len; i++)
        {
			if(*data_start == '\n')
			{
				break;
			}

			data_start++;
		}
		if(i == len)
		{
			if(qca_uart_get_buf_full_status())
			{
				return -3;
			}

			return -1;
		}

		*packet_end = data_start;

		return 0;
	}
	else 
	{
		len = buf_len  - (data_start - data_end);

		if(len <= 0)
		{
			return -1;
		}

		*packet_start = data_start;

		if(data_end >= data_start)
		{
			goto start;
        }

		data_start = qca_uart_get_next_addr(data_start, 1);

		for(i = 0; i < len; i++)
		{
			if(*data_start == '\n')
			{
				break;
			}

			data_start = qca_uart_get_next_addr(data_start, 1);
        }

		*packet_end = data_start;

		return 0;
	}
	return -1;			
}


