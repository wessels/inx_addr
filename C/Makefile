NAME=inx_addr
LIB=lib${NAME}_c.a
CFLAGS=-Wall -pg
PREFIX=/usr/local


all: ${LIB}

${LIB}: ${NAME}.o
	ar r $@ ${NAME}.o

${NAME}.o: ${NAME}.c ${NAME}.h

install: ${LIB}
	install -d -m 755 ${PREFIX}/lib
	install -d -m 755 ${PREFIX}/include
	install -C -m 644 ${LIB} ${PREFIX}/lib
	install -C -m 644 ${NAME}.h ${PREFIX}/include/${NAME}_c.h

clean:
	rm -f ${NAME}.o
	rm -f ${LIB}
