#ifndef _METABALLS_H_
#define _METABALLS_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <random>
#include <vector>

struct Ball
{
    int rad;
    float x;
    float y;
    int vx;
    int vy;
};

extern int TOTAL_BALLS;
extern std::vector<Ball> METABALLS;

extern bool drag;
extern unsigned int active_drag;

// generates balls and append to METABALLS
void generate_balls(int w, int h, int ball_num, int min_vel, int max_vel);
// updates ball positions and updates velocity if collision occurs
void update_ball_positions(int w, int h, float delta);
// checks if point lies within or outside function
std::vector<float> point_state(int x, int y);
// draws rect at center of each ball
void draw_paused(SDL_Renderer *renderer, bool clicked, int xm, int ym);

#endif
