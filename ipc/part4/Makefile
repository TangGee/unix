include ../Make.defines

PROGS = mainpipe mainfifo 

all: ${PROGS}

mainpipe: mainpipe.o client.o server.o
	${CC} ${CFLAGS} -g -o $@ mainpipe.o client.o server.o $(LIBS)

mainfifo: mainfifo.o client.o server.o
	${CC} ${CFLAGS} -o $@ mainfifo.o client.o server.o $(LIBS)

clean: 
	rm -f ${PROGS} mainfifo.o mainpipe.o client.o server.o 
