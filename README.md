# Graphing Calculator
 Basic implementation of a graph plotter using the marching squares algorithm
 
 ## Usage

`Ctrl+g` to toggle grid; `Ctrl+[` and `Ctrl+]` to increase/decrease grid resolution.

## Running Instructions
Install SDL2, SDL_Image and use the following command in project directory to compile and run the program:

```

g++ -c src/*.cpp -std=c++14 -g -Wall -m64 -I include -I C:/SDL2-w64/include && g++ *.o -o bin/debug/main -L C:/SDL2-w64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image && start bin/debug/main"

```
