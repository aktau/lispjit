CC ?= clang

STD := -std=c11 -pedantic -fstrict-aliasing
SECURITY := -D_THREAD_SAFE -D_FORTIFY_SOURCE=2
WARN := -Wextra -Wcast-align -Wcast-qual \
	-Wpointer-arith -Waggregate-return -Wunreachable-code \
	-Wfloat-equal -Wformat=2 -Wredundant-decls \
	-Wundef -Wdisabled-optimization -Wshadow \
	-Wmissing-braces -Wstrict-aliasing=2 -Wstrict-overflow=5 \
	-Wconversion
UNWARN := -Wno-sign-conversion -Wno-cast-align -Wno-conversion \
	-Wno-incompatible-pointer-types
DEBUG := -g3 -ggdb -DDEBUG
OPT := -O2 -march=native -mtune=native -ftree-vectorize

CFLAGS ?= $(STD) $(SECURITY) $(WARN) $(UNWARN)

INC := -I.

EXECUTABLE := lisp

all: $(EXECUTABLE)

src/prototype.h: src/prototype.dasc
	luajit dynasm/dynasm.lua $< > $@

$(EXECUTABLE): src/dynasm-driver.c src/prototype.h
	$(CC) $(CFLAGS) $(INC) -o $@ src/dynasm-driver.c -DJIT=\"prototype.h\"

clean:
	rm -f $(EXECUTABLE) || true
	rm -rf src/prototype.h || true

.PHONY: clean
