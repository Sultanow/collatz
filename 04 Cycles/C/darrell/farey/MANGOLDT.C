/******************************************************************************
*									      *
*  COMPUTE MANGOLDT FUNCTION						      *
*  04/02/10 (dkc)							      *
*									      *
******************************************************************************/
#include <math.h>
double mangoldt(unsigned int a, unsigned int *table) {
unsigned int i,count,p,savep;
if (a==1)
   return(0.0);
count=0;
savep=0;
for (i=0; i<17984; i++) {
   p=table[i];
   if (p>a)
      break;
   if (a==(a/p)*p) {
      if (count!=0)
	 return(0.0);
      savep=p;
      count=count+1;
      }
   }
return(log((double)savep));
}
