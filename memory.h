/*
 * memory.h
 * by: Ambika Jayakumar and Danielle Zelin
 * date: 3/29/15
 * Assignment 6
 * Summary: header file for the header implementation
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "assert.h"
#include "uarray.h"
#include "seq.h"
#include "stack.h"

typedef struct mem_t *mem_t;

mem_t memory_new(int num_words);
void memory_free(mem_t *memory);
uint32_t word_load(mem_t *memory, uint32_t seg_num, uint32_t offset);
void word_store(mem_t *memory, uint32_t seg_num, uint32_t offset,
                uint32_t *val);
UArray_T segment_load(mem_t *memory, uint32_t seg_num);
void segment_store(mem_t *memory, uint32_t seg_num, UArray_T array);
uint32_t segment_map(mem_t *memory, uint32_t num_words);
void segment_unmap(mem_t *memory, uint32_t seg_num);
uint32_t stack_get(mem_t *memory);
