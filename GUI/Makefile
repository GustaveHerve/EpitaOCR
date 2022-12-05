CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -rdynamic 
#LDFLAGS = `pkg-config --libs gtk+-3.0` -lSDL2 -lSDL2_image -Iinclude -lm
LDFLAGS = `pkg-config --libs gtk+-3.0` -lSDL2 -lSDL2_image -I../include -lm

all:
	$(CC) main.c ../rotate_img.c ../display.c ../grid_detection.c ../grid_extraction.c ../morph.c  ../utils.c ../thresholding.c ../image_processing.c ../image_loading.c ../pixel.c ../matrix.c ../geometry.c -o main $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o main
