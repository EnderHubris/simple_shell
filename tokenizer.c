#include "tokenizer.h"
#include "splitter.h"
#include "environ.h"
#include "parser.h"
#include "evaluator.h"
#include "utils.h"

/*
    shell-tokens | "exit", "cd", "read", ...
    -------------+-----------------------------
    control-flow | "if", "elif", "else", "then", ...
*/

const char* TOKENS[] = {
    "if", "fi", "elif", "else", "then", ";", "\n",
    "exit", "cd", "read", "smsh_env", "echo"
};
const size_t TOKEN_COUNT = sizeof(TOKENS) / sizeof(TOKENS[0]);

smsh_env SMSH_ENV = { 0 };

/**
 * Get the value of the argument after index i
 */
static char* get_value(char** argv, size_t argc, size_t i) {
    char* val = (i+1 < argc) ? argv[i+1] : NULL;

    // check that varName is not a token
    for (size_t i = 0; val && i < TOKEN_COUNT; ++i) {
        // if varName is a token run cd with no arg
        if (strcmp(val, TOKENS[i]) == 0) {
            val = NULL;
            break;
        }
    }
    return val;
}

void tokenize(char** argv, size_t argc, FILE* scriptFile) {
    // printf("[TOKENIZE] ");
    // for (size_t i = 0; i < argc; ++i)
    //     printf("%s ", argv[i]);
    // if (argc > 0) printf("\n");

    for (size_t i = 0; i < argc; ++i) {
        const char* token = argv[i];

        if (strcmp("exit", token) == 0) {
            int exitCode = 0;
            
            if (i+1 < argc) {
                char* endp;
                exitCode = strtol(argv[i+1], &endp, 10);
                if ( (endp == argv[i+1]) || ( *endp != '\0' )) {
                    // name token => eval token to get numeric value
                }
            }

            // quit smsh program
            exit(exitCode);
        } else if (strcmp("cd", token) == 0) {
            if (strcmp(argv[i], "cd") == 0) {
                char* newDir = get_value(argv, argc, i);
                // exec chdir
                if (newDir == NULL) {
                    // move to user home directory
                    chdir(getenv("HOME"));
                } else {
                    ++i;
                    if (chdir(newDir) == -1)
                        perror("chdir");
                }
            }
            continue;
        } else if (strcmp("read", token) == 0) {
            char* varName = get_value(argv, argc, i);
            if (validName(varName) && varName) {
                ++i;
                smshRead(&SMSH_ENV, varName);
            } else {
                printf("Missing read target\n");
            }

            continue;
        } else if (strcmp("smsh_env", token) == 0) {
            printEnviron(&SMSH_ENV);
            continue;
        } else if (strcmp("echo", token) == 0) {
            // print all next tokens and stop at the token
            // with a semi-colon
            size_t k = i+1;
            for (; k < argc; ++k) {
                char* echo_token = argv[k];

                if (
                    (strcmp(";", echo_token) == 0) ||
                    (strcmp("\n", echo_token) == 0)
                ) {
                    i+=k+1;
                    break;
                }

                char* result = evalExpr(&SMSH_ENV, echo_token);
                printf("%s ", result ? result : echo_token);
                free(result);
            }
            printf("\n");
            i+=k;
            continue;
        }

        if (strcmp("if", token) == 0) {
            // resolve the condition
            int execBlock = resolveCondition(&SMSH_ENV, argv, argc, i);
            executeFlow(&SMSH_ENV, scriptFile, execBlock);
            return; // exit this method and read the next line
        } else if (
            strcmp(";", token) == 0 ||
            strcmp("\n", token) == 0 ||
            strcmp("fi", token) == 0 ||
            strcmp("then", token) == 0
        ) {
            continue;
        }

        // check for assignment syntax
        if ( (i+2 < argc) && (strcmp(argv[i+1], "=") == 0) ) {
            char* varValue = get_value(argv, argc, i+1);
            if (varValue) {
                i += 2;
                char* result = evalExpr(&SMSH_ENV, varValue);
                addVariable(&SMSH_ENV, token, result ? result : varValue);
                free(result);
                continue;
            }
        }

        size_t cmdlet_len;
        char** cmdlet = extractCommand(&argc, argv, &cmdlet_len, i);
        exec_smsh(cmdlet);

        // free cmdlet array
        for (size_t k = 0; cmdlet && k < cmdlet_len; ++k)
            free(cmdlet[k]);
        free(cmdlet);

        // skip consumed tokens
        i+=cmdlet_len;
    }
}