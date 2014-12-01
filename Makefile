
CC=gcc
CFLAGS= -Wall -Wextra -O2


all: scroll_text

scroll_text: scroll_text.o


.PHONY: clean
clean:
	$(RM) scroll_text *.o

tst: scroll_text
	./scroll_text
