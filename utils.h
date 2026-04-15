#ifndef SMSH_UTILS
#define SMSH_UTILS

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

char* substr(const char *src, size_t start, size_t len);
int validName(const char* name);
char** extractTokens(const char* input, size_t* tokenCount);
/**
 * append a string to another
 */
char* append(char* dest, const char* src);

#endif