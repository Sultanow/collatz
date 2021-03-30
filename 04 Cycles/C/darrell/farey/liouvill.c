/******************************************************************************
*									      *
*  COMPUTE LIOUVILLE FUNCTION						      *
*  04/02/10 (dkc)							      *
*									      *
******************************************************************************/
#include <math.h>
int liouvill(unsigned int a, unsigned int *table) {
unsigned int i,b,count,p;
if (a==1)
   return(1);
count=0;
for (i=0; i<17984; i++) {
   p=table[i];
   if (p>a)
      break;
   if (a==(a/p)*p) {
      b=a;
      while (b==(b/p)*p) {
	 count=count+1;
	 b=b/p;
	 }
      }
   }
if ((count&1)==0)
   return 1;
else
   return -1;
}
