#ifndef SMSH_ENVIRONMENT
#define SMSH_ENVIRONMENT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    LIT_STRING,
    NUMERIC
} smsh_ev_t;

typedef struct {
    char* name;
    smsh_ev_t v_type;
    char* value;
} smsh_env_var;

typedef struct {
    // collection of local env vars
    smsh_env_var** smsh_env_var;
    // number of variables in local env
    size_t var_c;
} smsh_env;

/**
 * Read from stdin and set its value to the given name
 * in the local env
 */
void smshRead(smsh_env* env, const char* name);

void addVariable(smsh_env* env, const char* name, const char* val);
smsh_env_var* getVariable(smsh_env* env, const char* name);

void printEnviron(smsh_env* env);

void freeEnviron(smsh_env* env);

#endif