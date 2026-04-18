#include "smsh.h"
#include "environ.h"
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

void shellEval(char** argv, size_t argc, FILE* scriptFile) {
    // interpret tokens
    tokenize(argv, argc, scriptFile);
}

static void smsh_cancel() {
    // reprompt shell
    printf("\n");
    showPrompt();
}

int main(int argc, char** argv) {
    FILE* file = NULL;
    if (argc == 2) {
        // read from command file
        file = fopen(argv[1], "r");

        if (file == NULL) {
            printf("Error opening file\n");
            return 1;
        }

        char buffer[BUFFER_SIZE];

        // read the file line by line
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            // replace the first occurrance of new-line with null-terminating
            buffer[strcspn(buffer, "\n")] = '\0';

            // ignore line commands
            if (buffer[0] == '#') continue;

            strings* arglist = splitLine(buffer, strlen(buffer));
            if ( arglist != NULL ) {
                shellEval(arglist->strs, arglist->count, file);
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
            // replace the first occurrance of new-line with null-terminating
            prompt[strcspn(prompt, "\n")] = '\0';
            
            strings* arglist = splitLine(prompt, len);
            if ( arglist != NULL ) {
                shellEval(arglist->strs, arglist->count, file);
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
