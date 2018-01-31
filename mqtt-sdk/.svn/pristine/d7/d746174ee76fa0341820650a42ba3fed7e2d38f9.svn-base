/**
 * @file
 */
/******************************************************************************
 * Copyright (c) 2015 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$

 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the license is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the license for the specific language governing permissions and
 *    limitations under the license.
 ******************************************************************************/


#ifndef __QCOM_IR_CTRL_H__
#define __QCOM_IR_CTRL_H__

/*   ts to save internal timestamp, length is the copy size and it will use the minmum one
**	from the user's length and real effective timestamp array length. It will close ISR INT and
**   restore it after copy the data.
**/

/*The following defines callback function bottom behavior, CB_ISR_CONTINUE means
   after callback is executed, the other half (DSR) will be executed. CB_ISR_COMPLETE
   means only this callback fucntion will be called. DSR will not be ran.
*/
#define CB_ISR_CONTINUE	0
#define CB_ISR_COMPLETE	1
#define INSTALL_RET_OK	0
#define INSTALL_RET_ERR	1

/* This is for callback fun register use. when users want to run cb function in a higher prioriy
    they should firstly register it by qcom_isr_handler_install(), then if normal interrupt function
    would be used, qcom_gpio_interrupt_register should also be registered.
*/
typedef struct _qcom_gpio_isr_info_t {
    A_UINT32    pin;
    A_UINT32    (*gpio_isr_handler_fn)(void *arg);
    void        *arg;
} qcom_gpio_isr_info_t;

A_UINT64 qcom_time_us();

A_UINT32 qcom_get_ir_intr_ts(A_UINT32 *ts, A_UINT32 length);

void qcom_config_ir(A_UINT32 pin);
void qcom_ir_ctrl_install(void);
/* return value: 0 = OK, 1 = error */
A_UINT32 qcom_isr_handler_install(qcom_gpio_isr_info_t *gpio_isr);

/* pin: choose GPIO pins as output control
*   us: delay microsecond for output low. 
*   ms: pull GPIO as output high for millisecond. minimum is 2 ms.
*/
A_UINT32 qcom_cust_speed_ctrl(A_UINT32 pin, A_UINT32 us, A_UINT32 ms);

void qcom_hf_timer_uninstall(void);
void qcom_hf_timer_install(void);

/* It will clear ISR handler and back to DSR */
void qcom_ir_ctrl_uninstall(void);


#endif

