#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#define PI 3.14159265358979323846
typedef struct Node
{
    double probability ;
    double steps ;
	//
} TreeNode ;

double run(double angle, double satSpeed){
	     double G = 6.67408e-11;
     double Mm = 7.34767309e22;
     double Me = 5.97219e24;
     double Re = 6.371e6;
     double altitude = 384400e3;
     double speed = 1022.8275; //meters per seconds
    
     double T = 2419000/5; 
     double timeIncrement = 8; //seconds
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
    double moonR = pow(pow(satx[i]-x[i], 2) + pow(saty[i]-y[i], 2), .5);
	sataxm[i] = ((x[i] - satx[i])/moonR)*(G*Mm)/(pow(satx[i]-x[i], 2) + pow(saty[i]-y[i], 2));
    sataym[i] = ((y[i] - saty[i])/moonR)*(G*Mm)/(pow(satx[i]-x[i], 2) + pow(saty[i]-y[i], 2));
    vy[0] = speed;
    vx[0] = 0;
    radius[0] = r;
    for(i = 1; i < (int)(T/timeIncrement); i++){
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
        double moonR = pow(pow(satx[i]-x[i], 2) + pow(saty[i]-y[i], 2), .5);
        sataxm[i] = ((x[i] - satx[i])/moonR)*(G*Mm)/(pow(satx[i]-x[i], 2) + pow(saty[i]-y[i], 2));
        sataym[i] = ((y[i] - saty[i])/moonR)*(G*Mm)/(pow(satx[i]-x[i], 2) + pow(saty[i]-y[i], 2));
    }
    if(satvx[(int)(T/timeIncrement)-1] < 0 && satvy[(int)(T/timeIncrement)-1] < 0){
		return 1.0;
		}
	else{
		return 0.0;
		}
	}
int main( int argc , char* argv[] )
{
   //
   // MPI variables
   //
   int        rank    ;
   int        size    ;
   MPI_Status status  ;
   int        tag = 0 ;
      int        k , j  ;
      double     prob , nbt ;
      double tup[3];
      double sending[2];
   //
   // other variables
   //
      MPI_Init(      &argc          , &argv ) ;
   MPI_Comm_size( MPI_COMM_WORLD , &size ) ; // same
   
   MPI_Comm_rank( MPI_COMM_WORLD , &rank ) ; // different

    if( rank == 0 )
   {
      printf( "\n" ) ;
      //
      //
      
      int j = 1;
        for(double angle = 30; angle < 40; angle += 0.1){
			for(double satSpeed = 1500; satSpeed < 1600; satSpeed += 10){
				double sending[2];
				sending[0] = angle;
				sending[1] = satSpeed;
				MPI_Send( sending , 2 , MPI_DOUBLE, j , tag , MPI_COMM_WORLD ) ;
			}
		  if(j < size-1){
              j += 1;
          }
          else{
              j = 1;
          }
		}
      for( k = 0 ; k < 1000; k++)
      {
          
         MPI_Recv( tup , 3, MPI_DOUBLE , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
         //
         j = status.MPI_SOURCE;
         double angleReceived = tup[0];
         double satSpeedReceived = tup[1];
         double nbt = tup[2];
         if(nbt == 1.0){
			printf("%g\t%g\n", angleReceived, satSpeedReceived);
			break;
		 }
      }
      //printf("after received everything");
      for(k = 1; k < size; k++){
		  double sending[2];
				sending[0] = 0; //0 is the signal that tells it to stop
				sending[1] = 0;
          MPI_Send( sending , 2 , MPI_DOUBLE, k , tag , MPI_COMM_WORLD ) ;
      }
      //
      printf( "\n" );
   }
   
      else //for the workers
   {
      for(int counter = 0; counter < 1000; counter++){
          MPI_Recv( sending, 2 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status ) ;
          if(sending[1] == 0){
              break;
          }
          double nbt = run(sending[0], sending[1]); //1.0 if it hit earth, 0.0 if it didn't hit earth
          double tup[3];
          tup[0] = sending[0];
          tup[1] = sending[1];
          tup[2] = nbt;
          MPI_Send( tup , 3 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD ) ;
      }
   }

   //
   // boilerplate
   //

   
}
