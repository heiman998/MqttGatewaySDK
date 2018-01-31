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

A_CRYPTO_STATUS qcom_crypto_op_alloc(A_UINT32 alg, A_UINT32 mode, A_UINT32 max_key_size, 
                                    qcom_crypto_op_hdl_t *hdl)
{
    cdr_crypto_op_alloc_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_ALLOC_REQ;
    req.alg = alg;
    req.mode = mode;
    req.max_key_size = max_key_size;
    req.hdl = hdl;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_free(qcom_crypto_op_hdl_t hdl)
{
    cdr_crypto_op_free_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_FREE_REQ;
    req.hdl = hdl;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_reset(qcom_crypto_op_hdl_t hdl)
{
    cdr_crypto_op_reset_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_RESET_REQ;
    req.hdl = hdl;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_key_set(qcom_crypto_op_hdl_t op_hdl, qcom_crypto_obj_hdl_t obj_hdl)
{
    cdr_crypto_op_key_set_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_KEY_SET_REQ;
    req.op_hdl = op_hdl;
    req.obj_hdl = obj_hdl;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_enable_hw_key(qcom_crypto_op_hdl_t hdl, QCOM_CRYPTO_HW_KEY hw_key)
{
    cdr_crypto_op_enable_hw_key_req_t req;

	if (hw_key != QCOM_CRYPTO_HW_KEY_QC && hw_key != QCOM_CRYPTO_HW_KEY_QC)
	{
		return A_CRYPTO_ERROR;
	}
	
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_ENABLE_HW_KEY_REQ;
    req.op_hdl = hdl;
	req.hw_key = hw_key;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_copy(qcom_crypto_op_hdl_t dst_hdl, qcom_crypto_op_hdl_t src_hdl)
{
    cdr_crypto_op_copy_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_COPY_REQ;
    req.src_hdl = src_hdl;
    req.dst_hdl = dst_hdl;
    
    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_key_derive(qcom_crypto_op_hdl_t op_hdl, qcom_crypto_attrib_t *params, 
        A_UINT32 param_count, qcom_crypto_obj_hdl_t obj_hdl)
{
    cdr_crypto_op_key_derive_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_KEY_DERIVE_REQ;
    req.op_hdl = op_hdl;
    req.attrs = params;
    req.attr_count = param_count;
    req.obj_hdl = obj_hdl;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}


A_CRYPTO_STATUS qcom_crypto_op_verify_digest(qcom_crypto_op_hdl_t hdl, qcom_crypto_attrib_t *params, A_UINT32 param_count,
                    void *digest, A_UINT32 digest_len, void *sign,  A_UINT32 sign_len)
{
    cdr_crypto_op_verify_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_VERIFY_DIGEST_REQ;
    req.hdl = hdl;
    req.attrs = params;
    req.attr_count = param_count;
    req.digest = digest;
    req.digest_len = digest_len;
    req.sign = sign;
    req.sign_len = sign_len;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_sign_digest(qcom_crypto_op_hdl_t hdl, qcom_crypto_attrib_t *params, A_UINT32 param_count,
                    void *digest, A_UINT32 digest_len, void *sign,  A_UINT32 *sign_len)
{
    cdr_crypto_op_sign_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_SIGN_DIGEST_REQ;
    req.hdl = hdl;
    req.attrs = params;
    req.attr_count = param_count;
    req.digest = digest;
    req.digest_len = digest_len;
    req.sign = sign;
    req.sign_len = sign_len;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_cipher_init(qcom_crypto_op_hdl_t hdl, void *IV, A_UINT32 IVLen)
{
    cdr_crypto_op_cipher_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_CIPHER_INIT_REQ;
    req.hdl = hdl;
    req.IV= IV;
    req.IVLen= IVLen;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_cipher_update(qcom_crypto_op_hdl_t hdl, void *srcData, A_UINT32 srcLen, void *destData, A_UINT32 *destLen)
{
    cdr_crypto_op_cipher_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_CIPHER_UPDATE_REQ;
    req.hdl = hdl;
    req.srcData = srcData;
    req.srcLen = srcLen;
    req.destData = destData;
    req.destLen = destLen;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_cipher_dofinal(qcom_crypto_op_hdl_t hdl, void *srcData, A_UINT32 srcLen, void *destData, A_UINT32 *destLen)
{
    cdr_crypto_op_cipher_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_CIPHER_DOFINAL_REQ;
    req.hdl = hdl;
    req.srcData = srcData;
    req.srcLen = srcLen;
    req.destData = destData;
    req.destLen = destLen;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_ae_init(qcom_crypto_op_hdl_t hdl, void *nonce, A_UINT32 nonceLen, A_UINT32 tagLen, A_UINT32 AADLen, A_UINT32 payloadLen)
{
    cdr_crypto_op_ae_init_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_AE_INIT_REQ;
    req.hdl = hdl;
    req.nonce= nonce;
    req.nonceLen= nonceLen;
    req.tagLen= tagLen;
    req.AADLen= AADLen;
    req.payloadLen= payloadLen;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_ae_update_aad(qcom_crypto_op_hdl_t hdl, void *AAData, A_UINT32 AADataLen)
{
    cdr_crypto_op_ae_update_aad_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_AE_UPDATE_AAD_REQ;
    req.hdl = hdl;
    req.AAData= AAData;
    req.AADataLen= AADataLen;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_ae_update(qcom_crypto_op_hdl_t hdl, void *srcData, A_UINT32 srcLen, void *destData, A_UINT32 *destLen)
{
    cdr_crypto_op_ae_update_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_AE_UPDATE_REQ;
    req.hdl = hdl;
    req.srcData = srcData;
    req.srcLen = srcLen;
    req.destData = destData;
    req.destLen = destLen;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_ae_encrypt_final(qcom_crypto_op_hdl_t hdl, void *srcData, A_UINT32 srcLen, void *destData, A_UINT32 *destLen, void *tag, A_UINT32 *tagLen)
{
    cdr_crypto_op_ae_encrypt_final_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_AE_ENCRYPT_FINAL_REQ;
    req.hdl = hdl;
    req.srcData = srcData;
    req.srcLen = srcLen;
    req.destData = destData;
    req.destLen = destLen;
    req.tag = tag;
    req.taglen= tagLen;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_ae_decrypt_final(qcom_crypto_op_hdl_t hdl, void *srcData, A_UINT32 srcLen, void *destData, A_UINT32 *destLen, void *tag, A_UINT32 tagLen)
{
    cdr_crypto_op_ae_decrypt_final_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_AE_DECRYPT_FINAL_REQ;
    req.hdl = hdl;
    req.srcData = srcData;
    req.srcLen = srcLen;
    req.destData = destData;
    req.destLen = destLen;
    req.tag = tag;
    req.taglen= tagLen;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}


A_CRYPTO_STATUS qcom_crypto_op_mac_init(qcom_crypto_op_hdl_t hdl, void *IV, A_UINT32 IVLen)
{
    cdr_crypto_op_mac_init_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_MAC_INIT_REQ;
    req.hdl = hdl;
    req.IV= IV;
    req.IVLen= IVLen;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_mac_update(qcom_crypto_op_hdl_t hdl, void *chunk, A_UINT32 chunkSize)
{
    cdr_crypto_op_mac_update_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_MAC_UPDATE_REQ;
    req.hdl = hdl;
    req.chunk= chunk;
    req.chunkSize= chunkSize;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_mac_compute_final(qcom_crypto_op_hdl_t hdl, void *message, A_UINT32 messageLen, void *mac, A_UINT32 *macLen)
{
    cdr_crypto_op_mac_compute_final_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_MAC_COMPUTE_FINAL_REQ;
    req.hdl = hdl;
    req.message= message;
    req.messageLen= messageLen;
    req.mac = mac;
    req.macLen = macLen;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_mac_compare_final(qcom_crypto_op_hdl_t hdl, void *message, A_UINT32 messageLen, void *mac, A_UINT32 macLen)
{
    cdr_crypto_op_mac_compare_final_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_MAC_COMPARE_FINAL_REQ;
    req.hdl = hdl;
    req.message= message;
    req.messageLen= messageLen;
    req.mac = mac;
    req.macLen = macLen;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_digest_update(qcom_crypto_op_hdl_t hdl, void *chunk, A_UINT32 chunkSize)
{
    cdr_crypto_op_digest_update_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_DIGEST_UPDATE_REQ;
    req.hdl = hdl;
    req.chunk= chunk;
    req.chunkSize= chunkSize;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_digest_dofinal(qcom_crypto_op_hdl_t hdl, void *chunk, A_UINT32 chunkSize, void *hash, A_UINT32 *hashLen)
{
    cdr_crypto_op_digest_dofinal_req_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_DIGEST_DOFINAL_REQ;
    req.hdl = hdl;
    req.chunk= chunk;
    req.chunkSize= chunkSize;
    req.hash= hash;
    req.hashLen= hashLen;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_asym_encrypt (qcom_crypto_op_hdl_t hdl, qcom_crypto_attrib_t *params, A_UINT32 param_count,
                    void *srcData, A_UINT32 srcLen, void *destData,  A_UINT32* destLen)
{
    cdr_crypto_op_asym_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_ASYM_ENCRYPT_REQ;
    req.hdl = hdl;
    req.attrs = params;
    req.attr_count = param_count;
    req.srcData= srcData;
    req.srcLen= srcLen;
    req.destData= destData;
    req.destLen= destLen;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}

A_CRYPTO_STATUS qcom_crypto_op_asym_decrypt (qcom_crypto_op_hdl_t hdl, qcom_crypto_attrib_t *params, A_UINT32 param_count,
                    void *srcData, A_UINT32 srcLen, void *destData,  A_UINT32* destLen)
{
    cdr_crypto_op_asym_t req;
    req.hdr.cmd = CDR_CRYPTO_REQUEST;
    req.hdr.sub_cmd = CDR_CRYPTO_OP_ASYM_DECRYPT_REQ;
    req.hdl = hdl;
    req.attrs = params;
    req.attr_count = param_count;
    req.srcData= srcData;
    req.srcLen= srcLen;
    req.destData= destData;
    req.destLen= destLen;

    BGR_crypto_CDR_initiate(&req, sizeof(req));
    return req.status;
}


