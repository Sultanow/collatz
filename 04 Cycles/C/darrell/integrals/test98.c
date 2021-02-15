//
// COMPUTE INTEGRALS OF FUNCTIONS ON [0,1]
// 01/07/21 (dkc)
//
// The trigonometric functions require a fixed amount to be added to the
// sequence (apparently to change the phase).  The exponential function also
// requires a fixed amount to be added, the same as the cosine.  Apparently,
// this is due to Euler's formula e^(ix)=cos(x)+i*sin(x).  The amount
// required for the tangent function is 1/e.  Denote the other amounts by
// x and y.  These values satisfy x^2+y^2=cos(1), similar to the formula
// sin(x)^2+cos(x)^2=1.  They also satisfy x/y=sqrt(tan(1)), similar to the
// formula sin(x)/cos(x)=tan(x).  The amount for the sine is sin^(-1)(cos(1)).
// The amount for the cosine can be determined by using the formula x^2+y^2=
// cos(1).
//
// The integral of 1/(a^2+x^2) is (1/a)tan^(-1)(x/a).
//
#include <stdio.h> 
#include <math.h> 
#include "bz.h"
unsigned int size=99; // adjust so that slightly larger than larget input value
// 80 for az.h, N-x for L=97 (largest=77, 12 zeros deleted)
// 99 for bz.h, x for L=97 (largest=96)
// 905 for cz.h, N-x for L=997 (largest=902, 12 zeros deleted)
// 999 for dz.h, x for L=997 (largest=996)
// 4853 for ez.h, N-x for L=4999 (largest=4850, 24 zeros deleted)
// 5001 for fz.h, x for L=4999 (largest=4998)
// 6828 for gz.h, N-x for L=6997 (largest=6825, 24 zeros deleted)
// 6998 for hz.h, x for L=6997 (largest=6996(
// 46 for b.h (values of the form pq)
// 2000 for outa1999.h, N-x for L=1999 (largest=1998)
// 1911 for outb1999.h, x for L=1999 (largest=1908, 16 zeros deleted)
// 3990 for outa3989.h, N-x for L=3989 (largest=3988)
// 3739 for outb3989.h, x for L=3989 (largest=3736, 6 zeros deleted)
// 5989 for outa5987.h, N-x for L=5987 (largest=5986)
// 5834 for outb5987.h, x for L=5987 (largest=5832, 8 zeros deleted)
// 7995 for outa7993.h, N-x for L=7993 (largest=7992)
// 7814 for outb7993.h, x for L=7993 (largest=7811, 32 zeros deleted)
// 9974 for outa9973.h, N-x for L=9973 (largest=9972)
// 9663 for outb9973.h, x for L=9973 (largest=9660, 18 zeros deleted)
// 11989 for oua19987.h, N-x for L=11987 (largest=11986)
// 11676 for oub11987.y, x for L=11987 (largest=11674, 8 zeros deleted)
// 98 for outa97.h, N-x for L=97 (largest=96)
// 80 for outb97.h, x for L=97 (largest=77, 12 zeros deleted)
// 999 for outa997.h, N-x for L=997 (largest=996)
// 905 for outb997.h, x for L=997 (largest=902, 12 zeros deleted)
//
unsigned int select=1;  // functions
// 1: seq
// 2: seq*seq
// 3: seq*seq*seq
// 4: seq*seq*seq*seq
// 5: sqrt(seq)
// 6: sqrt(sqrt(seq))
// 7: log(seq)
// 8: exp(seq)
// 9: sin(seq)
//10: cos(seq)
//11: tan(seq)
//12: 1/(2^2+seq*seq)
//13: 1/(3^2+seq*seq)
void main () {
double z[10000];	  // for sequences having fewer than 10000 elements
double seq[10000];
double uni[10000];
double sub,sumsub,delta,sum,temp;
unsigned int i,j,count,newcnt;
FILE *Outfp;
Outfp = fopen("out98.dat","w");
//
// compute subdivision
//
sub=sqrt(2.0);
z[0]=0.0;
i=0;
sumsub=sub;
while (sumsub<(double)size) {
  i=i+1;
  z[i]=sumsub;
  sumsub=sumsub+sub;
  }
count=i;
printf("count=%d i=%d \n",count,i);
//for (j=0; j<count; j++)     // uncomment to check that subdivision is slightly
//   printf(" z=%f \n",z[j]); // larger than largest element in sequence
//
// apply to sequence
//
i=0;
for (j=0; j<count; j++) {
   while ((riem[i]>=z[j])&&(riem[i]<z[j+1])) {
      delta=riem[i]-z[j];
      delta=delta/sub;
      seq[i]=delta;
      i=i+1;
      }
   }
//
// apply to functions
//
newcnt=i;
printf("\n");
printf("newcnt=%d \n",newcnt);
sum=0.0;
for (j=0; j<newcnt; j++) {
   if (select==1)
      sum=sum+seq[j];
   if (select==2)
      sum=sum+seq[j]*seq[j];
   if (select==3)
      sum=sum+seq[j]*seq[j]*seq[j];
   if (select==4)
      sum=sum+seq[j]*seq[j]*seq[j]*seq[j];
   if (select==5)
      sum=sum+sqrt(seq[j]);
   if (select==6)
      sum=sum+sqrt(sqrt(seq[j]));
   if (select==7)
      sum=sum+log(seq[j]);
   if (select==8)
      sum=sum+exp(sqrt(cos(1.0)-asin(cos(1))*asin(cos(1.0)))+seq[j]);
   if (select==9)
      sum=sum+(sin(asin(cos(1.0))+seq[j]));
   if (select==10)
      sum=sum+cos(sqrt(cos(1.0)-asin(cos(1.0))*asin(cos(1.0)))+seq[j]);
   if (select==11)
      sum=sum+tan(1.0/exp(1.0)+seq[j]);  
   if (select==12)
      sum=sum+(1.0/(2.0*2.0+seq[j]*seq[j]));
   if (select==13)
      sum=sum+(1.0/(3.0*3.0+seq[j]*seq[j]));
   temp=sum/(double)(j+1);
   uni[j]=temp;
//  fprintf(Outfp," %f \n",seq[j]); // uncomment to output sequence
   printf(" %f \n",temp);
   fprintf(Outfp," %f \n",temp);
   }
fclose(Outfp);
return;
} 
 
