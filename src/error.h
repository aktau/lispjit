struct error {
    const char *msg; /* if not NULL, needs to be deallocated */
    const char *pos; /* position in the buffer */
};

#define ROK(_pos) ((struct error) { .msg = NULL, .pos = (_pos) })
#define RERR(_msg, _pos) ((struct error) { .msg = (_msg), .pos = (_pos) })
#define RERRNP(_msg) ((struct error) { .msg = (_msg), .pos = NULL })
