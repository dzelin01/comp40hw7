/* 
 * bitpack.c
 * By: Danielle Zelin and Hayley Cohen
 * Date: 2/22/15
 * Assignment 4 
 *
 * Summary: Implementation of the bitpack interface. Determines if a value
 * can fit in a given amount of bits, inserts the value, and receives the 
 * values
 */

#include "bitpack.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "except.h"
#include "assert.h"

static inline unsigned shift_left(uint64_t word, unsigned dim, int shift_val);
static inline unsigned shift_rightu(uint64_t word, unsigned dim, int shift_val);
static inline signed shift_rights(uint64_t word, unsigned dim, int shift_val);

/*
 * Determines whether an unsigned int can git in a given amount of width
 */ 
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        unsigned int max_val = (1 << width) - 1;
        unsigned int min_val = 0;
        
        if (n >= min_val && n <= max_val) 
                return true;
        
        return false;
}

/*
 * Determines whether a signed int can git in a given amount of width
 */ 
bool Bitpack_fitss(int64_t n, unsigned width)
{
        unsigned int unsigned_max_num = (1 << width);
        int max_val = (unsigned_max_num / 2) - 1;
        int min_val = -(unsigned_max_num / 2);
        if (n >= (signed) min_val && n <= (signed) max_val) 
                return true;
        return false;
}

/*
 * Extracts a specific unsigned value from a given word with a width and least
 * significant bit value
 */ 
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        if (width == 0)
                return (uint64_t) 0;

        uint64_t mask;

        mask = shift_left(word, width + lsb, 64 - (lsb + width));
        mask = shift_rightu((uint64_t) mask, width + lsb, 64 - width);

        return mask;
}

/*
 * Extracts a specific signed value from a given word with a width and least
 * significant bit value
 */ 
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        if (width == 0)
                return (uint64_t) 0;

        uint64_t mask;

        mask = shift_left(word, width + lsb, 64 - (lsb + width));
        mask = shift_rights((uint64_t) mask, width + lsb, 64 - width);

        return mask;
}

/*
 * Takes in a word and inserts an unsigned value in a given location
 */
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t     
                                                                        value)
{
       bool valid = Bitpack_fitsu(value, width);
       Except_T Bitpack_Overflow;

       if (!valid)
               RAISE (Bitpack_Overflow);
      
        unsigned mask = ~0;
        mask = shift_rightu(mask, lsb + width, 64 - width);
        mask = shift_left(mask, lsb + width, lsb);
        
        mask = ~mask;
        mask = (mask & word);
        value = shift_left(value, lsb + width, 64 - width);
        value = shift_rightu(value, lsb + width, 64 - width - lsb);
        mask = (mask | value);

        return (uint64_t) mask;
        
}

/*
 * Takes in a word and inserts a signed value in a given location
 */
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, int64_t
                                                                        value)
{      
        bool valid = Bitpack_fitss(value, width);
        Except_T Bitpack_Overflow;
        
        if (!valid)
                RAISE (Bitpack_Overflow);

        unsigned mask = ~0;
        mask = shift_rightu(mask, lsb + width, 64 - width);
        mask = shift_left(mask, lsb + width, lsb);     
        
        mask = ~mask;
        mask = (mask & word);
        value = shift_left(value, lsb + width, 64 - width);
        value = shift_rightu(value, lsb + width, 64 - width - lsb);
        mask = (mask | value);

        return (uint64_t) mask;
}
 
/*
 * Shifts left by adding zeros and checks that the given values fit within a
 * uint64_t
 */
static inline unsigned shift_left(uint64_t word, unsigned dim, int shift_val)
{
        assert(dim <= (unsigned) 64);
        return (unsigned) word << shift_val;
}

/*
 * Shifts right by adding zeros and checks that the given values fit within a
 * uint64_t
 */
static inline unsigned shift_rightu(uint64_t word, unsigned dim, int shift_val)
{
        assert(dim <= (unsigned) 64);
        return (unsigned) word >> shift_val;
}

/*
 * Shifts right by adding the value of the most significant bit and checks that
 * the given values fit within a uint64_t
 */
static inline signed shift_rights(uint64_t word, unsigned dim, int shift_val) 
{
        assert(dim <= (unsigned) 64);
        return (signed) word >> shift_val;
}