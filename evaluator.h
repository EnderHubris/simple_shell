#ifndef SMSH_EVALUATOR
#define SMSH_EVALUATOR

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "environ.h"

int resolveCondition(smsh_env* env, char** argv, size_t argc, size_t i);
void executeFlow(smsh_env* env, FILE* scriptFile, int readBlock);

#endif