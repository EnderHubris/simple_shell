#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "parser.h"
#include "environ.h"

#define INFO "[\033[33m*\033[0m]"
#define OK "[\033[32m+\033[0m]"

#define TEST_COUNT 5
static void showProgress() {
    static int n = 1;
    
    // delete the previous printed progress line
    printf("\r\033\b\033[A\r\033[2K");
    // print current progress
    printf("Test-Case %d/%d\n", n, TEST_COUNT);

    fflush(stdout);
    ++n;
}

int main() {
    printf("%s Testing Parser\n\n", INFO);

    smsh_env TEST_ENV = { 0 };
    addVariable(&TEST_ENV, "frost", "24");    // numeric
    addVariable(&TEST_ENV, "JREF", "e7c12a"); // string

    {
        char* result = evalExpr(&TEST_ENV, "symbol");

        assert( strcmp(result, "symbol") == 0 );

        free(result);
        showProgress();
    }

    {
        char* result = evalExpr(&TEST_ENV, "$frost/flake");

        assert( strcmp(result, "24/flake") == 0 );

        free(result);
        showProgress();
    }

    {
        char* result = evalExpr(&TEST_ENV, "$frost/flake/$JREF");

        assert( strcmp(result, "24/flake/e7c12a") == 0 );

        free(result);
        showProgress();
    }

    {
        // frost is not substituted so result returns NULL
        char* result = evalExpr(&TEST_ENV, "'$frost'");

        assert( result == NULL );

        free(result);
        showProgress();
    }

    {
        char* result = evalExpr(&TEST_ENV, "\"$frost\"");

        assert( strcmp(result, "24") == 0 );

        free(result);
        showProgress();
    }

    printf("%s Test Passed!\n", OK);

    return 0;
}