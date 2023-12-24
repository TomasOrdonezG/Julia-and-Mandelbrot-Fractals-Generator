# Julia-and-Mandelbrot-Fractals-Generator
Renders Mandelbrot sets and Julia sets. Able to save frames, zoom in, animate Julia sets, etc.

Change macrodefinitions in fractals.h to render images to your liking
- MAX_ITERATIONS: Iteration cap for each pixel
- FRAMES: How many frames to render
- FRAME_START: How many frames to skip before first render
- ZOOM_FACTOR: How much the height decreases per frame (mandelbrot only)
- SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_ASPECT_RATIO: Height and width of the display in pixels, and the aspect ratio

Flags:
- PRESENT_EACH_FRAME: Show each frame after it is rendered on the display
- SAVE_EACH_FRAME: Save each frame to ./Frames/#.bmp where # is the frame number starting from 1

You can also change the 3 colour gradient inside the draw_set function at the top if you wish

Compilation
- Compile with -DJULIA to render julia sets, or run Julia.exe.
- Compile without -DJULIA to render the Mandelbrot set, or run Mandelbrot.exe.

The already compiled executable files will give you the following renders:

# Julia Set Render
![Julia Render](https://github.com/TomasOrdonezG/Julia-and-Mandelbrot-Fractals-Generator/blob/main/JuliaSample.png)

# Mandelbrot set Render
![Mandelbrot Render](https://github.com/TomasOrdonezG/Julia-and-Mandelbrot-Fractals-Generator/blob/main/MandelbrotSample.png)

