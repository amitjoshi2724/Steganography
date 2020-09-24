#include <fstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath> 
#include <math.h>
#include <time.h>
#include <vector>
#include <chrono>
using namespace std;
int width = 2400;
int npoints = 31;
int height = 800;
ofstream myfile;
int matrix[800][2400];

vector<double> sevenLook(int xpoints[], int ypoints[], int npoints, double closestDistanceSquare){
    double closestDistance = sqrt(closestDistanceSquare);
    for(int i=0;i<npoints;++i) //start bubble sort
   {
      for(int j=1;j<npoints;++j)
      {
         if(xpoints[i] == -1 || xpoints[j] == -1){
             continue;
         }
         if(ypoints[i] < ypoints[j])
         {
            int temp = ypoints[i];
            int xtemp = xpoints[i];
            ypoints[i] = ypoints[j];
            xpoints[i] = xpoints[j];
            ypoints[j] = temp;
            xpoints[j] = xtemp;
         }
      }
   }   //end bubble sort
   cout << "ypoints: ";
   int incounter = 0;
   for(int i = 0; i < npoints; i++){
       if(ypoints[i] != -1){
           incounter++;
       }
       cout << std::to_string(ypoints[i]) + " ";
   }
   cout << "\n";
   vector<double> results(3);
   if(incounter < 2){
       results[0] = -1;
       results[1] = -1;
       results[2] = 99999999999999;
   }
   /*vector<double> xseven(7);
   vector<double> yseven(7);*/
   int real = -100;
   int i = 0;
   while(i < npoints){
       cout << std::to_string(i) + "\n";
       if(real == -100){
           if(ypoints[i] == -1){
               i += 1;
               cout << "continued\n";
               continue;
           }
           else{
               int real = i;
               cout << "set real to: " + std::to_string(real);
               int modifyingI = i;
               int yd = 0;
               do{
                   modifyingI += 1;
                   yd = ypoints[modifyingI] - ypoints[i];
                   double distanceSquare = ((xpoints[modifyingI]-xpoints[i])*(xpoints[modifyingI]-xpoints[i])) + ((ypoints[modifyingI]-ypoints[i])*(ypoints[modifyingI]-ypoints[i]));
                   if(distanceSquare < closestDistanceSquare){
                       closestDistanceSquare = distanceSquare;
                       results[0] = i;
                       results[1] = modifyingI;
                       results[2] = closestDistance;
                   }
               }
               while(yd < closestDistance);
               real += 1;
               i = real;
               continue;
           }
       }
       else{
           int real = i;
           cout << "in big else, set real to: " + std::to_string(real);
           int modifyingI = i;
           int yd = 0;
               do{
                   modifyingI += 1;
                   yd = ypoints[modifyingI] - ypoints[i];
                   double distanceSquare = ((xpoints[modifyingI]-xpoints[i])*(xpoints[modifyingI]-xpoints[i])) + ((ypoints[modifyingI]-ypoints[i])*(ypoints[modifyingI]-ypoints[i]));
                   if(distanceSquare < closestDistanceSquare){
                       closestDistanceSquare = distanceSquare;
                       results[0] = i;
                       results[1] = modifyingI;
                       results[2] = closestDistance;
                   }
               }
               while(yd < closestDistance);
            real += 1;
            if(real == -100 || real == -99){
                cout << "wtf\n";
            }
            i = real;
       }

   }
   return results;
   
}
vector<double> bruteForce(int brutexpoints[], int bruteypoints[], int brutenpoints){
	   double closestDistanceSquare = 99999999999999;
	   int finalI = -1;
	   int finalJ = -1;
	   for(int i = 0; i < brutenpoints; i++){
		  for(int j = 0; j < brutenpoints; j++){
			 if(i == j || brutexpoints[i] == -1 || brutexpoints[j] == -1){
				continue;
			 }
			 else{
				int x1 = brutexpoints[i];
				int y1 = bruteypoints[i];
				int x2 = brutexpoints[j];
				int y2 = bruteypoints[j];
				double d = ((x2-x1)*(x2-x1)) + ((y2-y1)*(y2-y1));
				if(d < closestDistanceSquare){
				   closestDistanceSquare = d;
				   finalI = i;
				   finalJ = j;
				}
			 }
		}
	   }
	  vector<double> results(3);
	  results[0] = finalI;
	  results[1] = finalJ;
	  results[2] = closestDistanceSquare;
	  return results;
}
void illuminate(int i, int j, bool b){
   if(i < 0 || j < 0 || i > 800 || j > 800){
      return;
   }
   j *= 3;
   
   for(int counter = 0; counter < 3; counter+=1){
      if((!b && counter != 0) or b){
         matrix[i][j] = 0;
      }
      j+=1;
   }
}

vector<double> rcp(int xpoints[], int ypoints[], int npoints, int closestDistanceSquare){
		if(npoints == 2 || npoints == 3){
			return bruteForce(xpoints, ypoints, npoints);
		}
	    int leftxpoints[npoints];
	   int rightxpoints[npoints];
	   int leftypoints[npoints];
	   int rightypoints[npoints];
	   for(int p = 0; p < npoints; p++){
		  leftxpoints[p] = -1;
		  rightxpoints[p] = -1;
		  leftypoints[p] = -1;
		  rightypoints[p] = -1;
	  }
	  
		for(int i = 0; i < npoints; i++){
		  if(i >= (npoints/2)){
			 rightxpoints[i - (npoints/2)] = xpoints[i];
			 rightypoints[i - (npoints/2)] = ypoints[i];
		  }
		  else{
			 leftxpoints[i] = xpoints[i];
			 leftypoints[i] = ypoints[i];
		  }
	   }
	   int leftlength = (npoints/2);
	   int rightlength = -1;
	   if(npoints%2 == 0){
		   rightlength = (npoints/2);
		   }
		   else{
			   rightlength = (npoints/2)+1;
			   }
		vector<double> leftresults = rcp(leftxpoints, leftypoints, leftlength, closestDistanceSquare);
		vector<double> rightresults = rcp(rightxpoints, rightypoints, rightlength, closestDistanceSquare);
		vector<double> results(3);
		if(leftresults[2] <= rightresults[2]){
			results[0] = (int)leftresults[0];
			results[1] = (int)leftresults[1];
			results[2] = leftresults[2];
		}
		else{
			results[0] = (int)(rightresults[0] + (npoints/2));
			results[1] = (int)(rightresults[1] + (npoints/2));
			results[2] = rightresults[2];
		}
		return results;
	}
int main(void) {
   myfile.open ("closest_pair.ppm");
   for(int i = 0; i < 800; i++){
      for(int j = 0; j < 2400; j++){
         matrix[i][j] = 1;
      }
   }
   myfile << "P3 800 800 1\n";
   srand(chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
   int xpoints[30];
   int ypoints[30];
   int c = rand()%800;
   for(int i = 0; i < npoints; i++){
      int x = rand()%800;
      int y = rand()%800;
      xpoints[i] = x;
      ypoints[i] = y;
      if(x > 0){
         illuminate(x-1, y, true);
      }
      if(x < 799){
         illuminate(x+1, y, true);
      }
      if(y > 0){
         illuminate(x, y-1, true);
      }
      if(y < 799){
         illuminate(x, y+1, true);
      }
      illuminate(x, y, true);
   }
   //all brute force
   /*for(int i = 0; i < npoints; i++){
      for(int j = 0; j < npoints; j++){
         if(i == j){
            continue;
         }
         else{
            int x1 = xpoints[i];
            int y1 = ypoints[i];
            int x2 = xpoints[j];
            int y2 = ypoints[j];
            double d = ((x2-x1)*(x2-x1)) + ((y2-y1)*(y2-y1));
            if(d < closestDistanceSquare){
               closestDistanceSquare = d;
               finalI = i;
               finalJ = j;
            }
         }
      }
   }*/
   
   //brute force in the middle
   
   
   for(int i=0;i<npoints;++i) //start bubble sort
   {
      for(int j=1;j<npoints;++j)
      {
         if(xpoints[i] > xpoints[j])
         {
            int temp = xpoints[i];
            int ytemp = ypoints[i];
            xpoints[i] = xpoints[j];
            ypoints[i] = ypoints[j];
            xpoints[j] = temp;
            ypoints[j] = ytemp;
         }
      }
   }   //end bubble sort
   cout << "after bubble sort\n";
   int rightxpoints[npoints];
   int rightypoints[npoints];
   int leftxpoints[npoints];
   int leftypoints[npoints];
   for(int i = 0; i < npoints; i++){
      if(i >= (npoints/2)){
         rightxpoints[i - (npoints/2)] = xpoints[i];
         rightypoints[i - (npoints/2)] = ypoints[i];
      }
      else{
         leftxpoints[i] = xpoints[i];
         leftypoints[i] = ypoints[i];
      }
   }
    int leftlength = (npoints/2);
    int rightlength = -1;
	if(npoints%2 == 0){
	    rightlength = (npoints/2);
    }
    else{
	    rightlength = (npoints/2)+1;
	}
   double closestDistanceSquare = 99999999999999;
   int finalI = -1;
   int finalJ = -1;
   closestDistanceSquare = 9999999999;
   cout << "right before left rcp\n";
   vector<double> leftresults = rcp(leftxpoints, leftypoints, leftlength, closestDistanceSquare);
   cout << "right after left rcp and right before right rcp\n";
   vector<double> rightresults = rcp(rightxpoints, rightypoints, rightlength, closestDistanceSquare);
   cout << "right after right rcp\n";
   double results[3];
   if(leftresults[2] <= rightresults[2]){
		results[0] = leftresults[0];
		results[1] = leftresults[1];
		results[2] = leftresults[2];
	}
	else{
		results[0] = rightresults[0] + (npoints/2);
		results[1] = rightresults[1] + (npoints/2);
		results[2] = rightresults[2];
	}
	cout << "after side setting\n";
	finalI = (int)results[0];
	finalJ = (int)results[1];
	closestDistanceSquare = results[2];
   int centerxpoints[npoints];
   int centerypoints[npoints];
   double median = 0.0;
   if(npoints%2 == 0){
      median = ((double)(xpoints[npoints/2] + xpoints[(npoints/2) - 1]))/2.0;
   }
   else{
      median = (double)xpoints[npoints/2];
   }
   double leftBound = median - sqrt(closestDistanceSquare);
   double rightBound = median + sqrt(closestDistanceSquare);
   for(int p = 0; p < npoints; p++){
	  if(xpoints[p] >= leftBound && xpoints[p] <= rightBound){
			centerxpoints[p] = xpoints[p];
			centerypoints[p] = ypoints[p];
		  }
	  else{
		centerxpoints[p] = -1;
		centerypoints[p] = -1;
      }
   }
   cout << "after setting centerxpoints and centerypoints\n";
   vector<double> centerResults(3);
   centerResults = bruteForce(centerxpoints, centerypoints, npoints);
   //centerResults = sevenLook(centerxpoints, centerypoints, npoints, closestDistanceSquare);
   if(centerResults[2] <= results[2]){
       results[0] = centerResults[0];
       results[1] = centerResults[1];
       results[2] = centerResults[2];
   }
   cout << "after potential center setting\n";
   	finalI = results[0];
	finalJ = results[1];
    cout << std::to_string(finalI) + "\n";
   cout << std::to_string(finalJ) + "\n";
   cout << "after finalI and finalJ setting\n";
   int s = 5;
   
   //drawing rectangle around finalI
   int x = xpoints[finalI];
   int y = ypoints[finalI];
   x -= s;
   y -= s;
   for(int i = x, j = y; i < x+(2*s); i++){
       illuminate(i, j, false);
   }
   x += (2*s);
   for(int i = x, j = y; j < y+(2*s); j++){
       illuminate(i, j, false);
   }
   y += (2*s);
   for(int i = x, j = y; i > x-(2*s); i--){
       illuminate(i, j, false);
   }
   x -= (2*s);
   for(int i = x, j = y; j > y-(2*s); j--){
       illuminate(i, j, false);
   }
   y -= (2*s);
   
   //drawing rectangle around finalJ
   x = xpoints[finalJ];
   y = ypoints[finalJ];
   x -= s;
   y -= s;
   for(int i = x, j = y; i < x+(2*s); i++){
       illuminate(i, j, false);
   }
   x += (2*s);
   for(int i = x, j = y; j < y+(2*s); j++){
       illuminate(i, j, false);
   }
   y += (2*s);
   for(int i = x, j = y; i > x-(2*s); i--){
       illuminate(i, j, false);
   }
   x -= (2*s);
   for(int i = x, j = y; j > y-(2*s); j--){
       illuminate(i, j, false);
   }
   y -= (2*s);
   cout << "did both of the illumination\n";
    for(int i = 0; i < 800; i++){
      cout << std::to_string(i) + "\n";
      for(int j = 0; j < 2400; j++){
         myfile << std::to_string(matrix[i][j]);
         myfile << " ";
      }
      myfile << "\n";
   }
   cout << "here\n";
   myfile.close();
}
