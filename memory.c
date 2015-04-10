/*
 * memory.c
 * by: Ambika Jayakumar and Danielle Zelin
 * Date: 3/29/2015
 * Assignment 6
 * Summary: contains the functions for the segmented memory
 */

#include "memory.h"
#include "inttypes.h"

struct mem_t {
        Seq_T seg_memory;
        Stack_T unused_segments;
};

/*
 * Purpose: Mallocs spaces for the memory struct which containts a sequence,
 *          stack, and uarrays for each index in the sequence
 */
mem_t memory_new(int num_words)
{

        mem_t memory = malloc(sizeof(struct mem_t));        

        Seq_T seg_memory = Seq_new(100);
        UArray_T segment = UArray_new(num_words, sizeof(uint32_t));
        Seq_addlo(seg_memory, segment);
        memory->seg_memory = seg_memory;
        memory->unused_segments = Stack_new();
        return memory;
}

/*
 * Purpose: Frees all of the necessary data structures in memory
 */
void memory_free(mem_t *memory)
{
        int seq_size = Seq_length((*memory)->seg_memory);
        for (int i = 0; i < seq_size; i++) {
                UArray_T temp = Seq_remhi((*memory)->seg_memory);
                if (temp != NULL) {
                        UArray_free(&temp);
                }
        }
        Seq_free(&((*memory)->seg_memory));
        Stack_free(&((*memory)->unused_segments));
        free(*memory);
}

/*
 * Purpose: Returns a word from a given location in memory
 */
uint32_t word_load(mem_t *memory, uint32_t seg_num, uint32_t offset)
{
        UArray_T segment = Seq_get((*memory)->seg_memory, seg_num);
        uint32_t *word = UArray_at(segment, offset);
        return *word;
}

/*
 * Purpose: Stores a given value in a given location in memory
 */
void word_store(mem_t *memory, uint32_t seg_num, uint32_t offset, uint32_t *val)
{
        UArray_T segment = Seq_get((*memory)->seg_memory, seg_num);
        uint32_t *word = UArray_at(segment, offset);
        *word = *val;
}

/*
 * Purpose: Returns the UArray located at a given segment number
 */
UArray_T segment_load(mem_t *memory, uint32_t seg_num)
{
        UArray_T segment = Seq_get((*memory)->seg_memory, seg_num);       
        int size = UArray_length(segment);
        UArray_T new_seg = UArray_new(size, sizeof(uint32_t));
        /* puts new value in each word of a given segment */
        for (int i = 0; i < size; i++) {
                uint32_t word = word_load(memory, seg_num, i);
                uint32_t *value = UArray_at(new_seg, i);
                *value = word;
        }
        return new_seg;
}

/*
 * Purpose: Stores an entire UArray of words in a given location
 */
void segment_store(mem_t *memory, uint32_t seg_num, UArray_T array)
{
        UArray_T segment = Seq_get((*memory)->seg_memory, seg_num);
        UArray_free(&segment);
        Seq_put((*memory)->seg_memory, seg_num, array);
}

/*
 * Purpose: Determines the next available segment id in the sequence and
 *          allocates space of the given number of words
 */
uint32_t segment_map(mem_t *memory, uint32_t num_words)
{       
        uint32_t id;
        UArray_T new_segment;
        
        /* checks if there are any unmapped segment ids in the stack */
        if (Stack_empty((*memory)->unused_segments)) {
                assert((uint32_t)Seq_length((*memory)->seg_memory) <
                        ((uint32_t)1 << 31));
                new_segment = UArray_new(num_words, sizeof(uint32_t));
                Seq_addhi((*memory)->seg_memory, new_segment);
                id = (uint32_t) (Seq_length((*memory)->seg_memory) - 1);
        } else {
                id = stack_get(memory);
                new_segment = UArray_new(num_words, sizeof(uint32_t));
                Seq_put((*memory)->seg_memory, id, new_segment);
        } 
    
        return id;
}

/*
 * Purpose: Frees the uarray at a given segment number and sets it to NULL,
 *          pushes the segment number into the stack
 */
void segment_unmap(mem_t *memory, uint32_t seg_num)
{
        UArray_T segment = Seq_get((*memory)->seg_memory, seg_num);
        UArray_free(&segment);
        Seq_put((*memory)->seg_memory, seg_num, NULL);
        Stack_push((*memory)->unused_segments, (void *)(uintptr_t)seg_num);
}

/*
 * Purpose: Returns the most recently added segment id from the stack
 */
uint32_t stack_get(mem_t *memory)
{
        uintptr_t id = (uintptr_t)Stack_pop((*memory)->unused_segments); 
        return (uint32_t) id;
}
