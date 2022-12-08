CC= gcc
CPPFLAGS= -Iinclude
CFLAGS= -Wall -Wextra -g `pkg-config --cflags gtk+-3.0` -rdynamic
LDFLAGS= -lSDL2 -lSDL2_image -lm `pkg-config --libs gtk+-3.0` -rdynamic

SRC_DIR= src
OBJ_DIR= obj
BIN_DIR= bin

DEBUG= -g

TEMP_DIR= ${BIN_DIR}/.temp/cells

SRC= $(wildcard ${SRC_DIR}/*.c)
_OBJT = ${SRC:.c=.o}
_OBJ = ${notdir ${_OBJT}}
OBJ= $(patsubst ${SRC_DIR}/%.c, ${OBJ_DIR}/%.o, ${SRC})
EXE= ${BIN_DIR}/sudoku
TEMP = ${BIN_DIR}/.temp/

.PHONY: all
all: ${EXE}

${EXE}: ${BIN_DIR} ${OBJ}
	${CC} ${DEBUG} ${CPPFLAGS} ${OBJ} -o ${EXE} ${LDFLAGS}

${BIN_DIR}:
	mkdir bin/.temp

${OBJ}:	${OBJ_DIR} ${BIN_DIR}
	${CC} ${CFLAGS} ${CPPFLAGS} -c ${SRC}
	mv ${_OBJ} ${OBJ_DIR}

${OBJ_DIR}:
	mkdir -p ${OBJ_DIR}
	mkdir bin/.temp/
	mkdir bin/.temp/cells

clean:
	rm -rf ${OBJ_DIR}
	rm -rf ${EXE}
	rm -rf ${TEMP}
	rm ${BIN_DIR}/grid_00
	rm ${BIN_DIR}/grid.result
	rm ${BIN_DIR}/grid_solved.png
