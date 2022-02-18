CC=gcc
CFLAGS=-c -Wall
LFLAGS=-o
LIBS=-lpthread -lrt
SRC=cbuf.c main.c
OBJ=cbuf.o main.o
EXEC=cbuf
RM=rm -rf

all:
	${CC} ${CFLAGS} ${SRC}
	${CC} ${OBJ} ${LFLAGS} ${EXEC} ${LIBS}

clean:
	${RM} ${OBJ} ${EXEC}
