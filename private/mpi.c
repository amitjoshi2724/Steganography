// 
// Torbert, 14 November 2016
// 
// MPI Demo
//    mpicc mpiDemo.c
//    time mpirun -np 4                                  a.out
//    time mpirun -np 4 --mca orte_base_help_aggregate 0 a.out
//    time mpirun -np 4 -mca btl ^openib                 a.out
// 
//    time mpirun -np 6 -machinefile hosts.txt a.out
// 
// Manager-Worker model for parallel processing.
// 
// 2 4   0.6283180000000000
// 3 4   0.9424770000000000
// 1 4   0.3141590000000000
// 
// real    0m19.140s
// user    1m3.061s
// sys     0m13.251s
// 
// htop -u smtorbert
// 
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "mpi.h"
// 
const char tree = 'T';
const char blank = ' ';
const char sparked = '*';
const char fire = 'W';
const char fence = '|';
int trials = 1000;
int height = 40;
int width = 30;
typedef struct Node
{
    double probability ;
    double steps ;
	//
} TreeNode ;
double myrand()
{
   return ( rand() % 100 ) / 100.0 ;
}
int tick(char m[width][height], int fireCount){
    
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            if(m[i][j] == fire){
                if(i > 0){
                    if(m[i-1][j] == tree){
                    m[i-1][j] = sparked;
                    fireCount++;
                    }
                }
                if(i < width - 1){
                    if(m[i+1][j] == tree){
                    m[i+1][j] = sparked;
                    fireCount++;
                    }
                }
                if(j > 0){
                    if(m[i][j-1] == tree){
                    m[i][j-1] = sparked;
                    fireCount++;
                    }
                }
                if(j < height - 1){
                    if(m[i][j+1] == tree){
                    m[i][j+1] = sparked;
                    
                    fireCount++;
                    }
                }
                m[i][j] = blank;
                fireCount--;
            }
        }
    }
    
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            if(m[i][j] == sparked){
                m[i][j] = fire;
            }
        }
    }
    return fireCount;
}
double run(double prob){
    double p = prob;
    int rseed = 2724;

	srand(rseed);
	    int totalStep = 0;
	    for(int counter = 0; counter < trials; counter++){
	        int step = 0;
        	char m[width][height];
        	int treeCount = 0;
        	for(int i = 0; i < width; i++){
        		for(int j = 0; j < height; j++){
        			if(myrand() >= p){
        				m[i][j] = blank;
                    }
        			else{
        			    if(p == 0.0){
        			        printf("%s\n", "wtf");
        			    }
        				m[i][j] = tree;
        				treeCount += 1;
        			}
        		}	
        	}
        	int fireCount = 0;
        	for(int i = 0; i < width; i++){
        	    if(m[i][0] == tree){
        	        m[i][0] = fire;
        	        fireCount++;
        	    }
        	}
        	while(fireCount > 0){
        	    step++;
        	    fireCount = tick(m, fireCount);
        	}
        	totalStep += step;
	    }
	    double avgstep = (double)(totalStep)/trials;
	    double normalizedsteps = ((double)(avgstep*1.0))/height;
	    //fprintf(fp, "%g\n", ((double)(avgstep*1.0))/height);
	    //p += deltaP;
    /*TreeNode* t = NULL;
    t = (TreeNode*)malloc( sizeof(TreeNode) );
    t -> probability = prob;
    t -> steps = ((double)(avgstep*1.0))/height;
    return t;*/
    return normalizedsteps;
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
   //
   // other variables
   //
   int        k , j  ;
   double     prob , nbt ;
   double tup[2];
   //
   // boilerplate
   //
   MPI_Init(      &argc          , &argv ) ;
   MPI_Comm_size( MPI_COMM_WORLD , &size ) ; // same
   
   MPI_Comm_rank( MPI_COMM_WORLD , &rank ) ; // different
   //
   // manager has rank = 0
   //
   prob = 0.0;
   double deltaP = 0.01;
   if( rank == 0 )
   {
      printf( "\n" ) ;
      //
      //
      int j = 1;
      while(prob < 1.01){
          MPI_Send( &prob , 1 , MPI_DOUBLE, j , tag , MPI_COMM_WORLD ) ; 
          //printf("sent \t%d\n", j);
          prob += deltaP;
          if(j < size-1){
              j += 1;
          }
          else{
              j = 1;
          }
      }
      for( k = 0 ; k < 1/(deltaP) +1; k++)
      {
          
         MPI_Recv( tup , 2 , MPI_DOUBLE , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
         //
         j = status.MPI_SOURCE;
         double probreceived = tup[0];
         nbt = tup[1];
         //
         //printf( "%d %d %20.16f\n" , j , size , nbt ) ;
         //printf( "%d\t%g %g\n" , j, probreceived, nbt);
      }
      prob = -1;
      //printf("after received everything");
      for(k = 1; k < size; k++){
          
          MPI_Send( &prob , 1 , MPI_DOUBLE, k , tag , MPI_COMM_WORLD ) ;
      }
      //
      printf( "\n" );
   }
   //
   // workers have rank > 0
   //
   else
   {
      for(int counter = 0; counter < (1/deltaP) + 1; counter++){
          MPI_Recv( &prob , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status ) ;
          if(prob == -1){
              break;
          }
          nbt = run(prob);
          double tup[2];
          tup[0] = prob;
          tup[1] = nbt;
          //
          /*for( k = 1 ; k < 100000 ; k++ )
          for( j = 1 ; j < 100000 ; j++ )
          {
             nbt = 0.314159 * rank ; // these are the worst workers ever
          }*/
          //
          //printf("%s\t%g\n", "sending back: ", prob);
          MPI_Send( tup , 2 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD ) ;
      }
   }
   //
   // boilerplate
   //
   MPI_Finalize() ;
   //
   return 0;
}
// 
// end of file
// 