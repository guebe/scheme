
CC = clang
CFLAGS = -Wall -Wextra -Werror -O3 -fjump-tables
#CFLAGS = -Wall -Wextra -Werror -g -O0 -fsanitize=address,undefined

all: scheme

scheme: error.c number.c read.c port.c write.c main.c scheme.h
	$(CC) $(CFLAGS) -o $@ error.c number.c port.c pair.c read.c write.c main.c

clean:
	rm -f scheme *.out

test: scheme test_read.test fuzz fuzz_read

# test golden reference and round-trip invariant
%.test: %.scm
	./scheme < $< > $*.out
	diff $*.ref $*.out
	sed 's/scheme> //' $*.out > $*.scm.out
	./scheme < $*.scm.out > $*.scm.out.out
	diff $*.out $*.scm.out.out

fuzz_read: error.c number.c read.c port.c write.c fuzz.c scheme.h
	$(CC) -fsanitize=fuzzer,address,undefined -g -O1 -o $@ read.c write.c fuzz.c

fuzz:
	./fuzz.sh
