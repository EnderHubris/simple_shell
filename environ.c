#include "environ.h"

static int updateVariable(smsh_env* env, const char* name, const char* val) {
    smsh_env_var* v = NULL;
    if ( (v = getVariable(env, name)) ) {
        // update type
        char *endp; strtol(val, &endp, 10);
        int isLiteral = (endp == val) || (*endp != '\0');

        free(v->value); // free original value
        v->value = strdup(val);

        v->v_type = ( isLiteral ) ? (
            LIT_STRING
        ) : (
            NUMERIC
        );

        return 1;
    }
    return 0;
}

void addVariable(smsh_env* env, const char* name, const char* val) {
    // check if the name already exists and update it if so
    if (updateVariable(env, name, val)) return;

    smsh_env_var** tmp = env->smsh_env_var;
    env->smsh_env_var = malloc(
        (env->var_c + 1) * sizeof(smsh_env_var*)
    );

    if (!env->smsh_env_var) {
        // do not want to lose env
        perror("malloc");
        env->smsh_env_var = tmp;
        return;
    }

    // determine the env var type
    char *endp; strtol(val, &endp, 10);
    int isLiteral = (endp == val) || (*endp != '\0');

    // create new variable and define its attributes
    smsh_env_var* v = malloc(sizeof(smsh_env_var));
    v->name = strdup(name); // copy the value
    v->value = strdup(val);
    v->v_type = ( isLiteral ) ? (
        LIT_STRING
    ) : (
        NUMERIC
    );

    // move the old data back into the main-structure
    for (size_t i = 0; i < env->var_c; ++i) {
        env->smsh_env_var[i] = tmp[i];
    }
    free(tmp);

    // append to env
    env->smsh_env_var[env->var_c++] = v;
}

void smshRead(smsh_env* env, const char* name) {
    size_t len;
    ssize_t bytesRead;
    char* varValue = NULL;

    printf("%s = ", name);
    while ( (bytesRead = getline(&varValue, &len, stdin)) == -1) {
        // user must enter a value
        printf("%s = ", name);
    }

    // remove new-line from value
    int i = 0;
    while (1) {
        // null-terminating check if just for bound read safety
        if (varValue[i] == '\n' || varValue[i] == '\0') {
            varValue[i] = '\0';
            break;
        }
        ++i;
    }
    
    addVariable(env, name, varValue);
}

void printEnviron(smsh_env* env) {
    for (size_t i = 0; i < env->var_c; ++i) {
        smsh_env_var* v = env->smsh_env_var[i];
        if (v) printf("%s=%s\n", v->name, v->value);
    }
}
smsh_env_var* getVariable(smsh_env* env, const char* name) {
    for (size_t i = 0; i < env->var_c; ++i) {
        smsh_env_var* v = env->smsh_env_var[i];
        if ( strcmp(v->name, name) == 0 )
            return v;
    }
    return NULL;
}

void freeEnviron(smsh_env* env) {
    if (env == NULL) return;
    
    for (size_t i = 0; i < env->var_c; ++i) {
        if (!env->smsh_env_var[i]) continue;
        free(env->smsh_env_var[i]->name);
        free(env->smsh_env_var[i]->value);
        env->smsh_env_var[i] = NULL;
    }

    free(env);
    env = NULL;
}