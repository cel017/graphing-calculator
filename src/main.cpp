#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>
#include <iostream>

#include "metaballs.h"
#include "marching_squares.h"
#include "graph.h"

int main(int argc, char* args[])
{
    int width = 500, height = 500;

    // GET COLS AND ROWS FOR GRID
    generate_field(width, height);

    // GENERATE METABALLS PSEUDO RANDOMLY
    generate_balls(width, height, 180, 10, 20);

    unsigned int f0 = SDL_GetTicks();
    unsigned int f1 = SDL_GetTicks();
    double delta = 0;

    if (SDL_Init(SDL_INIT_VIDEO) == 0) 
    {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        if (SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer) == 0) 
        {   
            // initialising vars
            bool paused = false;
            bool unpaused = false;
            unsigned int time_paused = 0;
            unsigned int paused_time = 0;
            int xmouse, ymouse;
            bool mouse_clicked = false;
            bool update_text = false;
            std::string input_equation = "";

            SDL_RaiseWindow(window);
            SDL_bool run = SDL_TRUE;
            while (run) 
            {
                // get mouse pos
                SDL_GetMouseState(&xmouse, &ymouse);

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

                // INPUTS //
                SDL_Event event;
                while (SDL_PollEvent(&event)) 
                {
                    switch(event.type)
                    {
                        case SDL_QUIT:
                            run = SDL_FALSE;
                            break;
                        // MOUSE INPUTS //
                        case SDL_MOUSEBUTTONDOWN:
                            mouse_clicked = true;
                            break;
                        case SDL_MOUSEBUTTONUP:
                            mouse_clicked = false;
                            break;
                        // KEYBOARD INPUTS //
                        case SDL_KEYDOWN:
                        {
                            switch(event.key.keysym.sym)
                            {
                                // general keydown events
                                case SDLK_SPACE:    // CTRL + SPACE
                                    if (not (SDL_GetModState() & KMOD_CTRL))
                                        break;
                                    if (!paused)
                                        paused_time = SDL_GetTicks();
                                    else
                                        unpaused = true;
                                    paused = !paused;
                                    break;
                                case SDLK_g:        // CTRL + G
                                    if (not (SDL_GetModState() & KMOD_CTRL))
                                        break;
                                    GRID_ON = !GRID_ON;
                                    break;
                                case SDLK_RIGHTBRACKET:
                                    if (not (SDL_GetModState() & KMOD_CTRL))
                                        break;
                                    SQUARE_SIZE++;
                                    generate_field(width, height);
                                    break;
                                case SDLK_LEFTBRACKET:
                                    if (not (SDL_GetModState() & KMOD_CTRL))
                                        break;
                                    SQUARE_SIZE--;
                                    generate_field(width, height);
                                    break;

                                // equation input keydown events
                                case SDLK_BACKSPACE:
                                    if (input_equation.length() == 0)
                                        break;
                                    input_equation.pop_back();
                                    update_text = true;
                                    break;
                            }
                            break;
                        }
                        // TEXT INPUT //
                        case SDL_TEXTINPUT:
                        {
                            if( SDL_GetModState() & KMOD_CTRL)
                                break;
                            input_equation += event.text.text;
                            update_text = true;
                            break;
                        }
                    }
                }

                // RENDER FRAME //
                if (delta>1000/240.0 or paused)
                {
                    std::cout << 1000/delta << std::endl;
                    f1 = f0;

                    // draw border
                    SDL_SetRenderDrawColor(renderer, 40, 140, 80, 255);
                    SDL_Rect rect = {0, 0, width, height};
                    SDL_RenderDrawRect(renderer, &rect);

                    if (paused)
                        draw_paused(renderer, mouse_clicked, xmouse, ymouse);
                    else
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
