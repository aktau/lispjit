#include <inttypes.h>
#include <assert.h>

#define REG_NONE (-1)

#define REG_NUM_64_OFFSET 0
#define REG_NUM_64 16
#define REG_NUM_128_OFFSET (REG_NUM_64)
#define REG_NUM_128 16

#define REG_IDX_RSP (1 << 4)

/* bitset that represents the registers currently in use */
typedef struct registers {
    /* 64-bit registers */
    uint16_t r64;
    const uint16_t r64_total;

    /* 128-bit (vector) registers */
    uint16_t r128;
    const uint16_t r128_total;
} reg_state_t;

#define X64_REGS_INIT (reg_state_t) { \
    .r64 = REG_IDX_RSP, \
    .r64_total = 16, \
    .r128 = 0, \
    .r128_total = 16  \
}

/* allocate a register from the register set, choose the first free one */
static inline int reg_alloc(uint16_t *regs, uint16_t total) {
    int idx = 0;
    uint16_t cur = 1;

    for (; (*regs & cur) && idx < total; cur <<= 1, ++idx);

    if (idx >= total) {
        return REG_NONE;
    }

    /* occupy the register */
    *regs |= cur;

    return idx;
}

#define reg_get_128(rst) reg_alloc(&rst.r128, rst.r128_total)
#define reg_get_64(rst) reg_alloc(&rst.r64, rst.r64_total)

/* free a register (identified by index) */
static inline void reg_dealloc(uint16_t *regs, int idx) {
    assert(idx >= 0 && ((size_t) idx < sizeof(*regs) * 8) && "idx is not allowed");

    uint16_t cur = 1U << idx;

    assert((*regs & cur) && "reg wasn't allocated to begin with");

    *regs &= ~cur;
}

#define reg_put_128(rst, idx) reg_dealloc(&rst.r128, idx)
#define reg_put_64(rst, idx) reg_dealloc(&rst.r64, idx)
