#include "metaballs.h"

int TOTAL_BALLS;
std::vector<Ball> METABALLS;
 
void generate_balls(int w, int h, int ball_num, int min_vel, int max_vel)
{

    std::random_device rd;
    std::mt19937 rng(rd());

    std::uniform_int_distribution<> dist_rad(80, 100);
    std::uniform_int_distribution<> dist_cent_x(0, w);
    std::uniform_int_distribution<> dist_cent_y(0, h);
    std::uniform_int_distribution<> dist_vel_x(min_vel, max_vel);
    std::uniform_int_distribution<> dist_vel_y(min_vel, max_vel);
    std::uniform_int_distribution<> choose(1, 2);

    for (int i = 0; i < ball_num; i++)
    {
        // generate pseudo random radius and center
        int radius = dist_rad(rng);
        int center_x = dist_cent_x(rng);
        int center_y = dist_cent_y(rng);
        // makes sure circle is within bounds
        if (center_x > w-radius)
            center_x -= radius;
        else if (center_x < radius)
            center_x += radius;
        if (center_y > h-radius)
            center_y -= radius;
        else if (center_y < radius)
            center_y += radius;
        // generate velocity
        int vel_x    = dist_vel_x(rng);
        int vel_y    = dist_vel_y(rng);
        // randomise direction of vel
        vel_x = (choose(rng) == 1)? vel_x : -vel_x; 
        vel_y = (choose(rng) == 1)? vel_x : -vel_x;

        METABALLS.push_back(Ball());
        METABALLS[i].rad = radius;
        METABALLS[i].x = center_x;
        METABALLS[i].y = center_y;
        METABALLS[i].vx = vel_x;
        METABALLS[i].vy = vel_y;
    }
    TOTAL_BALLS = ball_num;
    return;
}

void update_ball_positions(int w, int h, float delta)
{
    const float tuning_const = 0.01;
    for (int i = 0; i < TOTAL_BALLS; i++)
    {
        // makeshift collision detection
        if (METABALLS[i].x - METABALLS[i].rad < 0 or METABALLS[i].x + METABALLS[i].rad > w)
            METABALLS[i].vx = -METABALLS[i].vx;
        if (METABALLS[i].y - METABALLS[i].rad < 0 or METABALLS[i].y + METABALLS[i].rad > h)
            METABALLS[i].vy = -METABALLS[i].vy;

        METABALLS[i].x += METABALLS[i].vx*delta*tuning_const;
        METABALLS[i].y += METABALLS[i].vy*delta*tuning_const;
    }
}

std::vector<float> point_state(int x, int y)
{
    std::vector<float> point;
    float val = 0.0f;
    for (int i = 0; i < TOTAL_BALLS; i++)
    {
        // radius `r` as float to get float val after division
        float r = METABALLS[i].rad;
        int e_x = METABALLS[i].x - x;
        int e_y = METABALLS[i].y - y;
        val += (r*r)/(e_x*e_x + e_y*e_y);
    }
    val = val/TOTAL_BALLS*TOTAL_BALLS;
    if (val > 1)
    {
        point.push_back(1.0f);
        point.push_back(val);
        return point;
    }
    else
    {
        point.push_back(0.0f);
        point.push_back(val);
        return point;
    }
}
