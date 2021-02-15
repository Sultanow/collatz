/******************************************************************************
*									      *
*  COMPUTE CONVOLUTION WITH MOBIUS FUNCTION				      *
*  07/14/18 (dkc)							      *
*									      *
*  Normally distributed (for differences between zeros).		      *
*									      *
******************************************************************************/
#include <math.h>
#include <stdio.h>
//#include "out14wa2.h" // 1999
//#include "out14wa.h"  // 998
//#include "out14a.h" // 1228
//#include "out20b.h" // 1786
#include "out19a.h" // 296
#include "table2.h"
extern char *malloc();
int mobius(unsigned int a, unsigned int *table, unsigned int tsize);
unsigned int nucheck(unsigned int N, unsigned int p, unsigned int pprod,
  unsigned int subcur, unsigned int size, unsigned int *table,
  unsigned int *skip, unsigned int nope);
//
unsigned int msn=0;  // if set output mean
double diag=0.0;
//
unsigned int MAXN=296; // maximum N value (or upper bound if "check!=0")
unsigned int newcnt=296;  // for curves and sub-curves ("check" not equal to   0)
unsigned int wrap=0;
unsigned int scale=0; // if set to 1, divide zeros by their logarithms
                      // if set to 2, multiply zeros by their logarithms
		      // if set to 3, multiply difference by log(y)/(2pi)^2
unsigned int diff=0;  // if set, take differences
unsigned int inc=0;   // offset for differences (0 for next)
//
unsigned int check=1; // check if N is prime, etc.
unsigned int p=3;   // power of prime (up to 21 complete)
unsigned int pprod=1; // if set when p>1 and odd, do primes and combinations
unsigned int subcur=2;	// sub-curves, set to 0 to do all
unsigned int nope=0;  // if set when p=3, omit prime x even if skip[3]=0
		      // if set when check=0, omit prime x values
			// 0, 1, 2, 3, 4 for p=3 (includes p^2 and p)
			// 0, 1, 2, 3 for p=5 (includes p^4)
			// 0, 1, 2, 3, 4 for p=7 (includes p^6)
			// 0, 1, 2, 3, 4 for p=9 (includes p^8 [use])
			// 0, 1, 2, 3, 4, 5 for p=11 (includes p^10)
			// 0, 1, 2, 3, 4, 5, 6, 7, 8 for p=13 (includes p^12)
			// 0, 1, 2, 3, 4, 5, 6, 7, 8 for p=15 (includes p^14)
			// 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 for p=17 (includes p^16)
            // 0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18 for p=19 (also p^18)
    // 0,1,2,3,4,5,6,7,8,9,10,11,12 for p=21 (includes p^20)
    // 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22 for p=23
    // 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27 for p=25
//
// check skip[18] and skip[19], if set to zero, flows through curves 
//    1 through 11 (skip[19]) and 13 through 21 (skip[18])
//
//unsigned int skip[20]={1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};  // p=8
//unsigned int skip[20]={0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};  // p=9
unsigned int skip[20]={0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};  // p=11
//unsigned int skip[20]={1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1};  // p=13
//unsigned int skip[20]={0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};  // p=13
//unsigned int skip[20]={1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};  // p=15
//unsigned int skip[20]={1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1};  // p=15
//unsigned int skip[20]={1,1,0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1};  // p=17
//unsigned int skip[20]={1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1};  // p=17
//unsigned int skip[27]={1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};  // p=19
//unsigned int skip[20]={1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1};  // p=19
//unsigned int skip[20]={1,1,1,1,1,1,0,1,1,1,1,1,0,0,1,0,1,1,1,1};  // p=19
//unsigned int skip[20]={0,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1};  // p=21
//unsigned int skip[20]={1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1};  // p=21
//unsigned int skip[27]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0};
//unsigned int skip[27]={0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
unsigned int pass=0;
unsigned int factor=2;
void main () {
unsigned int i,N,count,wcount,mobsum;
double sum,oldmean,mean,m2,total;
double *save;
double max,min,pi2;
double pi=3.14159265358979323846;
FILE *Outfp;
Outfp = fopen("out3.dat","w");
if ((msn!=0)&&(wrap==1))
   fprintf(Outfp," %llf \n",1.0);
pi2=pi*2.0;
if (Outfp==NULL)
   return;
save=(double*) malloc(800008);
if (save==NULL) {
   printf("not enough memory \n");
   return;
   }
//
mobsum=1;
count=0;
wcount=1;
max=0.0;
min=1000000;
for (N=1; N<=MAXN; N++) {
   if (check!=0) {
      if (nucheck(N,p,pprod,subcur,tsize,table,skip,nope)==1) {
	 count=count+1;
	 if (count>newcnt) {
	    count=count-1;
	    goto zskip;
	    }
	 }
      else
	  continue;
      }
   else {
      if (nope!=0) {
	 if (nucheck(N,1,0,subcur,tsize,table,skip,nope)==0) {
	    count=count+1;
	    if (count>newcnt) {
	       count=count-1;
	       goto zskip;
	       }
	    }
	 else
	    continue;
	 }
      else
	 count=count+1;
      }
   sum=0.0;
   for (i=1; i<=N; i++) {
      if (N==(N/i)*i) {
	 if (scale==0) {
	    if (diff==0)
	       sum=sum+riem[i-1]*mobius(i,table,tsize);
	     else
	       sum=sum+(riem[i+inc]-riem[i-1])*mobius(i,table,tsize);
	    }
	 else {
	    if (diff==0) {
	       if (scale==1)
		  sum=sum+riem[i-1]/log(riem[i-1])*mobius(i,table,tsize);
	       else
		  sum=sum+riem[i-1]*log(riem[i-1])*mobius(i,table,tsize);
	       }
	    else {
	       if (scale==1)
		  sum=sum+(riem[i+inc]/log(riem[i+inc])-riem[i-1]/log(riem[i-1]))*mobius(i,table,tsize);
	       else {
		  if (scale==2)
		     sum=sum+(riem[i+inc]*log(riem[i+inc])-riem[i-1]*log(riem[i-1]))*mobius(i,table,tsize);
		  else
		     sum=sum+(riem[i+inc]-riem[i-1])*log(riem[i-1]/pi2)/pi2*mobius(i,table,tsize);
		  }
	       }
	    }
	 }
      }
   if ((diff==0)&&(diag!=0.0)) {
      if (sqrt((double)N*(double)N+sum*sum)>diag)
	 continue;
      }
   mobsum=mobsum+mobius(N,table,tsize);
   if (sum>max)
      max=sum;
   if (sum<min)
      min=sum;
   if (wrap==0) {
      if (count==(count/100)*100)
	 printf("sum=%llf, N=%d \n",sum,N);
      }
   if (scale!=4) {
      if ((msn==0)&&(pass==0))
	 fprintf(Outfp," %llf, \n",sum);
//      else
//	 fprintf(Outfp," %d, \n",mobsum);
	   if ((pass!=0)&&(N==(N/factor)*factor))
	   fprintf(Outfp," %llf, \n",sum);
//   if (pass==0)
//      fprintf(Outfp," %llf, \n",sum);
      }
   else
      save[count-1]=sum;
   if (count==1) {
      total=sum;
      mean=sum;
      oldmean=sum;
      m2=sum;
      }
   else {
      total=total+sum;
      mean=total/(double)count;
      m2=m2+(sum-oldmean)*(sum-mean);
      oldmean=mean;
      if (wcount==wrap) {
	 printf("N=%d mean=%llf standard deviation=%llf \n",N,mean,sqrt(m2/(double)(count-1)));
	 if (msn==1)
	    fprintf(Outfp," %llf, \n",mean);
	 wcount=1;
	 }
      else
	 wcount=wcount+1;
      }
   }
zskip:
if ((scale==4)&&(msn==0)) {
   for (i=0; i<(count-1); i++)
      fprintf(Outfp," %llf, \n",save[i]+save[i+1]);
   }
printf("max=%llf min=%llf count=%d \n",max,min,count);
printf("mean=%llf standard deviation=%llf \n",mean,sqrt(m2/(double)(count-1)));
fclose(Outfp);
return;
}
