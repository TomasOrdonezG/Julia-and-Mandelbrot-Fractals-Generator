#include "fractals.h"

int main(int argc, char **argv)
{
    SDL_Window *window = SDL_CreateWindow("Mandelbrot Set", SDL_WINDOWPOS_UNDEFINED, 20, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    // Create Threads
    HANDLE threads[4];
    DWORD threadIDs[4];
    quarterArgs myArgs[4] = {{1}, {2}, {3}, {4}};

    #ifndef JULIA
        plane.coords.x = -2.5;
        for (int i = 0; i < FRAME_START; i++) zoom_in();
    #else
        ReCJulia = startRe;
        ImCJulia = startIm;
        for (int i = 0; i < FRAME_START; i++) moveJulia();
    #endif
    
    // Iterate over each frame
    int running = 1;
    for (int frame_i = FRAME_START; frame_i < (FRAME_START + FRAMES) && running; frame_i++) {
        #ifndef JULIA
            zoom_in();
        #else
            moveJulia();
        #endif
        clear(renderer);

        // Run each thread for each screen section
        for (int i = 0; i < 4; i++) {
            if ((threads[i] = CreateThread(NULL, 0, computeIterations, &(myArgs[i]), 0, &(threadIDs[i]))) == NULL) {
                printf("Thread %d creation failed\n", i+1);
                exit(1);
            }
        }
        // Wait for threads to finish generating the set and then close them
        WaitForMultipleObjects(4, threads, TRUE, INFINITE);
        for (int i = 0; i < 4; i++) CloseHandle(threads[i]);

        // Draw the set
        draw_set(renderer);

        #ifdef PRESENT_EACH_FRAME // Present the image to the display
            SDL_RenderPresent(renderer);
            handleEvents(&running);
        #endif

        #ifdef SAVE_EACH_FRAME // Save current frame as a PNG
            char name[30];
            sprintf(name, "Frames/%d.bmp", frame_i+1);
            saveScreenshot(name, renderer);
            printf("%d/%d frames rendered\n", frame_i+1, FRAMES);
        #endif
    }

    // Display after done generating set
    draw_set(renderer);
    SDL_RenderPresent(renderer);
    while (running) handleEvents(&running);

    quit(renderer, window);
    return 0;
}

void handleEvents(int *running)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Quit 
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            *running = 0;
        }

        // Mouse up and down positions
        SDL_GetMouseState(&mouse.x, &mouse.y);
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            mouse.down = 1;
            SDL_GetMouseState(&mouse.down_x, &mouse.down_y);
        }
        if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
            mouse.down = 0;
            SDL_GetMouseState(&mouse.up_x, &mouse.up_y);
        }
    } 
}

void quit(SDL_Renderer *renderer, SDL_Window *window)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return;
}

void clear(SDL_Renderer *renderer)
{
    // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    return;
}

int saveScreenshot(const char *filename, SDL_Renderer *renderer)
{
    // Used temporary variables
    SDL_Rect _viewport;
    SDL_Surface *_surface = NULL;
    
    // Get viewport size
    SDL_RenderGetViewport( renderer, &_viewport);
    
    // Create SDL_Surface with depth of 32 bits
    _surface = SDL_CreateRGBSurface( 0, _viewport.w, _viewport.h, 32, 0, 0, 0, 0 );
    
    // Check if the surface is created properly
    if ( _surface == NULL ) {
        printf("Cannot create SDL_Surface: %s\n", SDL_GetError());
        return 0;
    }
    
    // Get data from SDL_Renderer and save them into surface
    if ( SDL_RenderReadPixels( renderer, NULL, _surface->format->format, _surface->pixels, _surface->pitch ) != 0 ) {
        printf("Cannot read data from SDL_Renderer: %s\n", SDL_GetError());
        
        // Don't forget to free memory
        SDL_FreeSurface(_surface);
        return 0;
    }
    
    // Save screenshot as BMP file
    if (SDL_SaveBMP( _surface, filename) != 0 ) {
        printf("Cannot save BMP file: %s\n", SDL_GetError());
        
        // Free memory
        SDL_FreeSurface(_surface);
        return 0;
    }
    
    // Free memory
    SDL_FreeSurface(_surface);
    return 1;
}

void zoom_in()
{
    // Resize screen and zoooooom in
    float dheight = plane.height * ZOOM_FACTOR;

    plane.coords.y -= (dheight / 2.0);
    plane.height -= dheight;

    plane.coords.x += (SCREEN_ASPECT_RATIO * dheight / 2.0);
    plane.width -= (dheight * SCREEN_ASPECT_RATIO);

    return;
}

void moveJulia()
{
    #ifdef JULIA
        double Re_per_frame = (endRe-startRe) / (double)FRAMES;
        ReCJulia += Re_per_frame;
        
        double Im_per_frame = (endIm-startIm) / (double)FRAMES;
        ImCJulia += Im_per_frame;
    #endif
    return;
}

void draw_set(SDL_Renderer *renderer)
{
    float s = 0.8;
    #ifdef JULIA
        s = 1;
    #endif
    int steps = 2000;
    double rgbi[3] = {0x10, 0x10, 0x10};
    double rgbm[3] = {255, 0x34, 0x93};
    double rgbf[3] = {255, 255, 0};
    double **gradient = colourGradient(rgbi, rgbm, rgbf, steps); 
    
    for (int py = 0; py < SCREEN_HEIGHT; py++) for (int px = 0; px < SCREEN_WIDTH; px++) {
        int i = pixelIterations[px][py];

        // Get colour and render
        if (i == MAX_ITERATIONS)
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        else
        {
            int index = fmod(pow(pow(i / (float)MAX_ITERATIONS, s) * steps, 1.5), steps);
            // int index = steps / (float)MAX_ITERATIONS * i;
            double *c = gradient[index];

            SDL_SetRenderDrawColor(renderer, c[0], c[1], c[2], 255);
            // double alpha = 255.0 * (pow(i, s) / (double)pow(MAX_ITERATIONS, s));
            // SDL_SetRenderDrawColor(renderer, alpha, alpha, alpha, 255);
        }
        SDL_RenderDrawPointF(renderer, px, py);
    }

    for (int i = 0; i < steps; i++)
        free(gradient[i]);
    free(gradient);

    return;
}

DWORD WINAPI computeIterations(LPVOID lpParam)
{
    // Deconstruct parameters
    int q = ((quarterArgs*)lpParam)->q;

    // Recalculate complex plane density to display resolution
    long double re_p_x = plane.width / (double)SCREEN_WIDTH;
    long double im_p_y = plane.height / (double)SCREEN_HEIGHT;

    // Calculate boundaries for the appropiate section
    int screen_top = (q-1)*(SCREEN_HEIGHT/4);
    int screen_bottom = q*(SCREEN_HEIGHT/4);
    long double cPlane_top = plane.coords.y - (q-1)*(plane.height/4);
    long double cPlane_bottom = plane.coords.y - q*(plane.height/4);
    

    // Calculate iterations for each pixel
    int x, y;
    long double ReC, ImC;
    for (x = 0, ReC = plane.coords.x; ReC < plane.coords.x + plane.width && x < SCREEN_WIDTH; ReC += re_p_x, x++) {
        for (y = screen_top, ImC = cPlane_top; ImC > cPlane_bottom && y < screen_bottom; ImC -= im_p_y, y++) {
            int iterations = 0;

            #ifndef JULIA
                // * MANDELBROT Set Render
                if (sqrt(pow(ReC + 0.25, 2) + pow(ImC, 2)) < 0.5 || sqrt(pow(ReC + 1, 2) + pow(ImC, 2)) < 0.25) {
                    // Check main circles in the set
                    iterations = MAX_ITERATIONS;
                } else { // Escape time algorithm
                    long double ReZ = 0, ImZ = 0, ReZ2 = 0, ImZ2 = 0;

                    for (; ReZ2 + ImZ2 <= 4 && iterations < MAX_ITERATIONS; iterations++) {
                        ImZ = (ReZ + ReZ)*ImZ + ImC;
                        ReZ = ReZ2 - ImZ2 + ReC;
                        
                        ImZ2 = ImZ*ImZ;
                        ReZ2 = ReZ*ReZ;
                    }
                }
            #else
                // * JULIA Set Render
                long double ReZ = ReC, ImZ = ImC, ReZ2 = ReZ*ReZ, ImZ2 = ImZ*ImZ;

                for (; ReZ2 + ImZ2 <= 4 && iterations < MAX_ITERATIONS; iterations++) {
                    ImZ = (ReZ + ReZ)*ImZ + ImCJulia;
                    ReZ = ReZ2 - ImZ2 + ReCJulia;
                    
                    ImZ2 = ImZ*ImZ;
                    ReZ2 = ReZ*ReZ;
                }
            #endif

            pixelIterations[x][y] = iterations;
        }
    }

    return 0;
}

double **colourGradient(const double rgb_start[3], const double rgb_mid[3], const double rgb_end[3], int steps)
{
    // Allocate memory to gradient array
    double **gradient = malloc(sizeof(double*) * steps);
    for (int i = 0; i < steps; i++) {
        gradient[i] = malloc(sizeof(double) * 3);
    }

    // Copy initial colour
    for (int i = 0; i < 3; i++) {
        gradient[0][i] = rgb_start[i];
    }

    double drgb1[3], drgb2[3], drgb3[3];
    for (int i = 0; i < 3; i++) drgb1[i] = (rgb_mid[i] - rgb_start[i]) / (float)(steps/3.0);
    for (int i = 0; i < 3; i++) drgb2[i] = (rgb_end[i] - rgb_mid[i]) / (float)(steps/3.0);
    for (int i = 0; i < 3; i++) drgb3[i] = (rgb_start[i] - rgb_end[i]) / (float)(steps/3.0);

    // Generate gradient
    for (int step_i = 1; step_i < (steps/3); step_i++) {
        for (int i = 0; i < 3; i++) {
            gradient[step_i][i] = gradient[step_i - 1][i] + drgb1[i];
        }
    }
    for (int step_i = (steps/3); step_i < (2*steps/3); step_i++) {
        for (int i = 0; i < 3; i++) {
            gradient[step_i][i] = gradient[step_i - 1][i] + drgb2[i];
        }
    }
    for (int step_i = (2*steps/3); step_i < steps; step_i++) {
        for (int i = 0; i < 3; i++) {
            gradient[step_i][i] = gradient[step_i - 1][i] + drgb3[i];
        }
    }

    return gradient;
}
