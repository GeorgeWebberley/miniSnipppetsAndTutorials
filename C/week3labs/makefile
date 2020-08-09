CC=gcc
CFLAGS= -pedantic -ansi -Wall -Wextra -Wfloat-equal -O2

all: roman_numerals
roman_numerals: roman_numerals.o roman_numerals.c
	$(CC) $(CFLAGS) roman_numerals.c -o roman_numerals

clean:
	rm -f roman_numerals roman_numerals.o
run:	roman_numerals
	./roman_numerals
