/*
 * emulator.h
 * by: Ambika Jayakumar and Danielle Zelin
 * date: 3/29/15
 * Assignment 6
 * Summary: header file for the emulator implementation
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "assert.h"
#include "uarray.h"
#include "seq.h"
#include "bitpack.h"
#include "instructions.h"

extern void emulate(FILE *fp);