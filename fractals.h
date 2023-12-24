#ifndef COMPLEX_PLANE_H
#define COMPLEX_PLANE_H

// * HEADERS
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <math.h>
#include <Windows.h>
#include <processthreadsapi.h>

// * MACRODEFINITIONS
#define SCREEN_ASPECT_RATIO (16.0/9.0)
#define SCREEN_HEIGHT (1080)
#define SCREEN_WIDTH ((int)((SCREEN_HEIGHT)*SCREEN_ASPECT_RATIO))
#define MAX_ITERATIONS 1000
#define FRAMES 1
#define FRAME_START 0
#define ZOOM_FACTOR 0.06

// Flags
#define PRESENT_EACH_FRAME
// #define SAVE_EACH_FRAME
// #define JULIA

// * TYPE DEFINITIONS
typedef struct {
    double r;
    double g;
    double b;
} RGB;

// * GLOBAL VARIABLES
int pixelIterations[SCREEN_WIDTH][SCREEN_HEIGHT];
long double pixelIterationsSmooth[SCREEN_WIDTH][SCREEN_HEIGHT]; // Not working yet :(

#ifdef JULIA
    long double ReCJulia = -0.21421801;
    long double ImCJulia = 0.684992101;

    double startRe = -0.75;
    double endRe = -0.75;
    
    // Traverse 
    double startIm = 0.25; 
    double endIm = 0.25; 
#endif

struct Plane {
    long double width;
    long double height;
    long double aspect_ratio;
    struct {long double x; long double y;} coords;
} plane = {4.0, 9.0/4.0, SCREEN_ASPECT_RATIO, .coords = {-2.0, 9.0/8.0}}; // Center
// } plane = {4.499261, 2.999508, SCREEN_ASPECT_RATIO, .coords = {-3.499261, 1.347624}};
// } plane = {3.483891908, 2.32259460533, SCREEN_ASPECT_RATIO, .coords = {-2.483891908, 1.01824768667}};
// } plane = {5.074826559, 3.383217706, 3.0/2.0, .coords = {-2.8264783605, 1}}; // Zoom 45 frames at 0.06

struct Mouse {
    int down;
    int x;
    int y;
    int down_x;
    int down_y;
    int up_x;
    int up_y;
} mouse = {0};

// * FUNCTION PROTOTYPES
void handleEvents(int *running);
void quit(SDL_Renderer *renderer, SDL_Window *window);
void clear(SDL_Renderer *renderer);

int saveScreenshot(const char *filename, SDL_Renderer *renderer);

double **colourGradient(const double rgb_start[3], const double rgb_mid[3], const double rgb_end[3], int steps);
void zoom_in();
void moveJulia();
void draw_set(SDL_Renderer *renderer);

typedef struct {int q;} quarterArgs;
DWORD WINAPI computeIterations(LPVOID lpParam);

#endif
