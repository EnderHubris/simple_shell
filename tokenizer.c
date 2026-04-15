#include "tokenizer.h"

static void exitShell(int exitCode) { exit(exitCode); };

/*
    shell-tokens | "exit", "cd", "read"
    -------------+-----------------------------
    control-flow | "if", "elif", "else", "then"
*/

const char* TOKENS[] = {
    "if", "elif", "else", "then", ";", "\n",
    "exit", "cd", "read"
};
const size_t TOKEN_COUNT = sizeof(TOKENS) / sizeof(TOKENS[0]);

void tokenize(char** argv, size_t argc) {
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
            exitShell(exitCode);
        } else if (strcmp("cd", token) == 0) {
            if (strcmp(argv[i], "cd") == 0) {
                char* newDir = (i+1 < argc) ? argv[i+1] : NULL;

                // validate the chdir argument
                for (size_t k = 0; newDir && k < TOKEN_COUNT; ++k) {
                    // if newDir is a token run cd with no arg
                    if (strcmp(newDir, TOKENS[k]) == 0) {
                        newDir = NULL;
                        break;
                    }
                }

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
            continue;
        }

        if (strcmp("if", token) == 0) {
            continue;
        } else if (strcmp("elif", token) == 0) {
            continue;
        } else if (strcmp("else", token) == 0) {
            continue;
        } else if (strcmp("then", token) == 0) {
            continue;
        }

        printf("%s is unrecognized\n", token);
    }
}