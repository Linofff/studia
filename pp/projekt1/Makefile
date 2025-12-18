COMPILER = gcc
CFLAGS = -c  
LDFLAGS = -lncurses -lm

SRCS = main.c $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

all: swallow-stars

swallow-stars: ${OBJS}
	${COMPILER} -o $@ $^ ${LDFLAGS}
	rm -f ${OBJS}

main.o: main.c
	${COMPILER} ${CFLAGS} main.c

clean:
	rm -f $(OBJS) swallow-stars
