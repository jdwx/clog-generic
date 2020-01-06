
CXX=		clang++ -std=c++17 -stdlib=libc++
CXXFLAGS=	-Weverything -pedantic
CXXFLAGS+=	-Wno-padded -Wno-weak-vtables -Wno-c++98-compat
#CXXFLAGS+=  -I/usr/local/include

CXXFLAGS+=	-g -O2

COMMON_OBJS=	RingBuffer.o

CLIENT_OBJS=	clog.o
CLIENT_TARGET=	clog

SERVER_OBJS=	clogd.o
SERVER_TARGET=	clogd

ARCH=`uname -m`
MAJOR=`uname -r | cut -d. -f1`

OBJS=			${SERVER_OBJS} ${CLIENT_OBJS} ${COMMON_OBJS}
TARGETS=		${SERVER_TARGET} ${CLIENT_TARGET}


all: ${TARGETS}

install: all
	cp ${SERVER_TARGET} /net/setup/data/${SERVER_TARGET}-${ARCH}-${MAJOR}
	cp ${CLIENT_TARGET} /net/setup/data/${CLIENT_TARGET}-${ARCH}-${MAJOR}

${CLIENT_TARGET}: ${CLIENT_OBJS} ${COMMON_OBJS}
	${CXX} -o $@ ${CLIENT_OBJS} ${COMMON_OBJS}

${SERVER_TARGET}: ${SERVER_OBJS} ${COMMON_OBJS}
	${CXX} -o $@ ${SERVER_OBJS} ${COMMON_OBJS}

clean:
	rm -f ${TARGETS} ${OBJS} *.core

depend:
	${CXX} ${CXXFLAGS} -M *.cc >.depend

test: clogd
	./clogd /clog-test <test.data

-include .depend

