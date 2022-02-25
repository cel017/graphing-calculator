#ifndef _METABALLS_H_
#define _METABALLS_H_

#include <iostream>
#include <random>
#include <vector>
#include <math.h>

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

// generates balls and append to METABALLS
void generate_balls(int w, int h, int ball_num, int min_vel, int max_vel);
// updates ball positions and updates velocity if collision occurs
void update_ball_positions(int w, int h, float delta);
// checks if point lies within or outside function
std::vector<float> point_state(int x, int y);

#endif
