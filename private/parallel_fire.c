#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "mpi.h"
//
double p = 0.6;
double deltaP = 0.01;

int height = 40;
int width = 30;
double trials = 1000;
const char tree = 'T';
const char blank = ' ';
const char sparked = '*';
const char fire = 'W';
const char fence = '|';

int usleep(useconds_t useconds);


double myrand()
{
   return ( rand() % 100 ) / 100.0 ;
}
void printBoard(char m[width][height]){
    
    for(int i = -1; i <= width; i++){
        if(i == -1 || i == width){
            printf("%c", '\\');
        }
        for(int j = 0; j < height; j++){
            if(i == -1 || i == width){
                printf("%c", '-');
            }
            else{
                if(j == 0){
                    printf("%c", fence);
                }
                printf("%c", (char)m[i][j]);
                if(j == height-1){
                    printf("%c", fence);
                }
            }
        }
        if(i == -1 || i == width){
            printf("%c", '/');
        }
        printf("%s", "\n");
    }
    printf("%s", "\n");
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

int main(int prob){
    p = prob;
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
	    
	    //fprintf(fp, "%g\n", ((double)(avgstep*1.0))/height);
	    //p += deltaP;
	    return p, ((double)(avgstep*1.0))/height;
}
//normalize -> step*1.0/width
//count trees

