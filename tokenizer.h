#ifndef TOKENIZER
#define TOKENIZER

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void tokenize(char** argv, size_t argc, FILE* scriptFile);

#endif