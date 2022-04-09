CXX ?= g++
ERROR ?= -Wall -Wextra -Werror -pedantic
CFLAGS ?= -O3 -ggdb ${ERROR}
LDFLAGS ?= -lSDL2 -lSDL2main -lSDL2_image
SRC = $(wildcard src/*.cpp)
OBJ = ${SRC:.cpp=.o}

metaballs: ${OBJ}
	${CXX} ${LDFLAGS} -o metaballs ${OBJ}

%.o: %.cpp
	${CXX} -I include/ -c ${CFLAGS} -o $@ $<

clean:
	rm -f metaballs ${OBJ}

.PHONY: metaballs clean
