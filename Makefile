default: library

library: pprintf.c
	gcc -Wall -c pprintf.c
	ar -cvq libpprintf.a pprintf.o
	rm -rf pprintf.o

test: clean library 
	gcc -Wall test.c -o test -L. -lpprintf
	./test

leak: test
	valgrind ./test

clean:
	rm -rf test
	rm -rf libpprintf.a
