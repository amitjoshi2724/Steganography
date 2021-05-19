#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void color(int red, int green, int blue)
{
    fputc((char)red, stdout);
    fputc((char)green, stdout);
    fputc((char)blue, stdout);
}

int main(int argc, char *argv[])
{
    int w = 600, h = 400, x, y; 
    //each iteration, it calculates: newz = oldz*oldz + p, where p is the current pixel, and oldz stars at the origin
    double pr, pi;                   
    double newRe, newIm, oldRe, oldIm;   
    double zoom = 1, moveX = -0.5, moveY = 0; 
    int maxIterations = 1000;

    clock_t begin, end;
    double time_spent;

    printf("P6\n# CREATOR: E.T / mandel program\n");
    printf("%d %d\n255\n",w,h);

    begin = clock();

    //loop through every pixel
    for(y = 0; y < h; y++) 
    for(x = 0; x < w; x++)
    {
        //calculate the initial real and imaginary part of z, based on the pixel location and zoom and position values
    pr = 1.5 * (x - w / 2) / (0.5 * zoom * w) + moveX;
        pi = (y - h / 2) / (0.5 * zoom * h) + moveY;
        newRe = newIm = oldRe = oldIm = 0; //these should start at 0,0
        //"i" will represent the number of iterations
        int i;
        //start the iteration process
        for(i = 0; i < maxIterations; i++)
        {
            //remember value of previous iteration
            oldRe = newRe;
            oldIm = newIm;
            //the actual iteration, the real and imaginary part are calculated
            newRe = oldRe * oldRe - oldIm * oldIm + pr;
            newIm = 2 * oldRe * oldIm + pi;
            //if the point is outside the circle with radius 2: stop
            if((newRe * newRe + newIm * newIm) > 4) break;
        }

        color(i % 256, 255, 255 * (i < maxIterations));

    }

    end = clock();

    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Elapsed time: %.2lf seconds.\n", time_spent);

    return 0;
}