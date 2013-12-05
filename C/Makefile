NAME=inx_addr
LIB=lib${NAME}_c.a
CFLAGS=-Wall -pg


all: ${LIB}

${LIB}: ${NAME}.o
	ar r $@ ${NAME}.o

${NAME}.o: ${NAME}.c ${NAME}.h

install: ${LIB}
	install -C -m 644 ${LIB} /usr/local/lib
	install -C -m 644 ${NAME}.h /usr/local/include/${NAME}_c.h

clean:
	rm -f ${NAME}.o
	rm -f ${LIB}
