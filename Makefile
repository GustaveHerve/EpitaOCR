CC=gcc
CLAGS= -Wall -Wextra -g
LDFLAGS= -lSDL2 -lSDL2_image -lm

SRC= matrix.c test.c image_processing.c geometry.c utils.c image_loading.c pixel.c edge_detection.c grid_detection.c morph.c thresholding.c grid_extraction.c
OBJ= ${SRC:.c=.o}
EXEC = debug

${EXEC}: ${OBJ}
	${CC} ${CFLAGS} $^ -o $@ ${LDFLAGS}

all: ${EXEC}

cleanbin:
	rm ${OBJ}

bin: ${EXEC} cleanbin

clean:
	rm ${OBJ}
	rm ${EXEC}
