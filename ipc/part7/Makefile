include ../Make.defines

PROGS = prodcons2


all: ${PROGS}

producons2: producons2.o
	${CC} ${CFLAGS} -g -o $@ producons2.o $(LIBS) -pthread


clean: 
	rm -f ${PROGS} producons2 producons2.o 
