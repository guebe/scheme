# (c) guenter.ebermann@htl-hl.ac.at

SRC = error.c number.c pair.c port.c read.c
SRC_SCHEME = $(SRC) write.c eval.c main.c
SRC_FUZZ = $(SRC) fuzz.c

CC = clang
CFLAGS = -Wall -Wextra -Wshadow -Wconversion -Wpedantic -Wstrict-prototypes -Wsign-compare -Wformat-security -Wmisleading-indentation -Wnonnull -Wold-style-definition -Wnested-externs -Werror -O3 -fjump-tables
CFLAGS_EXTRA_DEBUG = -O0 -g -fsanitize=address,undefined
CFLAGS_EXTRA_FUZZ = -O1 -g -fsanitize=fuzzer,address,undefined

.PHONY: all test clean analyze fuzz_blackbox

all: scheme

test: scheme scheme_debug scheme_test test_read.test fuzz_whitebox fuzz_blackbox analyze tidy

clean:
	rm -f scheme scheme_debug fuzz_whitebox *.out *.plist

analyze:
	clang --analyze $(SRC_SCHEME)

tidy:
	clang-tidy $(SRC_SCHEME) -checks=misc-unused-functions,misc-unused-parameters,-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling -- -I.

fuzz_blackbox:
	./fuzz_blackbox

scheme: $(SRC_SCHEME) scheme.h
	$(CC) $(CFLAGS) -o $@ $(SRC_SCHEME)

scheme_debug: $(SRC_SCHEME) scheme.h
	$(CC) $(CFLAGS) $(CFLAGS_EXTRA_DEBUG) -o $@ $(SRC_SCHEME)

fuzz_whitebox: $(SRC_FUZZ) scheme.h
	$(CC) $(CFLAGS) $(CFLAGS_EXTRA_FUZZ) -o $@ $(SRC_FUZZ)

scheme_test: $(SRC_SCHEME) scheme.h
	$(CC) $(CFLAGS) -DSCM_NO_EVAL -o $@ $(SRC_SCHEME)

# test golden reference and round-trip invariant
%.test: %.scm
	./scheme_test < $< > $*.out
	diff $*.ref $*.out
	sed 's/> //' $*.out > $*.scm.out
	./scheme_test < $*.scm.out > $*.scm.out.out
	diff $*.out $*.scm.out.out
