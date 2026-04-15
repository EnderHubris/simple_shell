#include "parser.h"

void exec_smsh(char** argv) {
    // create a child fork and run system commands
    // via execvp
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
    } else if (pid == 0) {
        // flushes stdout so we can
        // see the child output
        fflush(stdout);

        // child process
        if (execvp(argv[0], argv) == -1) {
            perror("execvp");
            _exit(1);
        }
    } else {
        // parent proc needs to wait for child to finish
        int status;
        waitpid(pid, &status, 0);
    }
}

char* evalExpr(smsh_env* env, const char* value) {
    // test if we need to resolve variables
    if (value[0] != '\'') {
        char* stripped_value = NULL;

        if (value[0] == '"') {
            // consume the double-quotes
            stripped_value = substr(value, 1, strlen(value) - 2);
        }
        
        size_t count;
        const char* str = stripped_value ? stripped_value : value;
        char** tokens = extractTokens(str, &count);

        // free potential substr result
        free(stripped_value);

        char* result = NULL;
        // rebuild expression via env var substitution
        for (size_t i = 0; i < count; ++i) {
            char* name = tokens[i];
            if (name[0] == '$') {
                name = substr(name, 1, sizeof(name));

                smsh_env_var* v = getVariable(env, name);
                char* val = v ? v->value : NULL;

                free(name);
                result = append(result, val ? val : "");
                continue;
            }
            result = append(result, name);
        }

        // result from extractTokens needs free'd
        for (size_t i = 0; i < count; ++i)
            free(tokens[i]);
        free(tokens);

        return result;
    } else {
        return NULL;
    }
}