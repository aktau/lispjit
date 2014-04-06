CC ?= clang

# CFLAGS += -g3 -ggdb -O0

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
