#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
int main(){
    const double G = 6.67408e-11;
    const double Me = 5.97219e24;
    const double Re = 6.371e6;
    const double altitude = 400e3;
    const double speed = 7777.7778;
    const double T = 15000; 
    const double timeIncrement = .25; //seconds
    int steps = T/timeIncrement;
    double x[steps];
    double y[steps];
    double vx[steps];
    double vy[steps];
    double ax[steps];
    double ay[steps];
    double radius[steps];
    x[0] = 0;
    y[0] = Re+altitude;
    int i = 0;
    double r = sqrt((x[i]*x[i]) + (y[i]*y[i]));
    double a = -(G*Me)/(r*r);
    ax[i] = a*(x[i]/r);
    ay[i] = a*(y[i]/r);
    vx[0] = 1.5*sqrt(G*Me/(r));
    vy[0] = 0;
    radius[0] = r;
    printf("%g\n", r);
    for(i = 1; i < T/timeIncrement; i++){
        x[i] = x[i-1] + (vx[i-1]*timeIncrement);
        y[i] = y[i-1] + (vy[i-1]*timeIncrement);
        
        vx[i] = vx[i-1] + (ax[i-1]*timeIncrement);
        vy[i] = vy[i-1] + (ay[i-1]*timeIncrement);
        double r = sqrt((x[i]*x[i]) + (y[i]*y[i]));
        radius[i] = r;
        double a = -(G*Me)/(r*r);
        ax[i] = a*(x[i]/r);
        ay[i] = a*(y[i]/r);
        //printf("%g\n", r);
    }
    FILE* fp;
    fp = fopen("hyperbolic_orbital.txt", "w");
    for(int i = 0; i < T/timeIncrement; i++){
        fprintf(fp, "%g\t%g\t%d\t%g\t%g\t%g\t%g\t%g\n", x[i], y[i], i, vx[i], vy[i], sqrt((vx[i]*vx[i]) + (vy[i]*vy[i])), radius[i], Re);
        //printf("%g\t%g\n", x[i], y[i]);   
    }
}
