/******************************************************************************
*									      *
*  COMPUTE MOBIUS FUNCTION						      *
*  04/02/10 (dkc)							      *
*									      *
******************************************************************************/
#include <math.h>
double mobius(unsigned int a, unsigned int *table) {
unsigned int i,count,p;
if (a==1)
   return(1);
for (i=0; i<17984; i++) {
   p=table[i];
   if (p>a)
      break;
   if (a==(a/p)*p) {
      a=a/p;
      if (a==(a/p)*p)
	 return(0.0);
      count=count+1;
      if (a==1)
	 break;
      }
   }
if ((count&1)==0)
   return(1.0);
else
   return(-1.0);
}
