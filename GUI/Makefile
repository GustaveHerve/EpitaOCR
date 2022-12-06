CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -rdynamic 
#LDFLAGS = `pkg-config --libs gtk+-3.0` -lSDL2 -lSDL2_image -Iinclude -lm
LDFLAGS = `pkg-config --libs gtk+-3.0` -lSDL2 -lSDL2_image -I../include -lm

all:
	$(CC) main.c ../src/rotate_img.c ../src/grid_detection.c ../src/grid_extraction.c ../src/morph.c  ../src/utils.c ../src/thresholding.c ../src/image_processing.c ../src/image_loading.c ../src/pixel.c ../src/matrix.c ../src/geometry.c -o main $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o main
