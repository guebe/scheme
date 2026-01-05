
CFLAGS = -Wall -Wextra -Werror -O3 -fjump-tables -fsanitize=address,undefined
#CFLAGS = -Wall -Wextra -Werror -g -O0

all: scheme

scheme: main.o read.o display.o cell.o
	${CC} ${CFLAGS} -o $@ $^

clean:
	rm -f scheme *.o *.out

main.o: main.c read.h display.h
read.o: read.c read.h box.h cell.h pair.h
display.o: display.c display.h box.h cell.h
cell.o: cell.c cell.h

test: test_read.test fuzz scheme

# 1. test against a golden reference file
# 2. test round-trip invariant
%.test: %.scm
	./scheme < $< > $*.out
	diff $*.ref $*.out
	sed 's/scheme> //' $*.out > $*.scm.out
	./scheme < $*.scm.out > $*.scm.out.out
	diff $*.out $*.scm.out.out

fuzz:
	./fuzz.sh
