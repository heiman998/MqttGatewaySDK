/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */
#ifndef __DSET_API_H__
#define __DSET_API_H__

#include "dsetid.h"

/*
 * DataSet support.
 *
 * A DataSet ("dset") is a collection of related data which is referenced
 * by the Target during operation.  Examples of potential DataSets:
 *   -Calibration/Board Data
 *   -Initial Noise Immunity Data
 *   -Regulatory Database
 *   -User Configuration settings
 *   -PHY settings
 *   -HTTP pages
 *
 * DataSets may contain data specific to a board design, data that is
 * specific to a particular instance of a board, data that is consumed
 * by the standard WLAN driver, data that is ODM-specific, data that 
 * is service-specific (e.g. to a particular VoIP service), data that
 * is defined by the end-user, data that is incorporated at chip tapeout
 * time, etc.  The DataSet API is designed to be flexible enough to
 * support all of these needs.
 *
 * Each DataSet is uniquely identified by a unique DataSet ID.  (Reserved
 * DataSet IDs are listed in dsetid.h).
 *
 * Interpretation of data in a DataSet is completely up to the caller.
 *
 * Depending on system configuration, DataSets may be stored in ROM,
 * RAM, Flash, OTP, on the Host -- potentially anywhere.  The DataSet
 * API hides the physical location of a DataSet -- and the method of
 * accessing it --  so that it's possible, for instance, for a
 * RAM-based DataSet to override an equivalent ROM-based DataSet.
 * It's also possible for, say, a flash-based DataSet to contain
 * small updates to a large ROM-based DataSet; this is hidden by the
 * API.
 * 
 * The DataSet API is broadly divided into read APIs and write APIs.
 * The read APis include:
 *    dset_open     -- Locate a DataSet and set up whatever "plumbing"
 *                     is needed to access that DataSet.
 *    dset_read     -- Read specified data from a DataSet
 *    dset_size     -- Return the size of a DataSet opened
 *                     by an earlier dset_open
 * The write APIs include:
 *    dset_create   -- Locate suitable space to store a DataSet
 *    dset_write    -- Write specified data to a DataSet
 *    dset_commit   -- Commit all written data to underlying media
 *    dset_destroy  -- Remove a DataSet
 * Note that write operations may not be supported on all media.
 *
 * Notes:
 * It is up to the caller to handle any compression, encryption, cacheing,
 * verification, security, consistency checking, sanity checking (checksums,
 * magic numbers, etc.).
 *
 * Several APIs offer both synchronous and asynchronous operation.
 * Using synchronous operation, a request is handled before the call
 * returns. Using asynchronous operation, the request is "queued" and
 * a caller-supplied function is invoked when the request is completed.
 * In some situations, asynchronous operation is mandatory. For instance,
 * an attempt to access a DataSet that is stored on the Host must be
 * made asynchronously. As a general rule, it's best to use asynchronous
 * operations since they work with any media. But in some cases
 * this adds too much complexity to calling code so a developer may choose
 * to use synchronous operation (which places restrictions on where the
 * DataSet may reside).
 */

/*
 * An open DataSet is represented by an unsigned int (A_UINT32) DataSet Handle.
 * Memory for the handle is allocated by the caller and is filled in by
 * dset_open/dset_create.  The Handle may then be used in calls to dset_read
 * and dset_write.
 */

#define		DSET_ID(a,b)		((a) << 16 | b)
#define		DSET_ID_CLASS(c)	(((c) >> 16) & 0x0FFFF)

#define 	DSET_ID_UNUSED 		257
#define 	DSET_ID_RWFILE 		0x1124
#define 	DSET_ID_ROFILE 		0x1125

#define         DSET_ID_STORERECALL     DSET_ID(0,0x1126)

#define         MAX_HOST_DSETS            10

/*
 * All callbacks used the same prototype.  Callbacks are invoked with
 *   status of the operation that was requested
 *   a pointer-sized argument supplied by the caller at request time
 */
typedef void (* dset_callback_fn_t)(A_STATUS status, void *callback_arg);

struct dset_api {
    /*
     * Called once to initialize the DataSet module.
     */
    void (* _dset_init)(void);

    /*
     * Request access to a DataSet specified by id.
     */
    A_STATUS (* _dset_open)(A_UINT32 id,
                            A_UINT32 *dset_handle);


    /*
     * Request data from a previously-opened DataSet.
     *   dset_handle is the open DataSet
     *   offset is the offset into the DataSet
     *   length is the number of bytes desired
     *   buf is a pointer to the buffer to be filled in with the data.
     * NB: Historic synchronous-only read API which works only with
     *     RAM- and ROM-based DataSets. All new software should use
     *     _dset_read_ex!
     */
    A_STATUS (* _dset_read)(A_UINT32 dset_handle,
                            A_UINT32 offset,
                            A_UINT32 length,
                            A_UINT8 *buf);

    /*
     * Return the size of a DataSet.
     * This call is synchronous only.
     */
    A_UINT32 ( * _dset_size)(A_UINT32 dset_handle);
    
    /*
     * Retrieve data from a patched dataset.
     */
    A_STATUS (* _BPatch)(A_UINT32 dset_handle,
                         A_UINT32 offset,
                         A_UINT32 length,
                         A_UINT8 *buf);
    /*
     * Retrieve data from a compressed dataset.
     */
    A_STATUS (* _uncompress)(A_UINT32 dset_handle,
                             A_UINT32 offset,
                             A_UINT32 length,
                             A_UINT8 *buf);

#if defined(CONFIG_DSET_XTNS)
    /*
     * Extended version of dset_open which support asynchronous
     * operation as well as synchronous operation.
     *   dset_handle is an opaque handle returned from 
     *               dset_open_ex to be used in future calls
     *   dset_id is a fixed DataSet ID
     *   flags - see DSET_OPEN_FLAG_*
     *   open_cb is the callback function for asynchronous operation
     *   callback_arg is an argument to be passed to open_cb
     *
     * If open_cb==NULL, dset_open_ex operates synchronously
     * (if permitted by the underlying media) and it updates
     * dset_handle before the call returns.  If open_cb is
     * non-NULL, dset_open_ex operates asynchronously and it
     * updates dset_handle before it invokes open_cb.
     * Asynchronous operation is REQUIRED for some media.
     */
    A_STATUS (* _dset_open_ex)(A_UINT32 *pdset_handle,
                               A_UINT32 dset_id,
                               A_UINT32 flags,
                               dset_callback_fn_t open_cb,
                               void *callback_arg);
    /*
     * Extended version of dset_read which supports asynchronous
     * operation as well as synchronous operation.
     *   dset_handle is the opened DataSet
     *   buffer is a pointer to the buffer to be filled in
     *   length is the number of bytes to be read
     *   offset is an offset into the DataSet where reading begins
     *   read_cb is the callback function for asynchronous operation
     *   callback_arg is an argument to be passed to read_cb
     */
    A_STATUS (* _dset_read_ex)(A_UINT32 dset_handle,
                               A_UINT8 *buffer,
                               A_UINT32 length,
                               A_UINT32 offset,
                               dset_callback_fn_t read_cb,
                               void *callback_arg);

    /*
     * Relinquish access to the specified dset_handle.
     * close_cb is the callback function for asynchronous operation
     * callback_arg is an argument to be passed to close_cb
     */
    A_STATUS (* _dset_close)(A_UINT32 dset_handle,
                             dset_callback_fn_t close_cb,
                             void *callback_arg);

    /*
     * Create a new DataSet with ID=dset_id by essentially
     * reserving space on the underlying media.
     *   dset_handle is an opaque handle returned from 
     *       dset_create to be used in future calls
     *   dset_id - fixed DataSet ID
     *   length - length of this DataSet
     *   flags - see DSET_FLAG_* and DSET_MEDIA_*
     *   create_cb is the callback function for asynchronous operation
     *   callback_arg is an argument to be passed to create_cb
     *
     * If create_cb==NULL, dset_create operates synchronously
     * (if permitted by the underlying media) and it
     * updates dset_handle before the call returns.  If create_cb
     * is non-NULL, dset_create operates asynchronously and it
     * updates dset_handle before it invokes create_cb.
     * Asynchronous operation is REQUIRED for some media.
     *
     * Note: Some media may prohibit DataSet creation.
     */
    A_STATUS (* _dset_create)(A_UINT32 *pdset_handle,
                              A_UINT32 dset_id,
                              A_UINT32 length,
                              A_UINT32 flags,
                              dset_callback_fn_t create_cb,
                              void *callback_arg);


    /*
     * Write data to a previously-created DataSet.
     *   dset_handle is the created DataSet
     *   buffer is a pointer to the data to be written
     *   length is the number of bytes to be written
     *   offset is an offset into the DataSet where writing begins
     *   flags - see DSET_WRITE_FLAG_*
     *   write_cb is the callback function for asynchronous operation
     *   callback_arg is an argument to be passed to write_cb
     *
     * In order for data to be written properly, the buffer
     * must remain valid until write_cb is invoked.
     *
     * Note: Not all media permit unrestricted writes.
     * If the media is unknown, writes to a DataSet should
     * occur sequentially starting from offset 0.
     */
    A_STATUS (* _dset_write)(A_UINT32 dset_handle,
                             A_UINT8 *buffer,
                             A_UINT32 length,
                             A_UINT32 offset,
                             A_UINT32 flags,
                             dset_callback_fn_t write_cb,
                             void *callback_arg);

#define DSET_WRITE_FLAG_NOCOPY  1 /* For RAM/ROM data, use supplied buffer directly */

    /*
     * Commit a DataSet to media.  Data writes may have been
     * committed to media already but a newly created DataSet
     * only becomes visible after a dset_commit.
     *   dset_handle is the created DataSet
     *   commit_cb is the callback function for asynchronous operation
     *   callback_arg is an argument to be passed to commit_cb
     */
    A_STATUS (* _dset_commit)(A_UINT32 dset_handle,
                              dset_callback_fn_t commit_cb,
                              void *callback_arg);

    /*
     * Delete a DataSet from media.
     *   dset_id is a fixed DataSet ID
     *   flags - see DSET_DELETE_FLAG_* and DSET_MEDIA_*
     *   delete_cb is the callback function for asynchronous operation
     *   callback_arg is an argument to be passed to delete_cb
     */
    A_STATUS (* _dset_delete)(A_UINT32 dset_id,
                              A_UINT32 flags,
                              dset_callback_fn_t delete_cb,
                              void *callback_arg);

    /*
     * Return the media associated with an open DataSet.
     * The value returned always includes DSET_MEDIA_*.
     * The upper bits of the return value may convey
     * additional media-specific information, so if
     * the caller just wants to know media, AND the
     * return value with DSET_FLAG_MEDIA_MASK.
     *
     * This call is synchronous only.
     */
    A_UINT32 (* _dset_media)(A_UINT32 dset_handle);
#endif /* CONFIG_DSET_XTNS */
};

#if defined(CONFIG_DSET_XTNS)
/*
 * DataSet Media, DSET_MEDIA_*.
 * When using dset_create or dset_delete, you can specify
 * "flags | DSET_MEDIA_xxx".
 */
#define DSET_MEDIA_GENERIC  0
#define DSET_MEDIA_HOST     1
#define DSET_MEDIA_FIRST    DSET_MEDIA_HOST
#define DSET_MEDIA_RAM      2
#define DSET_MEDIA_NVRAM    3
#define DSET_MEDIA_OTP      4
#define DSET_MEDIA_ROM      5
#define DSET_MEDIA_MEM      6 /* RAM or ROM */
#define DSET_MEDIA_AORAM    7

/* .... */
#define DSET_MEDIA_MAX     15

#define DSET_FLAG_MEDIA_MASK        0x0000000f /* DSET_MEDIA_* */
#define DSET_FLAG_CIRCULAR          0x00000010 /* Circular DataSet (when media supports it) */
#define DSET_FLAG_SECURE            0x00000020 /* Secure Storage */
#define DSET_FLAG_INTERNAL_RESERVED 0xff000000 /* Reserved for internal use */
#endif /* CONFIG_DSET_XTNS */

extern void dset_module_install(struct dset_api *api);
extern unsigned int persistent_dset_media;

#define dset_init() \
        A_INDIR(dset._dset_init())

#define dset_open(id, handle) \
        A_INDIR(dset._dset_open((id), (handle)))

#define dset_read(handle, offset, length, buf) \
        A_INDIR(dset._dset_read((handle), (offset), (length), (buf)))

#define dset_size(handle) \
        A_INDIR(dset._dset_size(handle))

#if defined(CONFIG_DSET_XTNS)
#define dset_open_ex(phandle, id, flags, cb, arg) \
        A_INDIR(dset._dset_open_ex((phandle), (id), (flags), (cb), (arg)))

#define dset_read_ex(handle, buf, length, offset, cb, arg) \
        A_INDIR(dset._dset_read_ex((handle), (buf), (length), (offset), (cb), (arg)))

#define dset_close(handle, cb, arg) \
        A_INDIR(dset._dset_close((handle), (cb), (arg)))

#define dset_create(phandle, id, length, flags, cb, arg) \
        A_INDIR(dset._dset_create((phandle), (id), (length), (flags), (cb), (arg)))

#define dset_write(handle, buffer, length, offset, flags, cb, arg) \
        A_INDIR(dset._dset_write((handle), (buffer), (length), (offset), (flags), (cb), (arg)))

#define dset_commit(handle, cb, arg) \
        A_INDIR(dset._dset_commit((handle), (cb), (arg)))

#define dset_delete(dset_id, flags, delete_cb, callback_arg) \
        A_INDIR(dset._dset_delete((dset_id), (flags), (delete_cb), (callback_arg)))

#define dset_media(handle) \
        A_INDIR(dset._dset_media(handle))
#endif /* CONFIG_DSET_XTNS */

#define A_BPatch(dset_handle, offset, length, buf) \
           A_INDIR(dset._BPatch((dset_handle), (offset), (length), (buf)))
           
#define A_Uncompress(dset_handle, offset, length, buf) \
           A_INDIR(dset._uncompress((dset_handle), (offset), (length), (buf)))


#endif /* __DSET_API_H__ */
