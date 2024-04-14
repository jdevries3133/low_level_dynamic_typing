CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRCDIR = src
BINDIR = bin

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:$(SRCDIR)/%.c=$(BINDIR)/%.o)

all: example

$(BINDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< -o $@

example: $(OBJS)
	mkdir -p bin
	$(CC) $^ -o $@

clean:
	rm -f $(BINDIR)/*.o example
