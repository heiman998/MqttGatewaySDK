/**
 * @file
 */
/******************************************************************************
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$

 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the license is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the license for the specific language governing permissions and
 *    limitations under the license.
 ******************************************************************************/
#ifndef __CUST_DSET_H__
#define __CUST_DSET_H__

#include "dset_api.h"

typedef A_UINT32 *PDSET_HANDLE;
typedef A_UINT32 DSET_HANDLE;
typedef A_UINT32 DSET_ID;
typedef void (*DSET_CB)(A_STATUS status, void *callback_arg);

#ifdef LIBCUST_INDIRECT_ACCESS

typedef struct  {
/* qcom_dset_create - create one dset on indicated media. */
/**
 * This function is used to create one dset on indicated media.
 *      
 * @param pdset_handle          dset handle pointer, if success, it has dset handle
 * @param dset_id               dset id
 * @param length                dset length
 * @param flags                 circlar dset flag + media ID
 * @param create_cb             asyn operation callback function
 * @param cb_arg                callback function's parameter
 *
 * @return                      A_OK
 *                              A_FAIL
 *                              A_PENDING
 */
    A_STATUS  (*_qcom_dset_create)(PDSET_HANDLE pdset_handle, DSET_ID dset_id, A_UINT32 length,
                                         A_UINT32 flags, DSET_CB create_cb, void *cb_arg);

/* qcom_dset_open - open one dset for access. */
/**
 * This function is used to open one dset for access.
 *      
 * @param pdset_handle          dset handle pointer, if success, it has dset handle
 * @param dset_id               dset id
 * @param flags                 only support circlar flag     
 * @param open_cb               asyn operation callback function
 * @param cb_arg                callback function's parameter
 *
 * @return                      A_OK
 *                              A_FAIL
 *                              A_PENDING
 */
    A_STATUS (*_qcom_dset_open)(PDSET_HANDLE pdset_handle, DSET_ID dset_id, A_UINT32 flags, 
                                                                      DSET_CB open_cb, void *cb_arg);

/* qcom_dset_read - read dset data into buffer. */
/**
 * This function is used to read dset data into buffer.
 *      
 * @param dset_handle           dset handle
 * @param buffer                data buffer that is written
 * @param length                data length
 * @param offset                offset in dset
 * @param read_cb               asyn operation callback function
 * @param cb_arg                callback function's parameter 
 *
 * @return                      A_OK
 *                              A_FAIL
 *                              A_PENDING
 */
    A_STATUS (*_qcom_dset_read)(DSET_HANDLE dset_handle, A_UINT8 *buffer, 
                                        A_UINT32 len, A_UINT32 offset, DSET_CB read_cb, void *cb_arg);

/* qcom_dset_write - write data into one dset. */
/**
 * This function is used to write data into one.
 *      
 * @param dset_handle           dset handle
 * @param buffer                data buffer that is written
 * @param length                data length
 * @param offset                offset in dset
 * @param flags                 only support circlar flag
 * @param open_cb               asyn operation callback function
 * @param cb_arg                callback function's parameter 
 *
 * @return                      A_OK
 *                              A_FAIL
 *                              A_PENDING
 */
    A_STATUS (*_qcom_dset_write)(DSET_HANDLE dset_handle, A_UINT8 *buffer, A_UINT32 len, A_UINT32 offset,
                                                     A_UINT32 flags, DSET_CB write_cb, void *cb_arg);

/* qcom_dset_close - close opened dset. */
/**
 * This function is used to close opened dset.
 *      
 * @param dset_handle           dset handle
 * @param close_cb               asyn operation callback function
 * @param cb_arg                callback function's parameter 
 *
 * @return                      A_OK
 *                              A_FAIL
 *                              A_PENDING
 */
    A_STATUS  (*_qcom_dset_close)(DSET_HANDLE dset_handle, DSET_CB close_cb, void *arg_cb);

/* qcom_dset_commit - commit changed dset. */
/**
 * This function is used to write cached dset data into media.
 *      
 * @param dset_handle           dset handle
 * @param commit_cb              asyn operation callback function
 * @param cb_arg                callback function's parameter 
 *
 * @return                      A_OK
 *                              A_FAIL
 *                              A_PENDING
 */
    A_STATUS  (*_qcom_dset_commit)(DSET_HANDLE dset_handle, DSET_CB commit_cb, void *arg_cb);

/* qcom_dset_delete - delete dset from media. */
/**
 * This function is used to dset from media.
 *      
 * @param dset_id               dset id
 * @param delete_cb             asyn operation callback function
 * @param cb_arg                callback function's parameter 
 *
 * @return                      A_OK
 *                              A_FAIL
 *                              A_PENDING
 */
    A_STATUS  (*_qcom_dset_delete)(DSET_ID dset_id, A_UINT32 flags, DSET_CB delete_cb, void *arg_cb);

/* qcom_dset_size - reture dset size. */
/**
 * This function is used to get dset size.
 *      
 * @param dset_handle           dset handle
 *
 * @return                      dset size
 */
    A_UINT32  (*_qcom_dset_size)(DSET_HANDLE dset_handle);

/* qcom_dset_media - return dset media that resident on. */
/**
 * This function is used to return dset media.
 *      
 * @param dset_handle           dset handle
 *
 * @return                      media, return value with DSET_FLAG_MEDIA_MASK
 */
    A_UINT32 (* _qcom_dset_media)(A_UINT32 dset_handle);

} QCOM_DSET_API_INDIRECTION_TABLE;

extern QCOM_DSET_API_INDIRECTION_TABLE qcomDsetApiIndirectionTable;

#define QCOM_DSET_API_FN(fn) qcomDsetApiIndirectionTable.fn

#else

A_STATUS  _qcom_dset_create(PDSET_HANDLE pdset_handle, DSET_ID dset_id, A_UINT32 length,
                                         A_UINT32 flags, DSET_CB create_cb, void *cb_arg);

A_STATUS _qcom_dset_open(PDSET_HANDLE pdset_handle, DSET_ID dset_id, A_UINT32 flags, 
                                                                      DSET_CB open_cb, void *cb_arg);

A_STATUS _qcom_dset_read(DSET_HANDLE dset_handle, A_UINT8 *buffer, 
                                        A_UINT32 len, A_UINT32 offset, DSET_CB read_cb, void *cb_arg);

A_STATUS _qcom_dset_write(DSET_HANDLE dset_handle, A_UINT8 *buffer, A_UINT32 len, A_UINT32 offset,
                                                     A_UINT32 flags, DSET_CB write_cb, void *cb_arg);

A_STATUS _qcom_dset_close(DSET_HANDLE dset_handle, DSET_CB close_cb, void *arg_cb);

A_STATUS _qcom_dset_commit(DSET_HANDLE dset_handle, DSET_CB commit_cb, void *arg_cb);

A_STATUS _qcom_dset_delete(DSET_ID dset_id, A_UINT32 flags, DSET_CB delete_cb, void *arg_cb);

A_UINT32 _qcom_dset_size(DSET_HANDLE dset_handle);

A_UINT32 _qcom_dset_media(A_UINT32 dset_handle);

#define QCOM_DSET_API_FN(fn) fn

#endif// DISABLE_FUNCTION_INDIRECTION

#define qcom_dset_create(p, i, l, f, cb, arg) \
          QCOM_DSET_API_FN(_qcom_dset_create) ((p),(i),(l),(f),(cb),(arg))

#define qcom_dset_open(p, i, f, cb, arg) \
          QCOM_DSET_API_FN(_qcom_dset_open)((p), (i), (f), (cb), (arg))

#define qcom_dset_read(h, b, l, off, cb, arg) \
          QCOM_DSET_API_FN(_qcom_dset_read)((h), (b), (l), (off), (cb), (arg))

#define qcom_dset_write(h, b, l, off, f, cb, arg) \
          QCOM_DSET_API_FN(_qcom_dset_write)((h), (b), (l), (off), (f), (cb), (arg))

#define qcom_dset_close(h, cb, arg) \
          QCOM_DSET_API_FN(_qcom_dset_close)((h), (cb), (arg))

#define qcom_dset_commit(h, cb, arg) \
          QCOM_DSET_API_FN(_qcom_dset_commit)((h), (cb), (arg))

#define qcom_dset_delete(id, f, cb, arg) \
          QCOM_DSET_API_FN(_qcom_dset_delete)((id), (f), (cb), (arg))

#define qcom_dset_size(h) \
          QCOM_DSET_API_FN(_qcom_dset_size)((h))

#define qcom_dset_media(h) \
          QCOM_DSET_API_FN(_qcom_dset_media)((h))

#endif
