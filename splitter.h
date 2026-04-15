#ifndef SMSH_SPLITTER
#define SMSH_SPLIITER

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct {
    char** strs;
    size_t count;
} strings;

/**
 *
 * @returns dynamically allocated object storing string collection
 */
strings* splitLine(char* line, size_t len);

void freeList(strings* obj);

#endif
