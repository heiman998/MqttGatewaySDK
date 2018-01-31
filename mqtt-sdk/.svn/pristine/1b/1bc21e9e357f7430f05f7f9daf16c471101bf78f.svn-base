/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */
 
#if defined(OTP_TEST)
#include "otp_test.h"
#else
#include "osapi.h"
#include "otpstream_api.h"
#include "efuse_api.h"
#endif

/*
 * Write portion of the implementation of One Time Programmable
 * Stream layer bound to eFuse OTP memory.
 */

#if defined(OTP_SUPPORT) || defined(OTP_TEST)
/*
 * Checks if there's enough room to commit the remainder of a stream
 * assuming no bit errors in the underlying OTP memory.  The idea
 * is to recognize a hopeless situation as quickly as possible and
 * abort the stream write ASAP without writing any more OTP memory.
 *
 * In practice, this sort of abort should never occur because it
 * means that we're either trying to write too much to OTP or that
 * the OTP memory is full of errors.
 */
#define OTP_REMAINING(desired) (otpstream_remaining >= (desired))

void
_otpstream_write_reset(void)
{
    A_UINT8 ch;

    /*
     * Recalculate otpstream_remaining by scanning
     * backwards from the end of OTP until we
     * see a non-zero byte.  Assuming everything
     * before that point is well-formed, that's the
     * first potential place to start a new otpstream.
     */
    for (otpstream_index = otpstream_max_index ;; otpstream_index--)
    {
        efuse_read(otpstream_index, &ch, 1);
        if (ch != 0x00) {
            otpstream_index++;
            break;
        }

        if (otpstream_index == otpstream_min_index) {
            break;
        }
    }
    otpstream_remaining = otpstream_max_index + 1 - otpstream_index;

    otpstream_start_index = otpstream_index;
}

#if defined(AR6002_REV43)
unsigned int otpstream_erase_max=(EFUSE_MAX_NUM_BYTES/2); /* arbitrary cutoff */
A_UINT8 otpstream_erase_byte=0xff;

A_STATUS
_otpstream_erase_all(void)
{
    unsigned int first_index, last_index;
    A_STATUS status;

    otpstream_reset(OTPSTREAM_READ);
    first_index = otpstream_index;
    otpstream_reset(OTPSTREAM_WRITE);
    last_index = otpstream_index;

    if (last_index-first_index  > otpstream_erase_max)  {
        A_PRINTF_ALWAYS("Error: Attempt to erase too much OTP (%d bytes)\n", last_index-first_index);
        return A_EINVAL;
    }

    if (first_index == last_index) {
        A_PRINTF_ALWAYS("Error: Nothing to erase on OTP\n");
        return A_EINVAL;
    }

    efuse_enable_write();

    /* Overwrite all bytes with the otpstream_erase_byte byte */
    for (otpstream_index=first_index; otpstream_index<last_index; otpstream_index++) {
        status = efuse_write(otpstream_index, &otpstream_erase_byte, 1, NULL);
        if (status != A_OK) {
            A_PRINTF_ALWAYS("Warning: OTP erasure failed at offset %d\n", otpstream_index);
            /* Continue....some failures might be OK */
        }
    }

    /* Write double END symbols */
    OTPSTREAM_SYMBOLS_DERIVE(otpstream_erase_byte);
    status = efuse_write(otpstream_index, &otpstream_END_symbol, 1, NULL);
    if (status != A_OK) {
        A_PRINTF_ALWAYS("Error: Cannot write END1 at OTP %d\n", otpstream_index);
        efuse_disable();
        return A_ERROR;
    }
    otpstream_index++;
    status = efuse_write(otpstream_index, &otpstream_END_symbol, 1, NULL);
    if (status != A_OK) {
        A_PRINTF_ALWAYS("Error: Cannot write END2 at OTP %d\n", otpstream_index);
        efuse_disable();
        return A_ERROR;
    }

    efuse_disable();

    /* Verify that the erasure worked well enough. */
    {
        unsigned int nbytes;
        A_UINT8 first_byte;

        otpstream_reset(OTPSTREAM_READ);
        otpstream_read(&first_byte, 1, &nbytes);
        if ((nbytes != last_index-first_index-1) ||
            (nbytes && (first_byte != otpstream_erase_byte)))
        {
            A_PRINTF_ALWAYS("Error: OTP erasure failed!\n");
            return A_ERROR;
        }
    }

    return A_OK;
}

#endif /* AR6002_REV43 */
/*
 * Selection of POP, END, and ESCAPE symbols for use with an otpstream
 * is made to minimize the number of escaped bytes needed.
 *
 * The chosen symbols are required to meet two conditions:
 * 1) Each symbol must have at least 1 bit set that the others do not
 * 2) Knowledge of the POP symbols must allow us to derive the other
 *    two symbols.
 * In addition, the algorithm should use minimal space and reasonable
 * time (though this algorithm only executes when WRITING OTP, so it's
 * not too cricital).
 *
 * This current algorithm chooses to FIX the most significant three bits
 * in order satisfy condition 1:
 *  001 -> POP
 *  010 -> ESCAPE
 *  100 -> END
 * The remaining 5 bits are common to all 3 chosen symbols so that
 * condition 2 is trivially satisfied.  The choice of those 5 bits
 * is made by minimizing the number of bytes in buf that would
 * require escaping.  We determine this minimum through an exhuastive
 * scan.  So this symbol selection algorithm requires at most 32*buflen
 * comparisons.  We cut short the search as soon we find a symbol
 * selection that would allow the entire buffer to be stored with 0
 * escape symbols.  In practice, this should happen frequently because
 * streams are expected to be short.  In the worst possible scenario, 
 * we will need to escape only buflen/32 bytes in the stream using
 * this algorithm.
 */

LOCAL unsigned int
otpstream_symbols_count(A_UINT8 basis, A_UINT8 *buf, unsigned int buflen)
{
    A_UINT8 pop_sym, esc_sym, end_sym;
    int i;
    unsigned int count;

    otpstream_symbols_derive(basis, &pop_sym, &esc_sym, &end_sym);

    count = 0;
    for (i=0; i<buflen; i++) {
        if ((buf[i] == pop_sym)
          || (buf[i] == esc_sym)
          || (buf[i] == end_sym))
        {
            count++;
        }
    }

    return count;
}

LOCAL void
_otpstream_symbols_select(A_UINT8 *buf, unsigned int buflen)
{
    int min_escapes = buflen+1;
    int lowbits, best_lowbits = -1;
    int cnt;

    for (lowbits=0; lowbits<32; lowbits++) {
        cnt = otpstream_symbols_count(lowbits, buf, buflen);

        if (cnt < min_escapes) {
            best_lowbits = lowbits;
            min_escapes = cnt;

            if (min_escapes == 0) {
                break; /* No need to look any further */
            }
        }
    }

    /* The selection is made -- use best_lowbits */
    OTPSTREAM_SYMBOLS_DERIVE(best_lowbits);
    otpstream_escbytes_remaining = min_escapes;
}

LOCAL A_STATUS
otpstream_push(A_UINT8 *pbyte)
{
    A_STATUS status;

    A_ASSERT(otpstream_index <= otpstream_max_index);
    A_ASSERT(otpstream_remaining > 0);
    status = efuse_write(otpstream_index, pbyte, 1, NULL);
    otpstream_index++;
    otpstream_remaining--;

    return status;
}


/* otpstream_write state machine states */
enum otpstream_write_state {
        WSTATE_INVALID,
        WSTATE_INIT,
        WSTATE_NORMAL,
        WSTATE_ESCAPE_OK,
        WSTATE_END_OK,
        WSTATE_POP_1,
        WSTATE_POP_2,
        WSTATE_POP,
        WSTATE_OOPS_ESCAPE,
        WSTATE_OOPS_END,
};

LOCAL A_STATUS
_do_otpstream_write(A_UINT8 *buf, unsigned int buflen)
{
    enum otpstream_write_state state, old_state = WSTATE_INVALID;
    unsigned int buf_index = 0;
    A_UINT8 ch;
    int bad_count = 0;
    int pop_incrementor = 0;

    if (!(otpstream_current_op == OTPSTREAM_WRITE)) {
        efuse_disable();
        A_ASSERT_ALWAYS(0);
    }

    state = WSTATE_INIT;
    otpstream_start_index = otpstream_index;

    for (;;) {
        switch(state) {
            case WSTATE_NORMAL:
                /* Need space for remaining buffer plus ESC symbols plus 2 END symbols */
                if (!OTP_REMAINING(buflen-buf_index+otpstream_escbytes_remaining+2)) {
                    return A_EINVAL;
                }

                if (buf_index == buflen) {
                    /* Reached end of buffer -- write first END symbol */
                    if (otpstream_push(END_SYMBOL) == A_OK) {
                        state = WSTATE_END_OK;
                    } else {
                        state = WSTATE_POP_2;
                        old_state = WSTATE_NORMAL;
                        bad_count = 1;
                    }
                } else {
                    /*
                     * We're somewhere in the middle of the buffer.
                     * Check for any special processing needed to escape special symbols.
                     */
                    ch = buf[buf_index];
                    if (IS_SYMBOL(ch)) {
                        /* Need to escape this byte */
                        if (otpstream_push(ESCAPE_SYMBOL) == A_OK) {
                            state = WSTATE_ESCAPE_OK;
                            otpstream_escbytes_remaining--;
                        } else {
                            state = WSTATE_POP_2;
                            old_state = WSTATE_NORMAL;
                            bad_count = 1;
                        }
                    } else {
                        /* Normal case: write a byte from buffer to OTP */
                        if (otpstream_push(&ch) == A_OK) {
                            buf_index++;
                        } else {
                            efuse_read(otpstream_index-1, &ch, 1); /* What did we write? */
                            /* Did we accidentally write a special symbol? */
                            if (IS_SYMBOL(ch)) {
                                if (IS_POP(ch)) {
                                    /*
                                     * If we accidentally wrote a POP symbol, we need
                                     * to back up (if we were not at the beginning of
                                     * the stream) and re-write the previous byte from
                                     * the input buffer.
                                     */
                                    if (buf_index > 0) {
                                        buf_index--;
                                        if (IS_SYMBOL(buf[buf_index])) {
                                            /* 
                                             * If we need to re-write a special symbol,
                                             * then we'll need to re-escape it.
                                             */
                                            otpstream_escbytes_remaining++;
                                        }
                                        /* Stay in WSTATE_NORMAL */
                                    } else {
                                        /*
                                         * Accidentally forced to start over, because
                                         * the very first character written was written
                                         * incorrectly as a POP symbol!
                                         */
                                        state = WSTATE_INIT;
                                    }
                                } else if (IS_END(ch)) {
                                    state = WSTATE_OOPS_END;
                                } else if (IS_ESCAPE(ch)) {
                                    state = WSTATE_OOPS_ESCAPE;
                                }
                            } else {
                                /* Wrote a bad byte, but not a symbol */
                                state = WSTATE_POP_2;
                                old_state = WSTATE_NORMAL;
                                bad_count = 1;
                            }
                        }
                    }
                }
                break;

            case WSTATE_INIT:
                /* Choose otpstream_POP_symbol, otpstream_ESCAPE_symbol and otpstream_END_symbol */
                otpstream_symbols_select(buf, buflen);

                /*
                 * The POP symbol must not be 0, and for each pair of symbols,
                 * at least 1 bit must be set in each symbol that is not set
                 * in the other symbol.  This restriction allows us to be certain
                 * that whenever we try to write a special symbol and that write
                 * fails, we will not end up with one of the other symbols in
                 * efuse OTP memory.  And that certainly allows us to both simplify
                 * code and otpimize OTP space.
                 */
                if(!((~otpstream_POP_symbol & otpstream_ESCAPE_symbol)
                         && (~otpstream_POP_symbol & otpstream_END_symbol)
                         && (~otpstream_ESCAPE_symbol & otpstream_END_symbol)
                         && (~otpstream_ESCAPE_symbol & otpstream_POP_symbol)
                         && (~otpstream_END_symbol & otpstream_POP_symbol)
                         && (~otpstream_END_symbol & otpstream_ESCAPE_symbol)))
                {
                    efuse_disable();
                    A_ASSERT_ALWAYS(0);
                }

                /* Need to write POP symbol plus data stream plus ESC symbols plus two END symbols */
                if (!OTP_REMAINING(buflen+otpstream_escbytes_remaining+3)) {
                    return A_EINVAL;
                }

                if (otpstream_push(POP_SYMBOL) == A_OK) {
                    state = WSTATE_NORMAL;
                    buf_index = 0;
                } else {
                    efuse_read(otpstream_index-1, &ch, 1); /* What did we write? */
                    if (ch == 0x00) {
                        /* Just try again in the next OTP position */
                    } else {
                        OTPSTREAM_SYMBOLS_DERIVE(ch);
                        state = WSTATE_POP_2;
                        old_state = WSTATE_INIT;
                        bad_count = 1;
                    }
                }
                break;

            case WSTATE_ESCAPE_OK:
                /* Need space for remaining buffer plus ESC symbols plus 2 END symbols */
                if (!OTP_REMAINING(buflen-buf_index+otpstream_escbytes_remaining+2)) {
                    return A_EINVAL;
                }

                /* Commit special symbol to OTP; it is preceded in OTP by an ESCAPE */
                if (otpstream_push(&buf[buf_index]) == A_OK) {
                    buf_index++;
                    state = WSTATE_NORMAL;
                } else {
                    state = WSTATE_POP_1;
                    old_state = WSTATE_ESCAPE_OK;
                    bad_count = 1;
                }
                break;

            case WSTATE_END_OK:
                /* Need space for second END symbol */
                if (!OTP_REMAINING(1)) {
                    return A_EINVAL;
                }

                if (otpstream_push(END_SYMBOL) == A_OK) {
                    return A_OK; /* Done writing otpstream to OTP! */
                } else {
                    state = WSTATE_POP_1;
                    old_state = WSTATE_END_OK;
                    bad_count = 1;
                }
                break;

            case WSTATE_POP_1:
                /* Need bad_count POP symbols. */
                pop_incrementor = 0;
                state = WSTATE_POP;
                break;

            case WSTATE_POP_2:
                /*
                 * Need bad_count POP symbols plus 1 for each bad byte written.
                 * If we fail to POP, we'll need one additional POP.
                 */
                pop_incrementor = 1;
                state = WSTATE_POP;
                break;

            case WSTATE_POP:
                /* Need space to erase, then add remaining buffer plus ESC symbols plus 2 END symbols */
                if (!OTP_REMAINING(bad_count + buflen-buf_index + otpstream_escbytes_remaining + 2)) {
                    return A_EINVAL;
                }

                if (otpstream_push(POP_SYMBOL) == A_OK) {
                    bad_count--; /* successful POP */
                } else {
                        /* failed to write POP symbol */
                        bad_count += pop_incrementor;
                }

                if (bad_count == 0) {
                    state = old_state; /* Successful POP */
                }
                break;

            case WSTATE_OOPS_ESCAPE:
                /*
                 * We accidentally wrote an ESC, so we need 2 POP symbols:
                 * The first to serve as the ESCaped symbol, and the second
                 * to pop that POP symbol.
                 */

                /* Need space for 2 POPs, then add remaining buffer plus ESC symbols plus 2 END symbols */
                if (!OTP_REMAINING(2 + buflen-buf_index + otpstream_escbytes_remaining + 2)) {
                    return A_EINVAL;
                }

                if (otpstream_push(POP_SYMBOL) == A_OK) {
                    /* Just need to POP the ESCaped POP, then back to NORMAL. */
                    state = WSTATE_POP_2;
                    old_state = WSTATE_NORMAL;
                    bad_count = 1;
                } else {
                    /*
                     * Most complex case: We accidentally wrote an ESC symbol,
                     * and when we tried to write a POP something else was
                     * accidentally written.  So now we need to POP everything
                     * after the ESC symbol, then write a POP (as the ESCaped
                     * byte) and finally a third POP in order to pop the 
                     * ESCaped POP symbol.
                     */
                     state = WSTATE_POP_1;
                     old_state = WSTATE_OOPS_ESCAPE;
                     bad_count = 1;
                }
                break;

            case WSTATE_OOPS_END:
                /* We accidentally wrote an END, so we need a POP symbol.  */

                /* Need space for a POP, then add remaining buffer plus ESC symbols plus 2 END symbols */
                if (!OTP_REMAINING(1 + buflen-buf_index + otpstream_escbytes_remaining + 2)) {
                    return A_EINVAL;
                }

                if (otpstream_push(POP_SYMBOL) == A_OK) {
                    state = WSTATE_NORMAL;
                } else {
                    /*
                     * We accidentally wrote an END symbol and when we tried
                     * to write a POP something else was accidentally written.
                     * So now we need to POP everything after the END symbol,
                     * then write a second POP to pop the END symbol.
                     */
                     state = WSTATE_POP_1;
                     old_state = WSTATE_OOPS_END;
                     bad_count = 1;
                }
                break;

            case WSTATE_INVALID:
                efuse_disable();
                A_ASSERT_ALWAYS(0);
                break;
        }
    }
}

LOCAL A_STATUS
_otpstream_write(A_UINT8 *buf, unsigned int buflen)
{
    A_STATUS status;

    efuse_enable_write();
    status = _do_otpstream_write(buf, buflen);
    efuse_disable();

    return status;
}

LOCAL unsigned int
_otpstream_remaining_bytes(void)
{
    A_ASSERT(otpstream_current_op == OTPSTREAM_WRITE);

    return otpstream_remaining;
}
#endif /* OTP_SUPPORT || OTP_TEST */

#if defined(OTP_SUPPORT)
/*
 * otpstream_read_module_install must be called before
 * otpstream_write_module_install.
 */
void
otpstream_write_module_install(struct otpstream_api_s *tbl)
{
    tbl->_otpstream_write =             _otpstream_write;
    tbl->_otpstream_remaining_bytes =   _otpstream_remaining_bytes;
    tbl->_otpstream_symbols_select  =   _otpstream_symbols_select;
    tbl->_otpstream_write_reset =       _otpstream_write_reset;
#if defined(AR6002_REV43)    
    tbl->_otpstream_erase_all =         _otpstream_erase_all;
#endif /* AR6002_REV43 */
}
#else /* OTP_SUPPORT */
void
otpstream_write_module_install(void *tbl)
{
}
#endif /* OTP_SUPPORT */
