CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -rdynamic
LDFLAGS = `pkg-config --libs gtk+-3.0`

all:
	$(CC) main.c -o main $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o main
