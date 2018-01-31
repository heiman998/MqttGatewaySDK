/**
 * @file
 */
/******************************************************************************
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#ifndef __QCOM_NVRAM_H__
#define __QCOM_NVRAM_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LIBCUST_INDIRECT_ACCESS

    typedef struct  {

        int	(* _qcom_board_type_get)(void);
        int	(* _qcom_board_cfg_get)(void);
        A_INT32	(* _qcom_nvram_mode_get)(void);
        A_INT32	(* _qcom_nvram_size_get)();
        void(* _qcom_nvram_init)(void);

        void(* _qcom_nvram_fini)(void);
        A_STATUS (* _qcom_nvram_read)(A_UINT32 offset, A_UCHAR *buf, A_UINT32 size);
        A_STATUS (* _qcom_nvram_erase)(A_UINT32 offset, A_UINT32 size);
        A_STATUS (* _qcom_nvram_write)(A_UINT32 offset, A_UCHAR  *buf, A_UINT32 size);

        A_INT32 (* _qcom_nvram_select)(A_INT32 partition);
        A_INT32 (* _qcom_nvram_read_partition_word)(int partition, unsigned int offset, A_UINT32 *buf);
        A_INT32 (* _qcom_nvram_read_partition_age)(int partition,A_UINT32 *buf);
    }QCOM_NVRAM_API_INDIRECTION_TABLE;

    extern QCOM_NVRAM_API_INDIRECTION_TABLE qcomNvramApiIndirectionTable;
#define QCOM_NVRAM_API_FN(fn) qcomNvramApiIndirectionTable.fn


#else

    int _qcom_board_type_get(void);
    int _qcom_board_cfg_get(void);
    A_INT32 _qcom_nvram_mode_get(void);
    A_INT32 _qcom_nvram_size_get();
    void _qcom_nvram_init(void);

    void _qcom_nvram_fini(void);
    A_STATUS _qcom_nvram_read(A_UINT32 offset, A_UCHAR *buf, A_UINT32 size);
    A_STATUS _qcom_nvram_erase(A_UINT32 offset, A_UINT32 size);
    A_STATUS _qcom_nvram_write(A_UINT32 offset, A_UCHAR  *buf, A_UINT32 size);

    A_INT32 _qcom_nvram_select(A_INT32 partition);
    A_INT32 _qcom_nvram_read_partition_word(int partition, unsigned int offset, A_UINT32 *buf);
    A_INT32 _qcom_nvram_read_partition_age(int partition,A_UINT32 *buf);

#define QCOM_NVRAM_API_FN(fn) fn


#endif




#define qcom_board_type_get() \
    QCOM_NVRAM_API_FN(_qcom_board_type_get())
#define qcom_board_cfg_get() \
    QCOM_NVRAM_API_FN(_qcom_board_cfg_get())
#define qcom_nvram_mode_get() \
    QCOM_NVRAM_API_FN(_qcom_nvram_mode_get())
#define qcom_nvram_size_get() \
    QCOM_NVRAM_API_FN(_qcom_nvram_size_get())
#define qcom_nvram_init() \
    QCOM_NVRAM_API_FN(_qcom_nvram_init())


#define qcom_nvram_fini() \
    QCOM_NVRAM_API_FN(_qcom_nvram_fini())
#define qcom_nvram_read(off, buf, size) \
    QCOM_NVRAM_API_FN(_qcom_nvram_read((off), (buf), (size)))
#define qcom_nvram_erase(off, size) \
    QCOM_NVRAM_API_FN(_qcom_nvram_erase((off), (size)))
#define qcom_nvram_write(off, buf, size) \
    QCOM_NVRAM_API_FN(_qcom_nvram_write((off), (buf), (size)))


#define qcom_nvram_select(part) \
    QCOM_NVRAM_API_FN(_qcom_nvram_select(part))
#define qcom_nvram_read_partition_word(part, off, buf) \
    QCOM_NVRAM_API_FN(_qcom_nvram_read_partition_word((part), (off), (buf)))
#define qcom_nvram_read_partition_age(part, buf) \
    QCOM_NVRAM_API_FN(_qcom_nvram_read_partition_age((part), (buf)))

#ifdef __cplusplus
}
#endif

#endif
