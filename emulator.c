/*
 * emulator.c
 * by: Ambika Jayakumar and Danielle Zelin
 * date: 3/29/15
 * Assignment 6
 * Summary: boots the emulator, fetches the instruction containing the opcode
 *          and the registers, executes the instruction, and increments the
 *          program counter
 *
 */

#include "emulator.h"
#include "inttypes.h"

static mem_t boot(FILE *fp, mem_t memory, uint32_t file_length);
static inline void fetch(mem_t *memory, uint32_t *registers);
/*
 * Purpose: Serves as the main function for the emulator module and is 
 *          responsible for calling the functions to emulate the UM
 */
void emulate(FILE *fp, uint32_t file_length)
{
        mem_t memory = NULL;
        uint32_t registers[8] = {0,0,0,0,0,0,0,0};
        memory = boot(fp, memory, file_length);
        fetch(&memory, registers);
}

/*
 * Purpose: Creates memory for the necessary data structures that make up the
 *          memory: sequence, uarray, and stack. Also initializes the memory
 *          and maps segment zero
 */
static mem_t boot(FILE *fp, mem_t memory, uint32_t file_length)
{
        memory = memory_new(file_length / 4);
        
        /* the while loop will only end when the halt operation is used */
        for (int i = 0; i < (int)(file_length / 4) ; i++) {
                uint32_t word = 0;
                /* getc reads in word 8 bytes at a time from input */
                uint32_t left = getc(fp);
                if ((int)left == EOF) break;
                
                unsigned midleft = getc(fp);
                unsigned midright = getc(fp);
                unsigned right = getc(fp);
                
                word = (left << 24 | midleft << 16 | midright << 8 | right);
                uint32_t *value = malloc(sizeof(uint32_t));
                *value = word;
                word_store(&memory, 0, i, value); 
                
                free(value);
        }
      
        return memory;
}

/*
 * Purpose: Responsible for updating the program counter and calling the
 *          function that determines the opcode and performs the necessary
 *          actions
 */
static void fetch(mem_t *memory, uint32_t *reg)
{
        uint32_t program_counter = 0;
        uint32_t opcode, a, b, c, val;
        //int update_counter;
        
        while (1) {
                uint32_t word =  word_load(memory, 0, program_counter);
                
                /* unpacks the 32 bit instruction */
                opcode = ((word >> 28) & 0xF);
                if ((int) opcode != 13) {
                        c = (word & 0x7);
                        b = ((word >> 3) & 0x7);
                        a = ((word >> 6) & 0x7);
                } else {
                        a = ((word >> 25) & 0x7);
                        val = ((word << 0x7) >> 0x7);
                }     
                
                /* calls a function according to the unpacked opcode */
                switch (opcode) {
                        case CMOV :
                                if (reg[c] != 0) reg[a] = reg[b];
                                program_counter++;
                                break;
                        case SLOAD :
                                reg[a] = word_load(memory, reg[b], reg[c]);
                                program_counter++;
                                break;
                        case SSTORE :
                                word_store(memory, reg[a], reg[b], &(reg[c]));
                                program_counter++;
                                break;
                        case ADD :
                                reg[a] = reg[b] + reg[c]; 
                                program_counter++;
                                break;
                        case MUL :
                                reg[a] = reg[b] * reg[c];
                                program_counter++;
                                break;
                        case DIV :
                                reg[a] = reg[b] / reg[c];
                                program_counter++;
                                break;
                        case NAND :
                                reg[a] = ~(reg[b] & reg[c]);
                                program_counter++;
                                break;
                        case HALT :
                                memory_free(memory); 
                                return;
                                break;
                        /* MAP function */
                        case ACTIVATE :
                                reg[b] = segment_map(memory, reg[c]);
                                program_counter++;
                                break;
                        /* UNMAP function */
                        case INACTIVATE :
                                segment_unmap(memory, reg[c]);
                                program_counter++;
                                break;
                        /* OUTPUT function */
                        case OUT :
                                assert((int)reg[c] >= 0 && (int)reg[c] <= 255);
                                printf("%c", reg[c]);
                                program_counter++;
                                break;
                        /* INPUT function */
                        case IN :
                                reg[c] = (uint32_t) getc(stdin);
                                program_counter++;
                                break;
                        case LOADP :
                                if (reg[b] != 0) {
                                        UArray_T dup = segment_load(memory,
                                                                    reg[b]);
                                        segment_store(memory, 0, dup);
                                }
                                program_counter = reg[c];
                                break;
                        /* LOAD VALUE function */
                        case LV :
                                reg[a] = (uint32_t) val;
                                program_counter++;
                                break;
                }
        }
}

