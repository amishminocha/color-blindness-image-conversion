CC = clang
CFLAGS = -Wall -Wextra -Wstrict-prototypes -Werror -pedantic
LFLAGS = -lm

EXEC = colorb 
OBJS = io.o bmp.o colorb.o 
OBJS1 = io.o iotest.o

all: colorb iotest

colorb: $(OBJS)
	$(CC) $(LFLAGS) -o colorb $(OBJS)

iotest: $(OBJS1)
	$(CC) $(LFLAGS) -o iotest $(OBJS1)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f colorb iotest $(OBJS) $(OBJS1)

format:
	clang-format -i --style=file *.[ch]

