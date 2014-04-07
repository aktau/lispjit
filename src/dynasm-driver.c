/**
 * Driver file for DynASM-based JITs.
 *
 * This code is in the Public Domain. Original idea by Joshua Haberman.
 */

#include <assert.h>
#include <stdio.h>
#include <sys/mman.h>

#include "dynasm/dasm_proto.h"
#include "dynasm/dasm_x86.h"

void *jitcode(dasm_State **state);
void free_jitcode(void *code);

#include JIT

/* either succeeds or exists the program, you will get a pointer to a
 * callable function. */
void *jitcode(dasm_State **state) {
  /* optional sanity check */
  int status = dasm_checkstep(state, -1);
  assert(status == DASM_S_OK);

  size_t size;

  /* make sure we can link the code before allocating a code page */
  status = dasm_link(state, &size);
  assert(status == DASM_S_OK);

  /* allocate memory readable and writable so we can write the encoded
   * instructions there. Add sizeof(size_t) bytes to store the size of the
   * page we allocated. */
  char *mem = mmap(NULL,
          size + sizeof(size_t),
          PROT_READ | PROT_WRITE,
          MAP_ANON | MAP_PRIVATE,
          -1, 0);
  assert(mem != MAP_FAILED);

  /* store length at the beginning of the region, so we
   * can free it without additional context. */
  *(size_t*)mem = size;
  void *ret = mem + sizeof(size_t);

  status = dasm_encode(state, ret);
  assert(status == DASM_S_OK);

  /* adjust the memory permissions so it is executable
   * but no longer writable. For security reasons. */
  int success = mprotect(mem, size, PROT_EXEC | PROT_READ);
  assert(success == 0);

#ifndef NDEBUG
  /* write generated machine code to a temporary file for debugging */
  FILE *f = fopen("/tmp/jitcode", "wb");
  fwrite(ret, size, 1, f);
  fclose(f);
#endif

  return ret;
}

void free_jitcode(void *code) {
  void *mem = (char*)code - sizeof(size_t);
  int status = munmap(mem, *(size_t*)mem);
  assert(status == 0);
}
