/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#ifndef __MALLOC_API_H__
#define __MALLOC_API_H__

/*
 * API for Target-side malloc/free, not for general use.
 *
 * These APIs are intended for use in limited circumstances, and are
 * provided mainly to facilitate porting of existing bodies of code
 * such as the "bignum" library needed by WPS.
 * 
 * A brief comparison of allocram and malloc:
 *
 *     allocram: returns cache-aligned DRAM (minimum word-aligned)
 *     malloc:   returns word-aligned DRAM
 *
 *     allocram: DRAM is cleared prior
 *     malloc:   DRAM is not cleared 
 *
 *     allocram: ASSERTS if the allocation request cannot be satisfied
 *     malloc:   returns NULL if the allocation request cannot be satisfied
 *
 *     allocram: DRAM will never be freed
 *     malloc:   DRAM is allocated for limited duration and
 *               *must be freed ASAP*
 *
 *     allocram: Mostly used during startup
 *     malloc:   May be used after startup
 *
 *     allocram: May be used to allocate very large chunks of DRAM
 *     malloc:   Limits the size of DRAM arenas (and therefore allocations)
 *               to MALLOC_ARENA_MAX
 */
struct malloc_arena_s
{
	A_UINT32 offset_lower;
	A_UINT32 offset_higher;
};

struct malloc_api_s {
    /*
     * Initialize the malloc module.  Expected to be called once
     * after the module is installed and before any other API is
     * used.
     *
     * Note: The allocram module must be initialized before malloc.
     */
    void (* _malloc_module_init)(void);

    /*
     * Allocate nbytes of DRAM, 32-bit aligned from the default malloc arena.
     * If a contiguous block of nbytes of DRAM is not available, returns NULL.
     * This function does not "sleep" or "wait" for DRAM, but it may try
     * to grab more space from allocram. It is the caller's responsibility
     * to deal with a NULL return. The DRAM returned is not cleared.
     */
    void *(* _malloc_alloc)(unsigned int nbytes);

    /* Same as malloc_alloc, but returns DRAM space that has been zero'ed */
    void *(* _malloc_calloc)(unsigned int nbytes);

    /* Standard realloc */
    void *(* _malloc_realloc)(void *ptr, unsigned int nbytes);

    /*
     * Free space that was allocated in an earlier allocation.
     * Returns the number of contiguous bytes made available by this operation.
     */
    unsigned int (* _malloc_free)(void *ptr);

    /*
     * Allocate from an arena. The arena parameter is the value
     * returned from an earlier malloc_arena_create.
     */
    void *(* _malloc_arena_alloc)(struct malloc_arena_s *arena, unsigned int nbytes, A_BOOL clear_flag);

#if !defined(AR6002_REV72)
    /* Free into an arena.  */
    unsigned int (* _malloc_arena_free)(struct malloc_arena_s *arena, void *ptr);
#endif

    /*
     * Create a private malloc arena that uses caller-supplied memory.
     * dram_start must be 32-bit aligned.
     * nbytes must be a multiple of 4.
     * nbytes must be at least MALLOC_MIN_FREE_BLOCK.
     * nbytes should allow some room for malloc overhead.
     */
    struct malloc_arena_s *(* _malloc_arena_create)(void *dram_start, unsigned int nbytes);

    /* Destroy a private malloc arena. */
    void (* _malloc_arena_destroy)(struct malloc_arena_s *arena);
};

/*
 * Maximum supported size of a malloc arena.
 * Applies to both private and default arenas.
 */
#define MALLOC_ARENA_MAX (512*1024 - sizeof(struct malloc_arena_s))

/*
 * Minimum number of bytes of residual space that is worth
 * tracking as a block.
 */
#define MALLOC_MIN_FREE_BLOCK 24 /* 16 bytes + 1 block info word */

extern struct malloc_api_s *malloc_api;

#define malloc_module_init()            malloc_api->_malloc_module_init()
#define malloc_alloc(nb)                malloc_api->_malloc_alloc(nb)
#define malloc_calloc(nb)               malloc_api->_malloc_calloc(nb)
#define malloc_realloc(p, nb)           malloc_api->_malloc_realloc((p), (nb))
#define malloc_free(p)                  malloc_api->_malloc_free(p)
#define malloc_arena_alloc(a, nb, cf)   malloc_api->_malloc_arena_alloc((a), (nb), (cf))
#define malloc_arena_free(a, p)         malloc_api->_malloc_arena_free((a), (p))
#define malloc_arena_create(dram, nb)   malloc_api->_malloc_arena_create((dram), (nb))
#define malloc_arena_destroy(a)         malloc_api->_malloc_arena_destroy(a)

#define malloc(nb)     malloc_alloc(nb)
#define calloc(ne, nbpe)     malloc_calloc(((ne)*(nbpe)))
#define realloc(p, nb) malloc_realloc((p), (nb))
#define free(p)        (void)malloc_free(p)

#endif /* __MALLOC_API_H__ */
