#include "evaluator.h"

#include "tokenizer.h"
#include "parser.h"
#include "splitter.h"

int resolveCondition(smsh_env* env, char** argv, size_t argc, size_t i) {
    if (!argc) return 0;
    if (!env) return 0;

    char** condition = NULL;
    // extract condition
    {
        // consume the if-token
        size_t start = i + 1;
        size_t k = start;
        for (; k < argc; ++k) {
            if (strcmp(argv[k], "then") == 0 || strcmp(argv[k], "\n") == 0)
                break;
        }

        size_t len = k - start;
        condition = malloc((len + 1) * sizeof(char *));
        condition[len] = NULL;

        for (size_t j = 0; j < len; ++j)
            condition[j] = strdup(argv[start + j]);
    }

    // check for env var sub within the condition
    for (int i = 0; condition[i] != NULL; ++i) {
        char* result = evalExpr(env, condition[i]);
        if (result) {
            // replace old string with new string
            free(condition[i]);
            condition[i] = strdup(result);
        }
        free(result);
    }

    // assume the condition string forms a shell command
    // rc of 0 means success otherwise means a fail
    return exec_smsh(condition) == 0;
}

void executeFlow(smsh_env* env, FILE* scriptFile, int readBlock) {
    if (!env || !scriptFile) return;
    
    char buffer[BUFFER_SIZE];

    if (readBlock) {
        while (fgets(buffer, sizeof(buffer), scriptFile) != NULL) {
            // ignore line commands
            if (buffer[0] == '#') continue;
            
            if ( strcmp(buffer, "then") == 0 ) continue;
            if ( strcmp(buffer, "fi") == 0 ) break; // end of if
            
            strings* arglist = splitLine(buffer, strlen(buffer)-1);
            if ( arglist != NULL ) {
                tokenize(arglist->strs, arglist->count, scriptFile);
                freeList(arglist);
            }
        }
    } else {
        // ignore block
        size_t depth = 1;

        // read the file line by line
        while (fgets(buffer, sizeof(buffer), scriptFile) != NULL) {
            if (buffer[0] == '#') continue;

            strings* arglist = splitLine(buffer, strlen(buffer)-1);
            if ( arglist != NULL ) {
                if ( arglist->count > 0 ) {
                    if ( strcmp(arglist->strs[0], "if") == 0 )
                        ++depth;
                    else if ( strcmp(arglist->strs[0], "fi") == 0 )
                        --depth;
                }
                freeList(arglist);
            }

            if (depth == 0) break;
        }
    }
}