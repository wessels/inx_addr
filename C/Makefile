NAME=inX_addr
LIB=lib${NAME}.a
CFLAGS=-Wall -pg


all: ${LIB}

${LIB}: ${NAME}.o
	ar r $@ ${NAME}.o

${NAME}.o: ${NAME}.c ${NAME}.h

install: ${LIB}
	install -C -m 644 ${LIB} /usr/local/lib
	install -C -m 644 ${NAME}.h /usr/local/include

clean:
	rm -f ${NAME}.o
	rm -f ${LIB}
