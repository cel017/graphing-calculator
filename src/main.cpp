#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <iostream>

#include "metaballs.h"
#include "marching_squares.h"

void render_frame(SDL_Renderer * renderer, int width, int height, float delta)
{
    update_ball_positions(width, height, delta);
    // rendering isolines for each 
    // square in the grid;
    for (int i = 0; i < COLS+1; i++)
    {
        for (int j = 0; j < ROWS+1; j++)
        {   
            int x_pos = i*SQUARE_SIZE, y_pos = j*SQUARE_SIZE;
            std::vector<std::vector<float>> square;

            square.push_back(point_state(x_pos, y_pos));
            square.push_back(point_state(x_pos+SQUARE_SIZE, y_pos));
            square.push_back(point_state(x_pos+SQUARE_SIZE, y_pos+SQUARE_SIZE));
            square.push_back(point_state(x_pos, y_pos+SQUARE_SIZE));

            draw_isolines(renderer, x_pos, y_pos, square);
        }
    }

    SDL_RenderPresent(renderer);    // update render
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}
int main(int argc, char* args[])
{
    int width = 1000, height = 840;

    // GET COLS AND ROWS FOR GRID
    generate_field(width, height);

    // GENERATE METABALLS PSEUDO RANDOMLY
    generate_balls(width, height, 5, 10, 20);

    unsigned int f0 = SDL_GetTicks();
    unsigned int f1 = SDL_GetTicks();
    double delta = 0;

    if (SDL_Init(SDL_INIT_VIDEO) == 0) 
    {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        if (SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer) == 0) 
        {   
            SDL_RaiseWindow(window);
            SDL_bool run = SDL_TRUE;
            bool paused = false;
            bool unpaused = false;
            unsigned int time_paused = 0;
            unsigned int paused_time = 0;
            while (run) 
            {
                if (unpaused)
                {
                    time_paused += SDL_GetTicks() - paused_time;
                    unpaused = false;
                }

                if (!paused)
                {
                    f0 = SDL_GetTicks() - time_paused;
                    delta = f0-f1;
                }

                SDL_Event event;
                while (SDL_PollEvent(&event)) 
                {
                    if (event.type == SDL_QUIT)
                        run = SDL_FALSE;
                    if (event.type == SDL_KEYDOWN)
                    {
                        switch(event.key.keysym.sym)
                        {
                            case SDLK_SPACE:
                                if (!paused)
                                    paused_time = SDL_GetTicks();
                                else
                                    unpaused = true;
                                paused = !paused;
                                break;
                            case SDLK_g:
                                GRID_ON = !GRID_ON;
                                break;
                            case SDLK_RIGHTBRACKET:
                                SQUARE_SIZE++;
                                generate_field(width, height);
                                break;
                            case SDLK_LEFTBRACKET:
                                SQUARE_SIZE--;
                                generate_field(width, height);
                                break;
                        }
                    }
                }

                if (delta>1000/120.0 and !paused)
                {
                    std::cout << 1000/delta << std::endl;
                    f1 = f0;
                    render_frame(renderer, width, height, delta);
                }
            }   
        }
        if (renderer)
            SDL_DestroyRenderer(renderer);
        if (window)
            SDL_DestroyWindow(window);
    }
    SDL_Quit();
    return 0;
}
