/*
 * um.c
 * by: Ambika Jayakumar and Danielle Zelin
 * date: 3/29/15
 * Assignment 6
 * Summary: contains the main function and runs the given program using the
 *          emulator
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "emulator.h"

int main(int argc, char *argv[])
{
        FILE *fp = fopen(argv[1], "r");
        if (fp == NULL || argc > 2 || argc == 1) {
                fprintf(stderr, "Input in incorrect format.\n");
                exit(EXIT_FAILURE);
        }
        emulate(fp);

        fclose(fp);
        return 0;
}