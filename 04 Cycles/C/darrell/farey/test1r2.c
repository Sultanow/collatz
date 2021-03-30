/*CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                             C
C  COMPUTE SUM OF EULER'S PHI FUNCTION                                        C
C  12/23/14 (DKC)							      C
C                                                                             C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC*/
#include <stdio.h>
#include <math.h>
unsigned int euclid(unsigned int d, unsigned int e);
//
unsigned int order=100;
unsigned int out=1;   // losum, hisum, hisum-losum, sqrt((hisum-losum))
unsigned int n=4;     // divisor (usually 4)
//
void main() {
unsigned int g,h,j,k,l,sum;
double losum,hisum;
FILE *Outfp;
Outfp = fopen("out1r2.dat","w");
losum=0.0;
hisum=1.0;   // changed from 0 on 08/19/14
for (g=2; g<=order; g++) {
   sum=1;
   for (j=2; j<g; j++) {
      k=euclid(g,j);
      if (k==1)
	 sum=sum+1;
      }
   l=sum/n;
   h=l;
   if (sum!=l*n)
      h=h+1;
   losum=losum+(double)l;
   hisum=hisum+(double)h;
   printf(" %d %d %e %e \n",g,sum,losum,hisum);
   if (out==0)
      fprintf(Outfp," %e\n",losum);
   else {
      if (out==1)
	 fprintf(Outfp," %e\n",hisum);
      else {
	 if (out==2)
	    fprintf(Outfp," %e\n",hisum-losum);
	 else {
	    if (out==3)
	       fprintf(Outfp," %e\n",sqrt(hisum-losum));
	    }
	 }
      }
   }
fclose(Outfp);
return;
}
