#include "smsh.h"
#include "tokenizer.h"

void showPrompt() {
    char cwd[PATH_MAX];

    // uses /etc/passwd to get the username by grabbing
    // the pwd value based on the uid
    struct passwd* pw = getpwuid(getuid());
    const char* username = pw ? pw->pw_name : "unknown";

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        // fallback prompt
        perror("getcwd");
        printf("%s%s%s:%s?%s$ ",
            USER_COLOR,
            username,
            DEFAULT_COLOR,
            PATH_COLOR,
            DEFAULT_COLOR
        );
        return;
    }

    printf("%s%s%s:%s%s%s$ ",
        USER_COLOR,
        username,
        DEFAULT_COLOR,
        PATH_COLOR,
        cwd,
        DEFAULT_COLOR
    );

    // flush ensures we bush the stdout buffer
    // to the screen before input is requested
    fflush(stdout);
}

ssize_t getCmd(char** input) {
    size_t len;
    return getline(&(*input), &len, stdin);
}

void shellEval(char** argv, size_t argc) {
    /*
    for (size_t i = 0; i < argc; ++i) {
        printf("%s ", argv[i]);
    }
    if (argc > 0) printf("\n");
    */

    // interpret tokens
    tokenize(argv, argc);
}

static void smsh_cancel() {
    // reprompt shell
    printf("\n");
    showPrompt();
}

int main(int argc, char** argv) {
    if (argc == 2) {
        // read from command file
        FILE *file = fopen(argv[1], "r");

        if (file == NULL) {
            printf("Error opening file\n");
            return 1;
        }

        size_t BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];

        // read the file line by line
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            // ignore line commands
            if (buffer[0] == '#') continue;

            strings* arglist = splitLine(buffer, strlen(buffer)-1);
            if ( arglist != NULL ) {
                shellEval(arglist->strs, arglist->count);
                freeList(arglist);
            }
        }

        fclose(file);
    } else {
        char* prompt = NULL;
        ssize_t len;
    
        // hijack ctrl+c
        signal(SIGINT, smsh_cancel);
        
        showPrompt();
    
        // len-1 is to ignore the new-line char at the end
        // of the string
        while ( (len = getCmd(&prompt)) != -1 ) {
            strings* arglist = splitLine(prompt, len-1);
            if ( arglist != NULL ) {
                shellEval(arglist->strs, arglist->count);
                freeList(arglist);
            }
            showPrompt();
        }
    
        if (prompt) {
            free(prompt);
            prompt = NULL;
        }
    }

    return 0;
}
