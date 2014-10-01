#include <string.h>

#define WHITESPACE " \t\n\r"

/* skips ahead until the start of the next token (assumes that `s` is
 * currenty not inside of a token) */
static inline char *skipwhite(const char *s) {
    char *t = (char *) s;

    for (; *t && strchr(WHITESPACE, *t); ++t);

    return *t ? t : NULL;
}

/* skips ahead past the end of the current token (assumes `s` is the start
 * of a token) */
static inline char *skiptoken(const char *s) {
    return strpbrk(s, WHITESPACE);
}
