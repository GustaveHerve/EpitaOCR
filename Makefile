CC=gcc
CLAGS= -Wall -Wextra -g -Werror -fsanitize=address
LDFLAGS= -lSDL2 -lSDL2_image -lm

SRC= matrix.c test.c image_processing.c geometry.c utils.c image_loading.c pixel.c edge_detection.c grid_detection.c morph.c thresholding.c grid_extraction.c display.c rotate_img.c
OBJ= ${SRC:.c=.o}
EXEC = debug

ROTSRC= sdaile.c image_loading.c rotate_img.c pixel.c main.c matrix.c
ROTATE = rotation

${EXEC}: ${OBJ}
	${CC} ${CFLAGS} $^ -o $@ ${LDFLAGS}

${ROTATE}: ${OBJ}
	${CC} ${CFLAGS} ${ROTSRC} -o $@ ${LDFLAGS}

all: ${EXEC}

cleanbin:
	rm ${OBJ}

bin: ${EXEC} cleanbin

rotation: ${ROTATE}

clean:
	rm ${OBJ}
	rm ${EXEC}

cleanrot:
	rm ${OBJ}
	rm ${ROTATE}
