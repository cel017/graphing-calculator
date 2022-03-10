#include "metaballs.h"

bool drag = false;
unsigned int active_drag = 0;

int TOTAL_BALLS;
std::vector<Ball> METABALLS;
 
void generate_balls(int w, int h, int ball_num, int min_vel, int max_vel)
{

    std::random_device rd;
    std::mt19937 rng(rd());

    std::uniform_int_distribution<> dist_rad(10, 12);
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

void draw_paused(SDL_Renderer *renderer, bool clicked, int xm, int ym)
{
    // function to draw pause screen in which balls can be dragged

    if (drag)
    {
        // if ball is actively in drag state, other balls do not need to be checked
        METABALLS[active_drag].x = xm;
        METABALLS[active_drag].y = ym;
    }

    for (int i = 0; i < TOTAL_BALLS; i++)
    {
        // rects to signify centres
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        SDL_Rect rect = {(int)METABALLS[i].x-15, (int)METABALLS[i].y-15, 30, 30};

        // not in active drag state and mouse click detected
        if (!drag and clicked)
        {
            if (rect.x<xm and xm<(rect.x+30) and rect.y<ym and ym<(rect.y+30))
            {
                METABALLS[i].x = xm;
                METABALLS[i].y = ym;
                drag = true;
                active_drag = i;
            }
        }

        // already in drag state and no mouse click detected
        else if (!clicked and drag)
            drag = false;

        // draw centre rects
        SDL_RenderDrawRect(renderer, &rect);
    }
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

        //update positions
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

    // return state and value of point(for interpolation)
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
