#ifndef _MARCHING_SQUARES_H_
#define _MARCHING_SQUARES_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

extern int SQUARE_SIZE;
extern int COLS, ROWS;
extern const int STATE_DICT[16][2][2];
extern bool GRID_ON;

// assign values to COLS and ROWS
void generate_field(int w, int h);
// convert state of square to decimal from binary
int get_state(int p1, int p2, int p3, int p4);
// get factor by which edgepoint is shifted (linearly interpolated between points)
float lin_intrp(int edgepoint, std::vector<std::vector<float>> point_list, float threshold);
// get coordinates for edgepoint on render window
std::vector<int> get_coordinates(int x, int y, int edgepoint, float interpolate);
// draws isolines based on state of square
void draw_isolines(SDL_Renderer *renderer, int x, int y, int width, int height, std::vector<std::vector<float>> point_list);

#endif
