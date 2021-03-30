/******************************************************************************
*									      *
*  COMPUTE RAMANUJAN FUNCTION						      *
*  04/02/10 (dkc)							      *
*									      *
******************************************************************************/
#include <math.h>
unsigned int euclid(unsigned int a, unsigned int b);
double mobius(unsigned int a, unsigned int *t);
double ramanuj(unsigned int n, unsigned int k, unsigned int *table) {
unsigned int i,g;
double sum;
g=euclid(n,k);
sum=0.0;
for (i=1; i<=g; i++) {
   if (g!=(g/i)*i)
      continue;
   sum=sum+(double)i*mobius(k/i, table);
   }
return(sum);
}
