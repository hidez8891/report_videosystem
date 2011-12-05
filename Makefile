# Makefile

SHELL=/bin/sh

MODE     = MODE_FULL
# MODE     = MODE_TSS
# MODE     = MODE_GS
# MODE     = MODE_DS
# MODE     = MODE_HEX

CXX      = g++
CPPFLAGS = -std=c++0x -O4 -D${MODE}

SRCS     = main.cpp
OBJS     = ${SRCS:.cpp=.o}
TARGET   = main


all: ${TARGET}

${TARGET}: ${OBJS}
	${CXX} -o $@ ${OBJS} ${LDFLAGS}

.cpp.o:
	${CXX} ${CPPFLAGS} -c $<

clean:
	rm -f ${OBJS} core 
