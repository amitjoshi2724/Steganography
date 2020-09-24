#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#define PI 3.14159265358979323846
int main(){
    const double G = 6.67408e-11;
    const double Mm = 7.34767309e22;
    const double Me = 5.97219e24;
    const double Re = 6.371e6;
    const double altitude = 384400e3;
    const double speed = 1022.8275; //meters per seconds
    const double satSpeed = 1600; //m/s
    const double T = 2419000/4; 
    const double timeIncrement = 12; //seconds
    int steps = T/timeIncrement;
    double satelliteAltitude = altitude/2;
    double x[steps];
    double y[steps];
    double satx[steps];
    double saty[steps];
    double vx[steps];
    double vy[steps];
    double satvx[steps];
    double satvy[steps];
    double ax[steps];
    double ay[steps];
    double sataxe[steps];
    double sataye[steps];
    double sataxm[steps];
    double sataym[steps];
    double radius[steps];
    double satelliteRadius[steps];
    double angle = 23.4;
    x[0] = Re+altitude;
    y[0] = 0;
    satx[0] = cos(PI*angle/180)*(Re + satelliteAltitude);
    saty[0] = sin(PI*angle/180)*(Re + satelliteAltitude);
    int i = 0;
    double r = sqrt((x[i]*x[i]) + (y[i]*y[i]));
    double satRadius = sqrt((satx[i]*satx[i]) + (saty[i]*saty[i]));
    double a = -(G*Me)/(r*r);
    satvx[i] = cos(PI*angle/180)*satSpeed;
    satvy[i] = sin(PI*angle/180)*satSpeed;
    double sat = satx[0];
    ax[i] = a*(x[i]/r);
    ay[i] = a*(y[i]/r);
    sataxe[i] = (satx[i]/satRadius)*(-G*Me)/(satRadius*satRadius);
    sataye[i] = (saty[i]/satRadius)*(-G*Me)/(satRadius*satRadius);
    sataxm[i] = (satx[i]/satRadius)*(G*Mm)/(satRadius*satRadius);
    sataym[i] = (saty[i]/satRadius)*(G*Mm)/(satRadius*satRadius);
    vy[0] = speed;
    vx[0] = 0;
    radius[0] = r;
    printf("%g\n", vy[i]);
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
        
        satx[i] = satx[i-1] + (satvx[i-1]*timeIncrement);
        saty[i] = saty[i-1] + (satvy[i-1]*timeIncrement);
        
        satvx[i] = satvx[i-1] + ((sataxe[i-1]+sataxm[i-1])*timeIncrement);
        satvy[i] = satvy[i-1] + ((sataye[i-1]+sataym[i-1])*timeIncrement);
        
        satRadius = sqrt((satx[i]*satx[i]) + (saty[i]*saty[i]));
        satelliteRadius[i] = satRadius;
        
        sataxe[i] = (satx[i]/satRadius)*(-G*Me)/(satRadius*satRadius);
        sataye[i] = (saty[i]/satRadius)*(-G*Me)/(satRadius*satRadius);
        sataxm[i] = (satx[i]/satRadius)*(G*Mm)/(satRadius*satRadius);
        sataym[i] = (saty[i]/satRadius)*(G*Mm)/(satRadius*satRadius);
    }
    FILE* fp;
    fp = fopen("moonorbit.txt", "w");
    for(int i = 0; i < T/timeIncrement; i++){
        fprintf(fp, "%g\t%g\t%g\t%g\n", x[i], y[i], satx[i], saty[i]);
        //printf("%g\t%g\n", x[i], y[i]);   
    }
}
