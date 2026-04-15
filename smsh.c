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

/**
 * Pass a statement into execvp
 */
static void exec_smsh(char** argv) {
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

void shellEval(char** argv, size_t argc) {
    for (size_t i = 0; i < argc; ++i) {
        printf("%s ", argv[i]);
    }
    if (argc > 0) printf("\n");

    // interpret tokens
    tokenize(argv, argc);
}

int main() {
    char* prompt = NULL;
    ssize_t len;
    
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
