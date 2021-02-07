/*CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC 
C
C COMPUTE MERTENS FUNCTION (sum of sign(M([x/i])sigma(i))
C 01/25/21 (DKC)
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC*/ 
#include <math.h> 
#include <stdio.h> 
double numdiv(unsigned int a, unsigned int flag);
unsigned int max=20; // maximum x value 
unsigned int out=2;   //  0 for M(x), 1 for sign sum, 2 for sum
		      //  1 for sum of absolute values of sign(M(x))
		      //  2 for sum of sign(M([x/i])i
		      //  3 for [x/n](x/n+1]/2
unsigned int j=1;     //  integers from 1 to max
unsigned int noclip=1;
void main() {
short m[500000];
int sum,t;
unsigned int i,k,r,d,index,stop,msum,temp;
double temp1;
FILE *Outfp;
Outfp = fopen("out7b.dat","w");
if (max>500000) {
   printf("x value too large \n"); 
   goto zskip;
   }
//
// compute Mertens function 
//
m[0]=1;
if (out==0)
   fprintf(Outfp," %d \n",1); 
for (index=2; index<=max; index++) {
   sum=0;
   for (i=2; i<=(index/3); i++)
      sum=sum+m[index/i-1]; 
   sum=sum+(index+1)/2; 
   t=1-sum;
   if ((t>32767)||(t<-32768)) { 
      printf("overflow \n"); 
      goto zskip;
      }
   m[index-1]=(short)t; 
   if (out==0) {
      fprintf(Outfp," %d \n",1-sum);
      }
   }
if (out==0)
   return;
if ((out==1)||(out==3)) {
   msum=0;
   for (i=0; i<j; i++) {
      if (m[i]<0)
	 msum=msum+1;
      if (m[i]>0)
	 msum=msum+1;
      if (out==1)
	 fprintf(Outfp," %d\n",msum);
      }
   if (out==1)
      return;
   }
//
// compute sum
//
if (out==2) {
   for (index=2; index<=max; index++) {
      sum=0;
      if (noclip!=0)
	 stop=index;
      else
	 stop=index/(j+1);
      for (i=1; i<=stop; i++) {
	 if (m[index/i-1]<0) {
	    temp1=numdiv(i,4);
	    sum=sum-(unsigned int)temp1;
	    }
	 if (m[index/i-1]>0) {
	    temp1=numdiv(i,4);
	    sum=sum+(unsigned int)temp1;
	    }
	 }
      fprintf(Outfp," %d \n",sum);
      }
   return;
   }
//
// compute upper bound
//
k=msum;
r=(k-1)-((k-1)/4)*4;
if (r==0)
   d=2;
if (r==1)
   d=3;
if (r==2)
   d=3;
if (r==3)
   d=4;
r=3*(k-1)/4+d;
for (index=2; index<=max; index++) {
   temp=index/r;
   temp=temp*(temp+1);
   temp=temp/2+1;
   fprintf(Outfp," %d \n",temp);
   }
zskip:
return;
} 
