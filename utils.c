#include "utils.h"

char* substr(const char *src, size_t start, size_t len) {
    if (src == NULL) return NULL;
    size_t src_len = strlen(src);

    // clamp len if it overshoots the string
    if (start >= src_len) return NULL;
    if (start + len > src_len) len = src_len - start;

    char *sub = malloc(len + 1);
    if (!sub) return NULL;

    memcpy(sub, src + start, len);
    sub[len] = '\0';
    return sub;   // caller must free()
}

int validName(const char* name) {
    if (name == NULL || *name == '\0') return 0;

    // first char must be letter or underscore
    if (!isalpha(*name) && *name != '_') return 0;

    // remaining chars can be letters, digits, or underscores
    for (const char *p = name + 1; *p != '\0'; p++) {
        if (!isalnum(*p) && *p != '_') return 0;
    }

    return 1;
}

/**
 * Splits a string based on tokens and returns a list of token strings
 */
char** extractTokens(const char* input, size_t* tokenCount) {
    // tracks the true number of strings we extracted
    // as we may return a char** with empty pointer indices
    *tokenCount = 0;

    // pre-allocate more room than possibly needed
    // to reduce reallocation calls
    size_t cap = 8;
    char** tokens = malloc(cap * sizeof(char *));

    const char* p = input;

    while (*p != '\0') {
        const char *start = p;

        if (*p == '$') {
            // consume '$' + valid var name chars
            p++;
            while (isalnum(*p) || *p == '_') p++;
        } else {
            // consume until next '$' or end
            while (*p != '\0' && *p != '$') p++;
        }

        // copy the token [start, p)
        size_t len = p - start;
        char *tok = malloc(len + 1);
        memcpy(tok, start, len);
        tok[len] = '\0';

        // if we need more room grow the array
        // using realloc for convinence
        if (*tokenCount >= cap) {
            cap *= 2;
            tokens = realloc(tokens, cap * sizeof(char *));
        }
        tokens[(*tokenCount)++] = tok;
    }

    tokens[*tokenCount] = NULL;
    return tokens;
}

char* append(char* dest, const char* src) {
    if (src == NULL) return dest;

    size_t dLen = dest ? strlen(dest) : 0;
    size_t sLen = strlen(src);

    char *result = malloc(dLen + sLen + 1);

    if (dest) memcpy(result, dest, dLen);
    memcpy(result + dLen, src, sLen);
    result[dLen + sLen] = '\0';

    free(dest);
    return result;
}