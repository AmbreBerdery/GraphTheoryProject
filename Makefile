CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -O2
SRCDIR  = src
SRCS    = $(SRCDIR)/main.c $(SRCDIR)/graph.c $(SRCDIR)/floyd_warshall.c
OBJS    = $(SRCS:.c=.o)
TARGET  = floyd_warshall

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(SRCDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/graph.h $(SRCDIR)/floyd_warshall.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
