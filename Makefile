
CFLAGS = -Wall -Wextra -Werror -O3 -fjump-tables
#CFLAGS = -Wall -Wextra -Werror -g -O0 -fsanitize=address,undefined

all: scheme

scheme: display.c object.h read.c scheme.h scheme.c
	$(CC) $(CFLAGS) -o $@ scheme.c

clean:
	rm -f scheme *.out

test: scheme test_read.test fuzz

# test golden reference and round-trip invariant
%.test: %.scm
	./scheme < $< > $*.out
	diff $*.ref $*.out
	sed 's/scheme> //' $*.out > $*.scm.out
	./scheme < $*.scm.out > $*.scm.out.out
	diff $*.out $*.scm.out.out

fuzz:
	./fuzz.sh
