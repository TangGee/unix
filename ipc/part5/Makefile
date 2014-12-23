include ../Make.defines

PROGS = mqcreatel 

all: ${PROGS}

mainpipe: mqcreatel.o 
	${CC} ${CFLAGS} -g -o  $@ mqcreatel.o $(LIBS)  -lrt

mqcreatel.o : mqcreatel.c
	${CC} ${CFLAGS} -c -o $@ mqcreatel.c  

clean: 
	rm -f ${PROGS} mqcreatel.o 
