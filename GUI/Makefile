CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -rdynamic 
#LDFLAGS = `pkg-config --libs gtk+-3.0` -lSDL2 -lSDL2_image -Iinclude -lm
LDFLAGS = `pkg-config --libs gtk+-3.0` -lSDL2 -lSDL2_image -I../include -lm

all:
	$(CC) main.c ../rotate_img.c ../image_loading.c ../pixel.c ../matrix.c -o main $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o main
