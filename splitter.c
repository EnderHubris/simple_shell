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

    // replace new-line with \0
    line[len+1] = '\0';

    for (size_t i = 0; i < len; ++i) {
        if (line[i] == ' ') {
            if (subString != NULL) {
                appendString(obj, subString);
                subString = NULL;
            }
            continue;
        }
        appendChar(&subString, line[i]);
    }
    appendString(obj, subString);

    return obj;
}
