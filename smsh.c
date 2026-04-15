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

int main() {
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
