CXX ?= g++
ERROR ?= -Wall
CFLAGS ?= -std=c++14 -g -m64 ${ERROR}

run: main
	start bin/debug/main.exe

main: obj
	${CXX} *.o -o bin/debug/main -L C:/SDL2-w64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

obj:
	${CXX} -c src/*.cpp ${CFLAGS} -I include -I C:/SDL2-w64/include

clean:
	rm -f main *.o

.PHONY: main clean
