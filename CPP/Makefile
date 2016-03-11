NAME=inX_addr
LIB=lib${NAME}.a
CFLAGS=-Wall -O2
PREFIX=/usr/local


all: ${LIB}

${LIB}: ${NAME}.o
	ar r $@ ${NAME}.o

${NAME}.o: ${NAME}.cc ${NAME}.h

test: test.cc ${NAME}.o ${NAME}.h
	${CXX} ${CFLAGS} -o $@ -I. test.cc ${NAME}.o

install: ${LIB}
	install -d -m 755 ${PREFIX}/lib
	install -d -m 755 ${PREFIX}/include
	install -C -m 644 ${LIB} ${PREFIX}/lib
	install -C -m 644 ${NAME}.h ${PREFIX}/include

clean:
	rm -f ${NAME}.o
	rm -f ${LIB}
