/*
 * instructions.h
 * by: Ambika Jayakumar and Danielle Zelin
 * date: 3/29/15
 * Assignment 6
 * Summary: header file for the instructions implementation
 *
 */

#include "assert.h"
#include "uarray.h"
#include "memory.h"

void CMOV(int A, int B, int C, uint32_t *registers);
void SLOAD(int A, int B, int C, mem_t *memory, uint32_t *registers);
void SSTORE(int A, int B, int C, mem_t *memory, uint32_t *registers);
void ADD(int A, int B, int C, uint32_t *registers);
void MULT(int A, int B, int C, uint32_t *registers);
void DIV(int A, int B, int C, uint32_t *registers);
void NAND(int A, int B, int C, uint32_t *registers);
void HALT(mem_t *memory);
void MAP(int B, int C, mem_t *memory, uint32_t *registers);
void UNMAP(int C, mem_t *memory, uint32_t *registers);
void OUTPUT(int C, uint32_t *registers);
void INPUT(int C, uint32_t *registers);
uint32_t LOADP(int B, int C, mem_t *memory, uint32_t *registers);
void LOADV(int A, int val, uint32_t *registers);