#include "splitter.h"

void freeList(strings* obj) {
    if (obj == NULL) return;

    // free entries
    for (size_t i = 0; i < obj->count; ++i)
        free(obj->strs[i]);
    // free main-structure
    free(obj->strs);
    free(obj);
}

static void appendChar(char** line, char c) {
    size_t oLen = (*line != NULL) ? strlen(*line) : 0;
    char *oStr = *line;

    *line = malloc(oLen + 2);

    for (size_t i = 0; i < oLen; i++) {
        (*line)[i] = oStr[i];
    }
    if (oStr) free(oStr);

    (*line)[oLen] = c;
    (*line)[oLen + 1] = '\0';
}

static void appendString(strings* pObj, char* nStr) {
    if (!pObj || !nStr) return;

    char** tmp = pObj->strs;
    pObj->strs = malloc( (pObj->count + 1) * sizeof(char*) );

    // move the old data back into the object
    for (size_t i = 0; i < pObj->count; ++i) {
        pObj->strs[i] = tmp[i];
    }
    if (tmp) free(tmp);

    // insert the new string
    pObj->strs[pObj->count++] = nStr;
}

strings* splitLine(char* line, size_t len) {
    if (len == 0) return NULL;

    strings* obj = malloc(sizeof(strings));
    if (obj == NULL) {
        perror("malloc");
        return NULL;
    }

    // initialize new object members
    obj->strs = NULL;
    obj->count = 0;

    char* subString = NULL;

    int isQuoted = 0;
    char quoteChar = 0;

    for (size_t i = 0; i < len; ++i) {
        char c = line[i];

        if (!isQuoted && c == ' ') {
            appendString(obj, subString);
            subString = NULL;
            continue;
        }

        // push two substrings (semi colon is its own token)
        if (!isQuoted && c == ';') {
            appendString(obj, subString);
            subString = NULL;

            char* token = NULL;
            appendChar(&token, ';');
            appendString(obj, token);

            continue;
        }

        // handle quote open
        if (!isQuoted && (c == '"' || c == '\'')) {
            isQuoted  = 1;
            quoteChar = c;
            appendChar(&subString, c);
            continue;
        }

        // handle quote close
        if (isQuoted && c == quoteChar && (i == 0 || line[i-1] != '\\')) {
            isQuoted = 0;
            appendChar(&subString, c);
            continue;
        }

        appendChar(&subString, c);
    }

    // trailing character from last-line of a file
    if (line[len] != '\n' && line[len] != '\0') {
        appendChar(&subString, line[len]);
    }

    appendString(obj, subString);

    return obj;
}