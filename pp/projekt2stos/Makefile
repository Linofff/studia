COMPILER = gcc

CFLAGS = -c `sdl2-config --cflags --libs` -Wall -Wextra -Wpedantic -Wstrict-overflow=5 -Wimplicit-fallthrough=3 -Warray-bounds=2 -Wstringop-overflow=4 -Wformat=2 -Wformat-security -D_FORTIFY_SOURCE=3 -fstack-protector-strong -fstack-clash-protection -fPIE -pie -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -fno-strict-overflow -fno-strict-aliasing -fno-omit-frame-pointer -I./SDL2-2.0.10/include -L. -O2
LDFLAGS = -lSDL2

SRCS = main.c $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

all: game

game: ${OBJS}
	${COMPILER} -o $@ $^ ${LDFLAGS}
	rm -f ${OBJS}

main.o: main.c
	${COMPILER} ${CFLAGS} main.c

clean:
	rm -f $(OBJS) game
