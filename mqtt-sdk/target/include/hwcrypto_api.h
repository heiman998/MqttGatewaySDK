/*
 *
 * Copyright  2013 Qualcomm Atheros, Inc.  All Rights Reserved.
 *
 * Qualcomm Atheros Confidential and Proprietary.
 */
#ifndef __HWCRYPTO_API_H
#define __HWCRYPTO_API_H
#include "hwcrypto.h"
#include "hwcrypto_ctrl.h"

#define HW_CRYPTO5_ENABLED 1

uint32_t _hw_crypto_handler(uint32_t sessionNum, uint32_t command1, char *algo1, char *mode1, uint32_t command2, char *algo2, char *mode2);                  
uint32_t _hw_crypto_one_session_process( enum crypto_buf_req_type op, char *algo, char *mode);                              
int  _crypto_module_init(void);
void _crypto_bam_init(uint32_t bam_iobase);
void _crypto_engine_init(void);
crypto_status_t _crypto_pipe_init(struct crypto_ctrl_eng *eng, uint32_t idx, struct crypto_desc **desc_paddr);
void _crypto_ctrl_init(void);
crypto_status_t _crypto_idx_init(struct crypto_ctrl_eng *eng);
int _crypto_bam_pipe_init(struct crypto_ctrl_eng *ctrl, uint32_t pipe);
void _crypto_setup_cmd_config(struct crypto_cmd_config *cfg, uint32_t base_addr, uint16_t pp_num,
					struct crypto_encr_cfg *encr, struct crypto_auth_cfg *auth);
void _crypto_setup_cmd_request(struct crypto_cmd_request *req, uint32_t base_addr, uint16_t pp_num);
void _crypto_write_ckey_regs(uint8_t *base, uint16_t pp_num, struct crypto_encr_cfg *encr);
void _crypto_write_akey_regs(uint8_t *base, uint16_t pp_num, struct crypto_auth_cfg *auth);
crypto_status_t _crypto_update_hash_algo(struct crypto_buf *buf, struct crypto_ctrl_eng *eng, struct crypto_cmd_config *cfg);
void _crypto_cblk_partial_auth_update(struct crypto_buf *buf, uint32_t base_addr, struct crypto_cmd_config *cfg, struct crypto_res_dump *result);
void _crypto_cblk_key_update(struct crypto_buf *buf, uint32_t base_addr, struct crypto_cmd_config *cfg);
int32_t _crypto_check_op_done(uint32_t sessionIdx, uint16_t pp_num, uint32_t pidx, struct crypto_desc *hw_desc);
crypto_status_t _crypto_validate_cipher(struct crypto_key *cipher, struct crypto_encr_cfg *encr_cfg);
crypto_status_t _crypto_validate_auth(struct crypto_key *auth, struct crypto_auth_cfg *auth_cfg);
void _crypto_key_update(struct crypto_ctrl_eng *eng, uint32_t idx, struct crypto_encr_cfg *encr_cfg,
					struct crypto_auth_cfg *auth_cfg);
void _crypto_cblk_update(struct crypto_ctrl_eng *eng, struct crypto_cmd_block *cblk, struct crypto_buf *buf);
void _crypto_session_update(struct crypto_ctrl *ctrl, struct crypto_buf *buf);
void _crypto_save_result_dump(struct crypto_buf *buf);      
void _crypto_enqueue_hw(struct crypto_ctrl_eng *eng, struct crypto_ctrl_idx *ses_idx, struct crypto_buf *buf);
uint32_t _crypto_enqueue_pbuf(struct crypto_buf *buf);
crypto_status_t _crypto_session_alloc(struct crypto_key *cipher, struct crypto_key *auth, uint32_t *session_idx);
crypto_status_t _crypto_session_free(uint32_t session_idx);
crypto_status_t _crypto_transform_payload(struct crypto_buf *buf);  
void _crypto_debug_config_print(struct crypto_buf *buf, struct crypto_cmd_config *cfg, struct crypto_cmd_request *req, struct crypto_in_trans *in,  struct crypto_out_trans *out);
                 
#define  hw_crypto_handler(sessionNum, command1, algo1, mode1, command2, algo2, mode2) \
         HWCRYPTO_FN(_hw_crypto_handler((sessionNum), (command1), (algo1), (mode1), (command2), (algo2), (mode2)))
         
#define hw_crypto_one_session_process(op, algo, mode)  \
         HWCRYPTO_FN(_hw_crypto_one_session_process((op), (algo), (mode)))     
                
#define  crypto_module_init() \
         HWCRYPTO_FN(_crypto_module_init())

#define  crypto_bam_init(bam_iobase) \
         HWCRYPTO_FN(_crypto_bam_init((bam_iobase)))

#define  crypto_engine_init() \
         HWCRYPTO_FN(_crypto_engine_init())

#define  crypto_pipe_init(eng, idx, desc_paddr) \
         HWCRYPTO_FN(_crypto_pipe_init((eng), (idx), (desc_paddr)))

#define  crypto_ctrl_init() \
         HWCRYPTO_FN(_crypto_ctrl_init())

#define crypto_idx_init(eng)  \
        HWCRYPTO_FN(_crypto_idx_init((eng)))
        
#define  crypto_bam_pipe_init(ctrl, pipe) \
         HWCRYPTO_FN(_crypto_bam_pipe_init((ctrl), (pipe)))

#define  crypto_setup_cmd_config(cfg, base_addr, pp_num,encr, auth) \
         HWCRYPTO_FN(_crypto_setup_cmd_config((cfg), (base_addr), (pp_num),(encr), (auth)))

#define  crypto_setup_cmd_request(req, base_addr, pp_num) \
         HWCRYPTO_FN(_crypto_setup_cmd_request((req), (base_addr), (pp_num)))

#define crypto_write_ckey_regs(base, pp_num, encr)  \
         HWCRYPTO_FN(_crypto_write_ckey_regs((base), (pp_num), (encr)))        

#define crypto_write_akey_regs(base, pp_num, auth)  \
          HWCRYPTO_FN(_crypto_write_akey_regs((base), (pp_num), (auth))) 

#define crypto_update_hash_algo(buf, eng, cfg)  \
          HWCRYPTO_FN(_crypto_update_hash_algo((buf), (eng), (cfg))) 
          
#define crypto_cblk_partial_auth_update(buf, base_addr, cfg, result)  \
          HWCRYPTO_FN(_crypto_cblk_partial_auth_update((buf), (base_addr), (cfg), (result))) 

#define crypto_cblk_key_update(buf, base_addr, cfg)  \
          HWCRYPTO_FN(_crypto_cblk_key_update((buf), (base_addr), (cfg))) 

#define crypto_check_op_done(sessionIdx, pp_num, pidx, hw_desc)  \
          HWCRYPTO_FN(_crypto_check_op_done((sessionIdx), (pp_num), (pidx), (hw_desc))) 

#define  crypto_validate_cipher(cipher, encr_cfg) \
         HWCRYPTO_FN(_crypto_validate_cipher((cipher), (encr_cfg)))

#define  crypto_validate_auth(auth, auth_cfg) \
         HWCRYPTO_FN(_crypto_validate_auth((auth), (auth_cfg)))

#define  crypto_key_update(eng, idx, encr_cfg, auth_cfg) \
         HWCRYPTO_FN(_crypto_key_update((eng), (idx), (encr_cfg), (auth_cfg)))

#define  crypto_cblk_update(eng, cblk, buf) \
         HWCRYPTO_FN(_crypto_cblk_update((eng), (cblk), (buf)))

#define  crypto_session_update(ctrl, buf) \
         HWCRYPTO_FN(_crypto_session_update((ctrl), (buf)))

#define  crypto_save_result_dump(buf) \
         HWCRYPTO_FN(_crypto_save_result_dump((buf)))

#define  crypto_enqueue_hw(eng,ses_idx, buf) \
         HWCRYPTO_FN(_crypto_enqueue_hw((eng),(ses_idx), (buf)))

#define  crypto_enqueue_pbuf(buf) \
         HWCRYPTO_FN(_crypto_enqueue_pbuf((buf)))

#define  crypto_session_alloc(cipher, auth, session_idx) \
         HWCRYPTO_FN(_crypto_session_alloc((cipher), (auth), (session_idx)))

#define  crypto_session_free(session_idx) \
         HWCRYPTO_FN(_crypto_session_free((session_idx)))

#define  crypto_transform_payload(buf) \
         HWCRYPTO_FN(_crypto_transform_payload((buf)))

#define  crypto_debug_config_print(buf, cfg, req, in, out) \
         HWCRYPTO_FN(_crypto_debug_config_print((buf), (cfg), (req), (in), (out)))

typedef struct hwCrypto_api {
        uint32_t (*_hw_crypto_handler)(uint32_t sessionNum, uint32_t command1, char *algo1, char *mode1, uint32_t command2, char *algo2, char *mode2);
        uint32_t (*_hw_crypto_one_session_process)( enum crypto_buf_req_type op, char *algo, char *mode);        
        int  (*_crypto_module_init) (void);
        void (*_crypto_bam_init) (uint32_t bam_iobase);
        void (*_crypto_engine_init) (void);
        crypto_status_t (*_crypto_pipe_init) (struct crypto_ctrl_eng *eng, uint32_t idx, struct crypto_desc **desc_paddr);
        void (*_crypto_ctrl_init) (void);
        crypto_status_t (*_crypto_idx_init) (struct crypto_ctrl_eng *eng);
        int (*_crypto_bam_pipe_init) (struct crypto_ctrl_eng *ctrl, uint32_t pipe);
        void (*_crypto_setup_cmd_config) (struct crypto_cmd_config *cfg, uint32_t base_addr, uint16_t pp_num,
					struct crypto_encr_cfg *encr, struct crypto_auth_cfg *auth);
        void (*_crypto_setup_cmd_request) (struct crypto_cmd_request *req, uint32_t base_addr, uint16_t pp_num);
        void (*_crypto_write_ckey_regs)(uint8_t *base, uint16_t pp_num, struct crypto_encr_cfg *encr);
        void (*_crypto_write_akey_regs)(uint8_t *base, uint16_t pp_num, struct crypto_auth_cfg *auth);
        crypto_status_t (*_crypto_update_hash_algo)(struct crypto_buf *buf, struct crypto_ctrl_eng *eng, struct crypto_cmd_config *cfg);
        void (*_crypto_cblk_partial_auth_update)(struct crypto_buf *buf, uint32_t base_addr, struct crypto_cmd_config *cfg, struct crypto_res_dump *result);
        void (*_crypto_cblk_key_update)(struct crypto_buf *buf, uint32_t base_addr, struct crypto_cmd_config *cfg);
        int32_t (*_crypto_check_op_done)(uint32_t sessionIdx, uint16_t pp_num, uint32_t pidx, struct crypto_desc *hw_desc);
        crypto_status_t (*_crypto_validate_cipher) (struct crypto_key *cipher, struct crypto_encr_cfg *encr_cfg);
        crypto_status_t (*_crypto_validate_auth) (struct crypto_key *auth, struct crypto_auth_cfg *auth_cfg);
        void (*_crypto_key_update) (struct crypto_ctrl_eng *eng, uint32_t idx, struct crypto_encr_cfg *encr_cfg,
					struct crypto_auth_cfg *auth_cfg);
        void (*_crypto_cblk_update)(struct crypto_ctrl_eng *eng, struct crypto_cmd_block *cblk, struct crypto_buf *buf);
        void (*_crypto_session_update)(struct crypto_ctrl *ctrl, struct crypto_buf *buf);
        void (*_crypto_save_result_dump)(struct crypto_buf *buf);      
        void (*_crypto_enqueue_hw)(struct crypto_ctrl_eng *eng, struct crypto_ctrl_idx *ses_idx, struct crypto_buf *buf);
        uint32_t (*_crypto_enqueue_pbuf)(struct crypto_buf *buf);
        crypto_status_t (*_crypto_session_alloc)(struct crypto_key *cipher, struct crypto_key *auth, uint32_t *session_idx);
        crypto_status_t (*_crypto_session_free)(uint32_t session_idx);
        crypto_status_t (*_crypto_transform_payload)(struct crypto_buf *buf); 
        void (*_crypto_debug_config_print)(struct crypto_buf *buf, struct crypto_cmd_config *cfg, struct crypto_cmd_request *req, struct crypto_in_trans *in,  struct crypto_out_trans *out);
        
}HWCRYPTO_API;

extern HWCRYPTO_API hwCrypto_api;
#define HWCRYPTO_FN(fn) hwCrypto_api.fn
#endif /* __CRYPTO_IF_H */
