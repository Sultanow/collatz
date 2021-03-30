/*CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                             C
C  COMPUTE NUMBER OF FRACTIONS						      C
C  12/19/14 (DKC)							      C
C                                                                             C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC*/
#include <math.h>
#include <stdlib.h>
unsigned int haros7(unsigned int N, unsigned int M,
		    unsigned int H, unsigned int K, unsigned int HP,
		    unsigned int KP, unsigned int D, unsigned int *ND);
void mertenaw(unsigned int N, unsigned int *count, unsigned int I) {
unsigned int count1,count2,J,ND[2];
J=I/2;
if (N<=J) {
   count[0]=0;
   count[1]=0;
   return;
   }
if ((N>=(J+1))&&(N<I)) {
   count[0]=0;
   count[1]=N-J;
   return;
   }
if (N==I) {
   count[0]=0;
   if ((I&1)==0)
      count[1]=J-1;
   else
      count[1]=J;
   return;
   }
if (N>I) {
   count1=haros7(N,0,0,1,1,N,I,ND);
   count1=count1-2;
   count[0]=count1;
   if ((I&1)==0)
      count2=haros7(N,0,1,I,ND[0],ND[1],J,ND);
   else
      count2=haros7(N,0,1,I,ND[0],ND[1],I,ND);
   count2=count2-2;
   count[1]=count2;
   return;
   }
return;
}
