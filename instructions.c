/*
 * instructions.c
 * by: Ambika Jayakumar and Danielle Zelin
 * Date: 3/29/2015
 * Assignment 6
 * Summary: contains all instruction definitions
 *
 */

#include "instructions.h"

/*
 * Purpose: Moves contents of register B into register A
 */
void CMOVa(int A, int B, int C, uint32_t *registers)
{
        if (registers[C] != 0) {
                registers[A] = registers[B];
        }
}

/*
 * Purpose: Loads the contents of segment B with an offset of C into register A
 */
void SLOADa(int A, int B, int C, mem_t *memory, uint32_t *registers)
{
        registers[A] = word_load(memory, registers[B], registers[C]);
}

/*
 * Purpose: Stores the contents of register C into segment A with an offset of B
 */
void SSTOREa(int A, int B, int C, mem_t *memory, uint32_t *registers)
{
        word_store(memory, registers[A], registers[B], &(registers[C]));
}

/*
 * Purpose: Stores sum of the contents of registers B and C in register A
 */
void ADDa(int A, int B, int C, uint32_t *registers)
{
        registers[A] = registers[B] + registers[C];
}

/*
 * Purpose: Stores product of the contents of registers B and C in register A
 */
void MULTa(int A, int B, int C, uint32_t *registers)
{
        registers[A] = registers[B] * registers[C];
}

/*
 * Purpose: Stores the contents in register B divided by the contents of
 *          register C in register A
 */
void DIVa(int A, int B, int C, uint32_t *registers)
{
        registers[A] = registers[B] / registers[C];
}

/*
 * Purpose: Stores contents of B NAND C in register A
 */
void NANDa(int A, int B, int C, uint32_t *registers)
{
        registers[A] = ~(registers[B] & registers[C]);
}

/*
 * Purpose: Halts the program and frees memory
 */
void HALTa(mem_t *memory)
{
        memory_free(memory);
}
        
/*
 * Purpose: Calls necessary function to map memory
 */
void MAPa(int B, int C, mem_t *memory, uint32_t *registers)
{
        registers[B] = segment_map(memory, registers[C]);
}

/*
 * Purpose: Calls necessary function to unmap memory
 */
void UNMAPa(int C, mem_t *memory, uint32_t *registers)
{
        segment_unmap(memory, registers[C]);
}

/*
 * Purpose: Prints to standard output the ascii value corresponding to the
 *          contents of register C
 */
void OUTPUTa(int C, uint32_t *registers)
{
        assert((int)registers[C] >= 0 && (int)registers[C] <= 255);
        printf("%c", registers[C]);
}

/*
 * Purpose: Stores the input from standard input into register C
 */
void INPUTa(int C, uint32_t *registers)
{
        registers[C] = (uint32_t) getc(stdin);
}

/*
 * Purpose: Handles loops and jumps and returns the function pointer which is 
 *          stored in register C
 */
uint32_t LOADPa(int B, int C, mem_t *memory, uint32_t *registers)
{
        if (registers[B] != 0) {
                UArray_T duplicate = segment_load(memory, registers[B]);
                segment_store(memory, 0, duplicate);
        }
        return registers[C];
}

/*
 * Purpose: Loads the given value in register A
 */
void LOADVa(int A, int val, uint32_t *registers)
{
        registers[A] = (uint32_t) val;
}