
#ifndef _LTM3210_H_
#define _LTM3210_H_


#include "qcom/qcom_common.h"
#include "qcom/qcom_internal.h"
#include "qcom/qcom_gpio.h"
#include "qcom/socket.h"
#include "qcom/select_api.h"
#include "qcom/socket_api.h"
#include "qcom/qcom_ir_ctrl.h"
#include "qcom_ssl.h"
#include "qcom_spi.h"
#include "qcom_utils.h"
#include "qcom/basetypes.h"
#include "qcom_system.h"
#include "qcom_timer.h"
#include "string.h"
#include "base.h"
#include "qcom/qcom_nvram.h"
#include "qcom_ir_ctrl.h"
#include "qcom_uart.h"

/**
* @brief: device id
*/
#ifndef DEVICE_ID
#define DEVICE_ID	0
#endif


/**
* @brief: printf enable
*/
#define PRINT						A_PRINTF
#define PRINT_HEX					print_hex

#define PRINT_CONNT					A_PRINTF
#define PRINT_CONNT_H(fmt,args...)	{A_PRINTF("[connt] %d ", __LINE__); A_PRINTF(fmt,##args);}

#define PRINT_UDPS					A_PRINTF
#define PRINT_UDPS_H(fmt,args...)	{A_PRINTF("[upds] %d ", __LINE__); A_PRINTF(fmt,##args);}

#define PRINT_TCPC					A_PRINTF
#define PRINT_TCPC_H(fmt,args...)	{A_PRINTF("[tcpc] %d ", __LINE__); A_PRINTF(fmt,##args);}

#define PRINT_TCPS					A_PRINTF
#define PRINT_TCPS_H(fmt,args...)	{A_PRINTF("[tcps] %d ", __LINE__); A_PRINTF(fmt,##args);}

#define PRINT_HAD					A_PRINTF
#define PRINT_HAD_H(fmt,args...)	{A_PRINTF("[tcps] %d ", __LINE__); A_PRINTF(fmt,##args);}

#define PRINT_UART					A_PRINTF
#define PRINT_UART_H(fmt,args...)	{A_PRINTF("[uart] %d ", __LINE__); A_PRINTF(fmt,##args);}

#define PRINT_INFRA					A_PRINTF
#define PRINT_INFRA_H(fmt,args...)	{A_PRINTF("[infra] %d ", __LINE__); A_PRINTF(fmt,##args);}


#endif

