/* Copyright (c) Qualcomm Atheros, Inc.
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. No reverse engineering, decompilation, or disassembly of this software is permitted.
 * 
 * 2. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * 3. Redistributions in binary form must reproduce the above copyright
 * notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 * 
 * 4. Neither the name of Qualcomm-Atheros Inc. nor the names of other
 * contributors to this software may be used to endorse or promote products
 * derived from this software without specific prior written permission.
 * 
 * 5. This software must only be used in a product manufactured by
 * Qualcomm-Atheros Inc, or in a product manufactured
 * by a third party that is used in combination with a product manufactured by
 * Qualcomm-Atheros Inc.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. NO LICENSES OR OTHER RIGHTS, WHETHER EXPRESS, IMPLIED, BASED ON
 * ESTOPPEL OR OTHERWISE, ARE GRANTED TO ANY PARTY'S PATENTS, PATENT 
 * APPLICATIONS, OR PATENTABLE INVENTIONS BY VIRTUE OF THIS LICENSE OR THE
 * DELIVERY OR PROVISION BY QUALCOMM ATHEROS, INC. OF THE SOFTWARE IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON * ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
#include "phost.h"
#include <cdr_api.h>
#include "qcom/cdr_bgr.h"
#include "qcom_common.h"
#include "crypto_internal.h"

void
cdr_crypto_completed(void *arg, struct cdr_req_s *cdr_req)
{
   cdr_crypto_completed_t *completed = (cdr_crypto_completed_t*) arg;
   struct A_RTOS_COND *std_done_cond = (struct A_RTOS_COND *)completed->event;

   if (completed->completed == 1)
       A_RTOS_COND_SIGNAL(std_done_cond);
}

void
BGR_crypto_CDR_initiate(void *request, unsigned int req_length)
{
    struct cdr_req_s cdr;
    struct A_RTOS_COND *std_done_cond;
    cdr_crypto_completed_t completed;

    A_RTOS_COND_INIT(&std_done_cond);

    cdr.req_block = request;
    cdr.req_sz = req_length;
    cdr.req_flags = 0;

    cdr.req_completed = cdr_crypto_completed;

    completed.completed = 0;
    completed.event = std_done_cond;

    cdr.completed_arg = &completed;

    cdr_initiate(cdr_bgr_handler, &cdr);
    A_RTOS_COND_WAIT(std_done_cond);
}

A_CRYPTO_STATUS qcom_crypto_obj_info_get(qcom_crypto_obj_hdl_t hdl, qcom_crypto_obj_info_t *info)
{
    cdr_crypto_obj_info_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OBJ_INFO_REQ;
    req.hdl = hdl;
    req.info = info;
    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_info_get(qcom_crypto_obj_hdl_t hdl, qcom_crypto_op_info_t *info)
{
    cdr_crypto_op_info_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_INFO_REQ;
    req.hdl = hdl;
    req.info = info;
    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_transient_obj_reset(qcom_crypto_obj_hdl_t hdl)
{
    cdr_crypto_obj_reset_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_TRANS_OBJ_RESET_REQ;
    req.hdl = hdl;
    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_obj_usage_restrict(qcom_crypto_obj_hdl_t hdl, A_UINT32 usage)
{
    cdr_crypto_obj_usage_restrict_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OBJ_USAGE_RESTRICT_REQ;
    req.hdl = hdl;
    req.usage = usage;
    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_transient_obj_alloc(A_UINT32 obj_type, A_UINT32 max_key_size, qcom_crypto_obj_hdl_t *hdl)
{
    cdr_crypto_trans_obj_alloc_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_TRANS_OBJ_ALLOC_REQ;
    req.obj_type = obj_type;
    req.max_key_size = max_key_size;
    BGR_crypto_CDR_initiate(&req, sizeof(req));

    if (hdl) {
        *hdl = req.hdl;
    }
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_transient_obj_free(qcom_crypto_obj_hdl_t hdl)
{
    cdr_crypto_trans_obj_free_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_TRANS_OBJ_FREE_REQ;
    req.hdl = hdl;
    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_transient_obj_keygen(qcom_crypto_obj_hdl_t hdl, A_UINT32 key_size, qcom_crypto_attrib_t *attrs, A_UINT32 attr_count)
{
    cdr_crypto_trans_obj_keygen_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_TRANS_OBJ_KEYGEN_REQ;
    req.hdl = hdl;
    req.key_size = key_size;
    req.attrs = attrs;
    req.attr_count = attr_count;
    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_transient_obj_populate(qcom_crypto_obj_hdl_t hdl, qcom_crypto_attrib_t *attrs, A_UINT32 attr_count)
{
    cdr_crypto_trans_obj_keygen_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_TRANS_OBJ_POPULATE_REQ;
    req.hdl = hdl;
    req.attrs = attrs;
    req.attr_count = attr_count;
    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_obj_buf_attrib_get(qcom_crypto_obj_hdl_t hdl, A_UINT32 attrib_id, void *buffer, A_UINT32 size)
{
    cdr_crypto_obj_buf_attrib_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OBJ_BUF_ATTRIB_REQ;
    req.hdl = hdl;
    req.attrib_id = attrib_id;
    req.buffer = buffer;
    req.size = size;
    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_obj_val_attrib_get(qcom_crypto_obj_hdl_t hdl, A_UINT32 attrib_id, A_UINT32 *a, A_UINT32 *b)
{
    cdr_crypto_obj_val_attrib_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OBJ_BUF_ATTRIB_REQ;
    req.hdl = hdl;
    req.attrib_id = attrib_id;
    req.a = a;
    req.b = b;
    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_rng_get(void *buffer, A_UINT16 len)
{
    cdr_crypto_obj_rng_get_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OBJ_RNG_GET_REQ;
    req.buffer = buffer;
    req.len = len;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}
