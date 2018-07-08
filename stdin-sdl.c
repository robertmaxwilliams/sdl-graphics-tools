#include <stdio.h>
#include <SDL.h>
#include <math.h>
#include <stdlib.h>


#include <unistd.h>
#include <signal.h>

#define WIDTH 900
//640
#define HEIGHT 700
//480


volatile sig_atomic_t stop;

void inthand(int signum) {
    stop = 1;
}


int main(int argc, char* argv[])
{

    int breakmode = 0;
    int pxp = 1;

    for (int i = 1; i<argc; i++)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
                case 'b':
                    breakmode = 1;
                    printf("breakmode!\n");
                    break;

                case 'p':
                    pxp = 16;
                    printf("big pix!\n");
                    break;

                case 'n':
                    pxp = atoi(argv[++i]);
                    printf("big pix n = %d!\n", pxp);
                    break;

                default:
                    printf("Not recorgnized\n");
            }
        } else
        {
            printf("Super not recognized\n");
        }
    }


    SDL_Event event;
    
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
    SDL_RenderClear(renderer);
    int * crash = NULL;
    signal(SIGINT, inthand);
    

    int quit = 0;
    int nomore = 0;
    int i = 0;
    while (!stop)
    {
        if (1 || SDL_PollEvent(&event))
        {

            switch(event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;

                case SDL_MOUSEMOTION:
                    //....
                    break;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_ESCAPE: 
                            quit = 1;
                            break;
                            // cases for other keypresses
                        case SDL_SCANCODE_D:
                            *crash = 1;
                            break; // never run
                    }
                    break;
                    // cases for other events
            }
        }

        SDL_Delay(50);
        if (nomore) continue;
        SDL_RenderClear(renderer);


        for (int y = 0; y < HEIGHT && !nomore; y+=pxp)
        {
            for (int x = 0; x < WIDTH && !nomore; x+=pxp)
            {
                unsigned int ch = getchar();
                if (ch == EOF)
                {
                    nomore = 1;
                    printf("No more\n");
                    break;
                }
                if (breakmode && ch == '\n')
                {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
                    break;
                }
                int rgb[3];
                rgb[0] = (0b111 & ch) * 32;
                rgb[1] = (0b111 & (ch >> 3)) * 32;
                rgb[2] = (0b11 & (ch >> 6)) * 32;
                int remaineder = (ch >> 8);
                SDL_SetRenderDrawColor(renderer, rgb[0], rgb[1], rgb[2], 255);

                if (pxp == 1)
                {
                    SDL_RenderDrawPoint(renderer, x, y);
                } else
                {
                    for (int y2 = 0; y2 < pxp; y2++)
                        for (int x2 = 0; x2 < pxp && !nomore; x2++)
                            SDL_RenderDrawPoint(renderer, x+x2, y+y2);
                }
            }
        }
        SDL_RenderPresent(renderer);
    }



    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
