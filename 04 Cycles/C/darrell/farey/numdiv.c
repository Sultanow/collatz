/******************************************************************************
*									      *
*  COUNT NUMBER OF DIVISORS AND SUM OF LOGARITHMS			      *
*  04/02/10 (dkc)							      *
*									      *
******************************************************************************/
#include <math.h>
double numdiv(unsigned int a, unsigned int flag) {
unsigned int i,count;
double temp,sum,sum2;
count=0;
sum=0.0;
sum2=0.0;
for (i=1; i<=a; i++) {
   if (a==(a/i)*i) {
      count=count+1;
      sum=sum+(double)i;
      sum2=sum2+(double)i*(double)i;
      }
   }
temp=(double)count/2.0;
if (flag==1)
   return temp;
if (flag==2)
   return(temp*temp);
if (flag==3) {
   temp=(log((double)a))/temp;
   return temp;
   }
if (flag==5)
   return(sum2);
return(sum);		   // flag=4
}
