#include "marching_squares.h"
#include <iostream>
// square side length
int SQUARE_SIZE = 20;

// initialise columns and rows
int COLS, ROWS;

// toggle grid
bool GRID_ON = false;

// indexes are decimal representation of state of square;
// values are the point pairs that create isolines when connected 
const int STATE_DICT[16][2][2] = 
{
    {{-1, -1}, {-1, -1}}, {{2, 3}, {-1, -1}}, {{1, 2}, {-1, -1}},
    {{1, 3}, {-1, -1}}, {{0, 1}, {-1, -1}}, {{2, 1}, {2, 3}}, 
    {{0, 2}, {-1, -1}}, {{0, 3}, {-1, -1}}, {{0, 3}, {-1, -1}},
    {{0, 2}, {-1, -1}}, {{0, 1}, {2, 3}}, {{0, 1}, {-1, -1}}, 
    {{1, 3}, {-1, -1}}, {{1, 2}, {-1, -1}}, {{2, 3}, {-1, -1}},
    {{-1, -1}, {-1, -1}}
};

void generate_field(int w, int h)
{
    COLS = w/SQUARE_SIZE;
    ROWS = h/SQUARE_SIZE;
}

int get_state(int p1, int p2, int p3, int p4)
{
    return (8*p1 + 4*p2 + 2*p3 + p4);
}

float lin_intrp(int edgepoint, std::vector<std::vector<float>> point_list, float threshold)
{
    int point1 = edgepoint;
    int point2 = edgepoint + 1;
    if (point2 > 3) 
        point2 = 0;

    float ratio = (threshold - point_list[point1][1])/(point_list[point2][1] - point_list[point1][1]);
    if (point1 < 2)
        return (ratio);
    else
        return (1 - ratio);
}

std::vector<int> get_coordinates(int x, int y, int edgepoint, float interpolate)
{
    std::vector<int> coordinates;
    switch(edgepoint)
    {
        case 0:
            coordinates.push_back((int)(x + SQUARE_SIZE*interpolate));
            coordinates.push_back(y);
            return coordinates;
        case 1:
            coordinates.push_back(x + SQUARE_SIZE);
            coordinates.push_back((int)(y + SQUARE_SIZE*interpolate));
            return coordinates;
        case 2:
            coordinates.push_back((int)(x + SQUARE_SIZE*interpolate));
            coordinates.push_back(y + SQUARE_SIZE);
            return coordinates;
        case 3:
            coordinates.push_back(x);
            coordinates.push_back((int)(y + SQUARE_SIZE*interpolate));
            return coordinates;
    }
    coordinates[0] = -1;
    coordinates[1] = -1;
    return coordinates;
}

void draw_square(SDL_Renderer *renderer, int x, int y, int side_length)
{   
    SDL_RenderDrawLine(renderer, x, y, x+SQUARE_SIZE, y);
    SDL_RenderDrawLine(renderer, x, y, x, y+SQUARE_SIZE);
    SDL_RenderDrawLine(renderer, x+SQUARE_SIZE, y, x+SQUARE_SIZE, y+SQUARE_SIZE);
    SDL_RenderDrawLine(renderer, x, y+SQUARE_SIZE, x+SQUARE_SIZE, y+SQUARE_SIZE);
}

void draw_isolines(SDL_Renderer *renderer, int x, int y, std::vector<std::vector<float>> point_list)
{
    int state = get_state(
        (int)point_list[0][0],
        (int)point_list[1][0],
        (int)point_list[2][0],
        (int)point_list[3][0]);

    for (auto line : STATE_DICT[state])
    {   
        bool empty = false;
        if (line[0] == -1)
        {
            if (GRID_ON)
            {
                SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
                SDL_Rect rect = {x, y, SQUARE_SIZE, SQUARE_SIZE};
                SDL_RenderDrawRect(renderer, &rect);
            }
            break;
        }
        float interpolate;

        interpolate = lin_intrp(line[0], point_list, 1);
        std::vector<int> start = get_coordinates(x, y, line[0], interpolate);
        interpolate = lin_intrp(line[1], point_list, 1);
        std::vector<int> end = get_coordinates(x, y, line[1], interpolate);


        if (GRID_ON)
        {
            SDL_SetRenderDrawColor(renderer, 250, 120, 80, 255);
        }
        else
            SDL_SetRenderDrawColor(renderer, 60, 150, 202, 255);
        SDL_RenderDrawLine(renderer, 
            start[0], start[1],
            end[0], end[1]);
    }
}