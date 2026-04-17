#ifndef SMSH_PARSER
#define SMSH_PARSER

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#include "environ.h"
#include "utils.h"

/**
 * Takes a given string and performs smsh env var substitution
 * if possible, if any substituion fails it is replaced with
 * an empty string
 * 
 * @returns NULL if no transformation occurs
 */
char* evalExpr(smsh_env* env, const char* value);

/**
 * Pass a statement into execvp
 */
int exec_smsh(char** argv);

/**
 * Perform a look-ahead and create an array of strings to pass
 * to exec_smsh
 */
char** extractCommand(size_t* argc, char** argv, size_t* len, int i);

#endif