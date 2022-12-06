CC= gcc
CPPFLAGS= -Iinclude
CFLAGS= -Wall -Wextra -fsanitize=address
LDFLAGS= -lSDL2 -lSDL2_image -lm -fsanitize=address

SRC_DIR= src
OBJ_DIR= obj
BIN_DIR= bin

TEMP_DIR= ${BIN_DIR}/.temp/cells

SRC= $(wildcard ${SRC_DIR}/*.c)
_OBJT = ${SRC:.c=.o}
_OBJ = ${notdir ${_OBJT}}
OBJ= $(patsubst ${SRC_DIR}/%.c, ${OBJ_DIR}/%.o, ${SRC})
EXE= ${BIN_DIR}/sudoku

.PHONY: all
all: ${EXE} 

${EXE}: ${BIN_DIR} ${OBJ}
	${CC} ${CPPFLAGS} ${OBJ} -o ${EXE} ${LDFLAGS}

${BIN_DIR}:
	mkdir -p ${TEMP_DIR}

${OBJ}:	${OBJ_DIR}
	${CC} ${CFLAGS} ${CPPFLAGS} -c ${SRC}
	mv ${_OBJ} ${OBJ_DIR}

${OBJ_DIR}:
	mkdir -p ${OBJ_DIR}

clean:
	rm -rf ${OBJ_DIR}
	rm -rf ${BIN_DIR}
