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

void CMOVa(int A, int B, int C, uint32_t *registers);
void SLOADa(int A, int B, int C, mem_t *memory, uint32_t *registers);
void SSTOREa(int A, int B, int C, mem_t *memory, uint32_t *registers);
void ADDa(int A, int B, int C, uint32_t *registers);
void MULTa(int A, int B, int C, uint32_t *registers);
void DIVa(int A, int B, int C, uint32_t *registers);
void NANDa(int A, int B, int C, uint32_t *registers);
void HALTa(mem_t *memory);
void MAPa(int B, int C, mem_t *memory, uint32_t *registers);
void UNMAPa(int C, mem_t *memory, uint32_t *registers);
void OUTPUTa(int C, uint32_t *registers);
void INPUTa(int C, uint32_t *registers);
uint32_t LOADPa(int B, int C, mem_t *memory, uint32_t *registers);
void LOADVa(int A, int val, uint32_t *registers);