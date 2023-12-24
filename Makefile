FLAGS = -Isrc/include/SDL2 -Lsrc/lib -Wall -std=c99 -lmingw32 -lSDL2main -lSDL2 -lm

runJulia: clean Julia
	./Julia

runMandelbrot: clean Mandelbrot
	./Mandelbrot

Julia:
	gcc fractals.c -DJULIA ${FLAGS} -o Julia

Mandelbrot:
	gcc fractals.c ${FLAGS} -o Mandelbrot

clean:
	del Mandelbrot.exe Julia.exe
