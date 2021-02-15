/*CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                             C
C  COMPUTE MERTENS FUNCTION (using Deleglise and Rivat's algorithm)           C
C  08/31/15 (DKC)							      C
C                                                                             C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC*/
#include <stdio.h>
#include <math.h>
#include "table2.h"
extern char *malloc();
int newmob(unsigned int a, unsigned int b, int *out, unsigned int *table,
		   unsigned int tsize);
double sumlo(unsigned int x, unsigned int u, int *mob, int *M);
double sumhi(unsigned int x, unsigned int u, int *mob, int *M);
unsigned int x=32000;
unsigned int tsize=17984;
void main() {
unsigned int i;
int t;
double f1,f2,f3;
int *mobb;
int *M;
unsigned int u;
FILE *Outfp;
Outfp = fopen("out19x.dat","w");
mobb=(int*) malloc(40000004);
if (mobb==NULL)
   return;
M=(int*) malloc(40000004);
if (M==NULL)
   return;
f1=log(log((double)x));
f1=f1*f1;
f1=exp(1.0/3.0*log(f1));
f2=exp(1.0/3.0*log((double)x));
u=(unsigned int)(f1*f2);
if ((x/u)>10000000) {
   printf("not enough memory \n");
   goto zskip;
   }
printf("x/u=%d \n",x/u);
t=newmob(1,x/u,mobb,table,tsize);
if (t!=1) {
   printf("too big \n");
   goto zskip;
   }
M[0]=1;
for (i=1; i<=(x/u); i++)
   M[i]=M[i-1]+mobb[i];
f1=sumlo(x,u,mobb,M);
f2=sumhi(x,u,mobb,M);
f3=(double)M[u-1]-f1-f2;
printf(" %d %d \n",x,(int)f3);
zskip:
fclose(Outfp);
return;
}
/******************************************************************************
*									      *
*  COMPUTE MERTENS FUNCTION						      *
*  04/02/10 (dkc)							      *
*									      *
******************************************************************************/
#include <math.h>
double sumhi(unsigned int x, unsigned int u, int *mob, int *M) {
unsigned int i,j,k,m;
double temp,sumb;
sumb=0.0;
for (m=1; m<=u; m++) {
   temp=(double)mob[m-1];
   j=(unsigned int)sqrt((double)(x/m));
   j=j+1;
   k=x/m;
   for (i=j; i<=k; i++)
      sumb=sumb+temp*(double)M[x/(m*i)-1];
   }
return(sumb);
}
/******************************************************************************
*									      *
*  COMPUTE MERTENS FUNCTION						      *
*  04/02/10 (dkc)							      *
*									      *
******************************************************************************/
#include <math.h>
double sumlo(unsigned int x,unsigned int u, int *mob, int *M) {
unsigned int i,j,k,m;
double temp,sumb;
sumb=0.0;
for (m=1; m<=u; m++) {
   temp=(double)mob[m-1];
   j=u/m+1;
   k=(unsigned int)sqrt((double)(x/m));
   for (i=j; i<=k; i++)
      sumb=sumb+temp*(double)M[x/(m*i)-1];
   }
return(sumb);
}
/******************************************************************************
*									      *
*  COMPUTE MOBIUS FUNCTION (Deleglise & Rivat's algorithm)                    *
*  08/31/15 (dkc)							      *
*									      *
******************************************************************************/
#include <math.h>
int newmob(unsigned int a, unsigned int b, int *out, unsigned int *table,
	   unsigned int tsize) {
unsigned int i,count,p,ps,beg,rem;
int t;
count=b-a;
for (i=0; i<count; i++)
   out[i]=1;
for (i=0; i<tsize; i++) {
   p=table[i];
   ps=p*p;
   if (ps>b)
      goto askip;
   rem=a-(a/ps)*ps;
   if (rem!=0)
      beg=ps-rem;
   else
      beg=0;
   while (beg<count) {
      out[beg]=0;
      beg=beg+ps;
      }
   rem=a-(a/p)*p;
   if (rem!=0)
      beg=p-rem;
   else
      beg=0;
   while (beg<count) {
      out[beg]=-out[beg]*p;
      beg=beg+p;
      }
   }
return(-1);
askip:
for (i=0; i<count; i++) {
   if (out[i]==0)
      continue;
   t=out[i];
   if (t<0)
      t=-t;
   if ((unsigned int)t<(i+a))
      out[i]=-out[i];
   }
for (i=0; i<count; i++) {
   if (out[i]==0)
      continue;
   if (out[i]>0)
      out[i]=1;
   else
      out[i]=-1;
   }
return(1);
}
