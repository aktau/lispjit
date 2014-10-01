#include <string.h>

#include "str.h"

typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_INV,
    OP_DOT,
    OP_CROSS,
    OP_SQRT,
    OP_RSQRT,
    OP_AND,
    OP_NAND,
    OP_OR,
    OP_XOR,
    OP_NEG,
    OP_LT,
    OP_LE,
    OP_GT,
    OP_GE,
    OP_EQ,
    OP_NEQ,
    OP_MIN,
    OP_MAX,
    OP_UNKNOWN,
} operation_t;

#define TOKMATCH(src, srclen, match) \
    ((srclen) == strlen(match)) && (memcmp((src), (match), strlen(match)) == 0)

/* decode an indentifier into an operation, if possible. Implemented in the
 * most inefficient way imaginable. */
operation_t decode(const char *str) {
    printf("going to isolate from %s\n", str);
    /* isolate the operator */
    const char *end = skiptoken(str);
    if (!end) {
        return OP_UNKNOWN;
    }
    size_t toklen = end - str;

    printf("isolated %.*s (start = %p, end = %p, len = %zu)\n",
            (int) toklen, str, str, end, end - str);

    if (TOKMATCH(str, toklen, "+")) return OP_ADD;
    if (TOKMATCH(str, toklen, "-")) return OP_SUB;
    if (TOKMATCH(str, toklen, "*")) return OP_MUL;
    if (TOKMATCH(str, toklen, "/")) return OP_DIV;
    if (TOKMATCH(str, toklen, "inv")) return OP_INV;
    if (TOKMATCH(str, toklen, ".")) return OP_DOT;
    if (TOKMATCH(str, toklen, "x")) return OP_CROSS;
    if (TOKMATCH(str, toklen, "sqrt")) return OP_SQRT;
    if (TOKMATCH(str, toklen, "rsqrt")) return OP_RSQRT;
    if (TOKMATCH(str, toklen, "&")) return OP_AND;
    if (TOKMATCH(str, toklen, "&~")) return OP_NAND;
    if (TOKMATCH(str, toklen, "|")) return OP_OR;
    if (TOKMATCH(str, toklen, "^")) return OP_XOR;
    if (TOKMATCH(str, toklen, "<")) return OP_LT;
    if (TOKMATCH(str, toklen, "<=")) return OP_LE;
    if (TOKMATCH(str, toklen, ">")) return OP_GT;
    if (TOKMATCH(str, toklen, ">=")) return OP_GE;
    if (TOKMATCH(str, toklen, "=")) return OP_EQ;
    if (TOKMATCH(str, toklen, "!=")) return OP_NEQ;
    if (TOKMATCH(str, toklen, "min")) return OP_MIN;
    if (TOKMATCH(str, toklen, "max")) return OP_MAX;

    /* TODO: improve parser so it can disern unary and n-ary minus so this
     * isn't necessary */
    if (TOKMATCH(str, toklen, "neg")) return OP_NEG;

    return OP_UNKNOWN;
}
