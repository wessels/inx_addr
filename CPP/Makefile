NAME=inX_addr
LIB=lib${NAME}.a
CFLAGS=-Wall -O2


all: ${LIB}

${LIB}: ${NAME}.o
	ar r $@ ${NAME}.o

${NAME}.o: ${NAME}.cc ${NAME}.h

test: test.cc ${NAME}.o ${NAME}.h
	${CXX} ${CFLAGS} -o $@ -I. test.cc ${NAME}.o

install: ${LIB}
	install -C -m 644 ${LIB} /usr/local/lib
	install -C -m 644 ${NAME}.h /usr/local/include

clean:
	rm -f ${NAME}.o
	rm -f ${LIB}
