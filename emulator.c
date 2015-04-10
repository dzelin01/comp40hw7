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

static mem_t boot(FILE *fp, mem_t memory);
static void fetch(mem_t *memory, uint32_t *registers);
static int decode(mem_t *memory, uint32_t word, uint32_t *registers);

/*
 * Purpose: Serves as the main function for the emulator module and is 
 *          responsible for calling the functions to emulate the UM
 */
void emulate(FILE *fp)
{
        mem_t memory = NULL;
        uint32_t registers[8] = {0,0,0,0,0,0,0,0};
        memory = boot(fp, memory);
        fetch(&memory, registers);
}

/*
 * Purpose: Creates memory for the necessary data structures that make up the
 *          memory: sequence, uarray, and stack. Also initializes the memory
 *          and maps segment zero
 */
static mem_t boot(FILE *fp, mem_t memory)
{
        int size = 0;
        Seq_T input = Seq_new(100);
        
        /* the while loop will only end when the halt operation is used */
        while (1) {
                uint64_t word;
                /* getc reads in word 8 bytes at a time from input */
                uint32_t val = getc(fp);
                if ((int)val == EOF) break;
                
                word = Bitpack_newu(word, 8, 24, (uint64_t) val);
                word = Bitpack_newu(word, 8, 16, (uint64_t) getc(fp));
                word = Bitpack_newu(word, 8, 8, (uint64_t) getc(fp));
                word = Bitpack_newu(word, 8, 0, (uint64_t) getc(fp));
                uint32_t *value = malloc(sizeof(uint32_t));
                *value = word;

                Seq_addhi(input, value);
                size++;
        }
      
        memory = memory_new(size);
        
        /* frees the space allocated for each value */
        for (int i = 0; i < size; i++) {   
                uint32_t *val = (uint32_t *)Seq_remlo(input);
                word_store(&memory, 0, i, val); 
                free(val);
        }
        
        Seq_free(&input);
        return memory;
}

/*
 * Purpose: Responsible for updating the program counter and calling the
 *          function that determines the opcode and performs the necessary
 *          actions
 */
static void fetch(mem_t *memory, uint32_t *registers)
{
        uint32_t program_counter = 0;
        while (1) {
                int update_counter;
                update_counter = decode(memory, word_load(memory, 0,
                                        program_counter), registers);
                if (update_counter == -1)
                        program_counter++;
                /* the update counter will only be -2 if halt is called */
                else if (update_counter == -2)
                        return;
                else
                        /* if load program is called, the program */
                        /*counter will be given a new value */
                        program_counter = (uint32_t) update_counter;
        }
}

/*
 * Purpose: Uses bitpack to extract the opcode and necessary registers, also
 *          calls the necessary functions corresponding to the opcodes
 */
int decode(mem_t *memory, uint32_t word, uint32_t *registers)
{
        uint32_t opcode, ra, rb, rc, val;
        word = (uint64_t) word;
        
        /* unpacks the 32 bit instruction */
        opcode = Bitpack_getu(word, 4, 28);
        if ((int) opcode != 13) {
                ra = (uint32_t) Bitpack_getu(word, 3, 6);
                rb = (uint32_t) Bitpack_getu(word, 3, 3);
                rc = (uint32_t) Bitpack_getu(word, 3, 0);
        } else {
                ra = (uint32_t) Bitpack_getu(word, 3, 25);
                val = (uint32_t) Bitpack_getu(word, 25, 0);
        }
              
        /* calls a function according to the unpacked opcode */
        switch (opcode) {
                case 0 :
                        CMOV(ra, rb, rc, registers);
                        break;
                case 1 :
                        SLOAD(ra, rb, rc, memory, registers);
                        break;
                case 2 :
                        SSTORE(ra, rb, rc, memory, registers);
                        break;
                case 3 :
                        ADD(ra, rb, rc, registers);
                        break;
                case 4 :
                        MULT(ra, rb, rc, registers);
                        break;
                case 5 :
                        DIV(ra, rb, rc, registers);
                        break;
                case 6 :
                        NAND(ra, rb, rc, registers);
                        break;
                case 7 :
                        HALT(memory);
                        return -2;
                        break;
                case 8 :
                        MAP(rb, rc, memory, registers);
                        break;
                case 9 :
                        UNMAP(rc, memory, registers);
                        break;
                case 10 :
                        OUTPUT(rc, registers);
                        break;
                case 11 :
                        INPUT(rc, registers);
                        break;
                case 12 :
                        return LOADP(rb, rc, memory, registers);
                        break;
                case 13 :
                        LOADV(ra, val, registers);
                        break;
        }
               
        return -1;
}
