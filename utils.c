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