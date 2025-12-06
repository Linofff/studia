COMPILER = gcc
# CFLAGS = -c
CFLAGS = -c -Wall -Wextra -Wpedantic -Wstrict-overflow=5 -Wimplicit-fallthrough=3 -Warray-bounds=2 -Wstringop-overflow=4 -Wformat=2 -Wformat-security -D_FORTIFY_SOURCE=3 -fstack-protector-strong -fstack-clash-protection -fPIE -pie -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -fno-strict-overflow -fno-strict-aliasing -fsanitize=address -fsanitize=undefined -fsanitize=leak -fno-omit-frame-pointer 
# CFLAGS = -c -g -O0
LDFLAGS = -lasan -lubsan -lncurses -lm
# LDFLAGS = -lncurses -lm

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
