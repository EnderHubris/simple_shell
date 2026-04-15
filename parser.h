#ifndef SMSH_PARSER
#define SMSH_PARSER

#include <stdio.h>
#include <stdlib.h>
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

#endif