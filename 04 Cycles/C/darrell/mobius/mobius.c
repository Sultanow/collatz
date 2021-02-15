/******************************************************************************
*									      *
*  COMPUTE MOBIUS FUNCTION						      *
*  04/02/10 (dkc)							      *
*									      *
******************************************************************************/
#include <math.h>
int mobius(unsigned int a, unsigned int *table, unsigned int tsize) {
unsigned int i,count,p;
if (a==1)
   return(1);
count=0;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (p>a)
      break;
   if (a==(a/p)*p) {
      a=a/p;
      if (a==(a/p)*p)
	 return(0);
      count=count+1;
      if (a==1)
	 break;
      }
   }
if ((count&1)==0)
   return(1);
else
   return(-1);
}
