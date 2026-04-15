#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "splitter.h"

#define INFO "[\033[33m*\033[0m]"
#define OK "[\033[32m+\033[0m]"

#define TEST_COUNT 7
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
    printf("%s Testing Splitter\n", INFO);

    {
        char s[] = "hi";
        strings* arglist = splitLine(s, strlen(s));
        assert(arglist != NULL);

        assert(arglist->count == 1);
        assert( strcmp("hi", arglist->strs[0]) == 0 );

        freeList(arglist);
        showProgress();
    }

    {
        char s[] = "hi programmer";
        strings* arglist = splitLine(s, strlen(s));
        assert(arglist != NULL);

        assert(arglist->count == 2);
        assert( strcmp("hi", arglist->strs[0]) == 0 );
        assert( strcmp("programmer", arglist->strs[1]) == 0 );

        freeList(arglist);
        showProgress();
    }

    {
        char s[] = "hi pro 12 mmer";
        strings* arglist = splitLine(s, strlen(s));
        assert(arglist != NULL);

        assert(arglist->count == 4);
        assert( strcmp("hi", arglist->strs[0]) == 0 );
        assert( strcmp("pro", arglist->strs[1]) == 0 );
        assert( strcmp("12", arglist->strs[2]) == 0 );
        assert( strcmp("mmer", arglist->strs[3]) == 0 );

        freeList(arglist);
        showProgress();
    }

    {
        char s[] = "trust; me";
        strings* arglist = splitLine(s, strlen(s));
        assert(arglist != NULL);

        assert(arglist->count == 3);
        assert( strcmp("trust", arglist->strs[0]) == 0 );
        assert( strcmp(";", arglist->strs[1]) == 0 );
        assert( strcmp("me", arglist->strs[2]) == 0 );

        freeList(arglist);
        showProgress();
    }

    {
        char s[] = "trust;me";
        strings* arglist = splitLine(s, strlen(s));
        assert(arglist != NULL);

        assert(arglist->count == 3);
        assert( strcmp("trust", arglist->strs[0]) == 0 );
        assert( strcmp(";", arglist->strs[1]) == 0 );
        assert( strcmp("me", arglist->strs[2]) == 0 );

        freeList(arglist);
        showProgress();
    }

    {
        char s[] = "'trust;' me";
        strings* arglist = splitLine(s, strlen(s));
        assert(arglist != NULL);

        assert(arglist->count == 2);
        assert( strcmp("'trust;'", arglist->strs[0]) == 0 );
        assert( strcmp("me", arglist->strs[1]) == 0 );

        freeList(arglist);
        showProgress();
    }

    {
        char s[] = "\"trust;\" me";
        strings* arglist = splitLine(s, strlen(s));
        assert(arglist != NULL);

        assert(arglist->count == 2);
        assert( strcmp("\"trust;\"", arglist->strs[0]) == 0 );
        assert( strcmp("me", arglist->strs[1]) == 0 );

        freeList(arglist);
        showProgress();
    }

    printf("%s Test Passed!\n", OK);

    return 0;
}