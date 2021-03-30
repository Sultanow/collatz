/*CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                             C
C  COMPUTE DIFFERENCES IN NUMBER OF FRACTIONS (0 to 1/I, 1/I to 2/I)	      C
C  05/13/15 (DKC)							                                  C
C  03/29/21 Added analogue of sum(M(x/(i*n))=1, n=1,2,3,...,x, for out=13     C
C                                                                             C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "table2.h"
int liouvill(unsigned int a, unsigned int *t);
double ramanuj(unsigned int n, unsigned int k, unsigned int *t);
double numdiv(unsigned int a, unsigned int flag);
double mangoldt(unsigned int a, unsigned int *t);
mertenaw(unsigned int, unsigned int *count, unsigned int I);
//
unsigned int limit=0;	// partial sum flag, normally set to 0
unsigned int norm=0;	// normalize, normally set to 0
unsigned int hflag=0;   // histogram flag
unsigned int MAXN=1000; // 2000000000 maximum
unsigned int BEGINN=2;
unsigned int delta=1;	// add 1/4, etc.
unsigned int I=4;     // I value (4,5,6,...)
unsigned int n=1;     // multiple of I (normally set to 1)
unsigned int k=35;	// for out=38
unsigned int power=0;	//  decimate by J (normally set to 0)
unsigned int scale=1;	// for out=37
unsigned int mflag=4;	// 0, 1, 2, 3, or 4 for out=37
unsigned int again=0;	// for out=37
unsigned int out=13;
		     // 10, sgn(n(x)-m(x))+1.781072418*sum(sgn(n(x/i)-m(x/i))*i*log(log(i)))
		     // 11, sum of m(x/i)
		     // 12, sum of n(x/i)
		     // 13, sum of n(x/i)-m(x/i)
		     // 14, sum of (n(x/i)-m(x/i))*i
		     // 15, |n(x)-m(x)|+1.781072418*sum(|n(x/i)-m(x/i)|*i*log(log(i)))
		     // 16, sum of (n(x/i)-m(x/i))*(h+exp(h)*log(h))
		     // 17, differences in number of fractions before, after 1/I
		     // 18, number of fractions before 1/I
		     // 19, number of fractions after 1/I
		     // 20, n(x)-m(x)+1.781072418*sum((n(x/i)-m(x/i))*i*log(log(i)))
		     // 21, sum of (n(x/i)-m(x/i))*2*d(i) (see #24)
		     // 22, sum of (n(x/i)-m(x/i))*sigma(i)
		     // 23, sum of (n(x/i)-m(x/i)^2*L(i)
		     // 24, sum of (n(x/i)-m(x/i))*d(i) (see #21)
		     // 25, sum of n(x/i)-m(x/i) when i is a perfect square
		     // 26, sum of (n(x/i)-m(x/i))^2
		     // 27, sum of (n(x/i)-m(x/i))*log(i)
		     // 28, sum of sgn(n(x/i)-m(x/i))
		     // 29, sum of (n(x/i)-m(x/i)*log(i)*d(i)
		     // 30, sum of (n(x/i)-m(x/i)^2*L(i)^2
		     // 31, sum of sgn(n(x/i)-m(x/i))*i
		     // 32, sum of |sgn(n(x/i)-m(x/i))|
		     // 33, sum of (n(x/i)-m(x/i))*L(i)
		     // 34, sum of (n(x/i)-m(x/i))*M(i)
		     // 35, --
		     // 36, sum of (n(x/i)-m(x/i))*sigma2(i)
		     // 37, sum of (n(x/i)-m(x/i))*mangoldt(i)
		     // 38, sum of (n(x/i)-m(x/i))*ramanuj(i)
		     // 39, sum of M(x/i)*ramanuj(i)
//
void main() {
int t,tsum,sum;
unsigned int N,i,temp,count[2],iters,J,JP;
double sum6,sum7,sum8,sum9,sum10,sum11,sum12,sum13,sum14,sum15,sum16,sum17,sum18;
double sum1,sum2,sum3,sum4,sum5,h,temp1;
unsigned int histo[301];
double mean,std,msum,msum2,dsum,d2sum,*output,*outman,*manout;
short M[60001];
FILE *Outfp;
Outfp = fopen("out1bz.dat","w");
if (out==37) {
   output=(double*) malloc(1000001);
   if (output==NULL)
      return;
   outman=(double*) malloc(1000001);
   if (outman==NULL)
      return;
   manout=(double*) malloc(1000001);
   if (outman==NULL)
      return;
   }
J=(I+1)/2;
JP=1;
if (power!=0)
   JP=J;
//
// compute Mertens function
//
if ((out==34)||(out==39)) {
   if (MAXN>60000) {
      printf("not enough memory allocated");
      goto zskip;
      }
   M[0]=1;
   for (N=2; N<=(MAXN+1); N++) {
      sum=0;
      for (i=2; i<=(N/3); i++)
	 sum=sum+M[N/i-1];
      sum=sum+(N+1)/2;
      t=1-sum;
      M[N-1]=(short)t;
//	  printf(" %d \n",M[N-1]);
      }
   }
for (i=0; i<301; i++)
   histo[i]=0;
iters=0;
dsum=0.0;
d2sum=0.0;
for (N=BEGINN; N<=MAXN; N++) {
   if (N!=(N/JP)*JP)
      continue;
   printf(" %d \n",N);
   if ((out==17)||(out==18)||(out==19))
      mertenaw(N, count, I);
   if (out==17) {
      t=(int)count[0]-(int)count[1];
      if (hflag==2) {
	 dsum=dsum+(double)t;
	 d2sum=d2sum+(double)t*(double)t;
	 iters=iters+1;
	 std=d2sum-dsum*dsum/(double)iters;
	 if (iters!=1)
	    std=sqrt(std/(double)(iters-1));
	 else
	    std=0.0;
	 fprintf(Outfp," %e \n",std);
	 }
      if (hflag==0)
	 fprintf(Outfp," %d\n",t);
      if (hflag==1) {
	 if ((t<-150)||(t>150)) {
	    printf("difference out of range \n");
	    goto zskip;
	    }
	 histo[t+150]=histo[t+150]+1;
	 }
      continue;
      }
   if (out==18) {
      fprintf(Outfp," %d\n",count[0]);
      continue;
      }
   if (out==19) {
      fprintf(Outfp," %d\n",count[1]);
      continue;
      }
   if ((out==11)||(out==12))  {
      sum6=0.0;
      sum7=0.0;
      for (i=1; i<=N; i++) {
	 mertenaw(N/i, count, I);
	 sum6=sum6+(double)count[0];
	 sum7=sum7+(double)count[1];
	 }
      if (out==11)
	 fprintf(Outfp," %e\n",sum6);
      if (out==12)
	 fprintf(Outfp," %e\n",sum7);
      continue;
      }
   if ((out==13)||(out==14)) {
      sum8=0.0;
      sum9=0.0;
      for (i=1; i<=N; i++) {
	 if (N<(i*n))
	    break;
	 mertenaw(N/(i*n), count, I);
	 if (delta==0)
	    sum8=sum8+(double)count[1]-(double)count[0];	      // h(x)
	 else
	    sum8=sum8+(double)count[0]-(double)count[1]+(double)(J-1)/(double)I; // h(x)
	 sum9=sum9+(double)i*((double)count[1]-(double)count[0]);
	 }
      if (out==13) {
	 if (norm==0)
	    fprintf(Outfp," %e\n",sum8);
	 else
	    fprintf(Outfp," %e\n",sum8/(double)N);
	 }
      if (out==14)
	 fprintf(Outfp," %e\n",sum9);
      continue;
      }
   if (out==10) {
      sum2=0.0;
      for (i=2; i<=N; i++) {
	 mertenaw(N/i, count, I);
	 temp1=(double)count[1]-(double)count[0];
	 if (temp1<0.0)
	    sum2=sum2-(double)i*log(log((double)i));
	 if (temp1>0.0)
	    sum2=sum2+(double)i*log(log((double)i));
	 }
      sum2=1.781072418*sum2;
      mertenaw(N, count, I);
      temp1=(double)count[1]-(double)count[0];
      if (temp1<0.0)
	 sum2=sum2-1;
      if (temp1>0.0)
	 sum2=sum2+1;
      fprintf(Outfp," %e\n",sum2);
      continue;
      }
   if (out==15) {
      sum2=0.0;
      for (i=2; i<=N; i++) {
	 mertenaw(N/i, count, I);
	 temp1=(double)count[1]-(double)count[0];
	 if (temp1<0.0)
	    temp1=-temp1;
//	 printf(" %e %e \n",temp1,(double)i*log(log((double)i)));
	 sum2=sum2+temp1*(double)i*log(log((double)i));
	 }
      mertenaw(N, count, I);
      temp1=(double)count[1]-(double)count[0];
      if (temp1<0.0)
	 temp1=-temp1;
      sum2=1.781072418*sum2+temp1;
      fprintf(Outfp," %e\n",sum2);
      continue;
      }
   if (out==16) {
      sum1=0.0;
      h=0.0;
      for (i=1; i<=N; i++) {
	 mertenaw(N/i, count, I);
	 h=h+1.0/(double)i;
	 sum1=sum1+((double)count[1]-(double)count[0])*(h+exp(h)*log(h));
	 }
      fprintf(Outfp," %e\n",sum1);
      continue;
      }
   if (out==20) {
      sum2=0.0;
      for (i=2; i<=N; i++) {
	 mertenaw(N/i, count, I);
//	 printf(" %e %e \n",(double)count[1]-(double)count[0],(double)i*log(log((double)i)));
	 sum2=sum2+((double)count[1]-(double)count[0])*(double)i*log(log((double)i));
	 }
      mertenaw(N, count, I);
      sum2=1.781072418*sum2+(double)count[1]-(double)count[0];
      fprintf(Outfp," %e\n",sum2);
      continue;
      }
   if (out==21) {
      sum3=0.0;
      for (i=1; i<=N; i++) {
	 mertenaw(N/i, count, I);
	 if (delta==0)
	    sum3=sum3+((double)count[1]-(double)count[0])*2.0*numdiv(i,1);
	 else
	    sum3=sum3+((double)count[0]-(double)count[1]+(double)(J-1)/(double)I)*2.0*numdiv(i,1);
	 }
      fprintf(Outfp," %e\n",sum3);
      continue;
      }
   if (out==22) {
      sum4=0.0;
      for (i=1; i<=N; i++) {
	 mertenaw(N/i, count, I);
	 if (delta==0)
	    sum4=sum4+((double)count[1]-(double)count[0])*numdiv(i,4);
	 else
	    sum4=sum4+((double)count[0]-(double)count[1]+(double)(J-1)/(double)I)*numdiv(i,4);
	 }
      fprintf(Outfp," %e\n",sum4);
      continue;
      }
   if (out==23) {
      sum5=0.0;
      tsum=0;
      for (i=1; i<=N; i++) {
	 mertenaw(N/i, count, I);
	 t=liouvill(i, table);
	 tsum=tsum+t;
	 sum5=sum5+((double)count[1]-(double)count[0])*((double)count[1]-(double)count[0])*(double)tsum;
	 }
      if (norm!=0)
	 sum5=sum5/(double)N;
      fprintf(Outfp," %e\n",sum5);
      continue;
      }
   if (out==24) {
      sum14=0.0;
      for (i=1; i<=N; i++) {
	 mertenaw(N/i, count, I);
	 if (delta==0)
	    sum14=sum14+((double)count[1]-(double)count[0])*numdiv(i,1);
	 else
	    sum14=sum14+((double)count[0]-(double)count[1]+(double)(J-1)/(double)I)*numdiv(i,1);
	 }
      fprintf(Outfp," %e\n",sum14);
      continue;
      }
   if (out==25) {
      sum13=0.0;
      for (i=1; i<=N; i++) {
	 temp=(unsigned int)(sqrt((double)i)+0.001);
	 if ((temp*temp)==i) {
	    mertenaw(N/i, count, I);
	    if (delta==0)
	       sum13=sum13+(double)count[1]-(double)count[0];
	    else
	       sum13=sum13+(double)count[0]-(double)count[1]+(double)(J-1)/(double)I;
	    }
	 }
      fprintf(Outfp," %e\n",sum13);
      continue;
      }
   if (out==26) {
      sum10=0.0;
      for (i=1; i<=N; i++) {
	 if ((N/i)<=limit)
	    break;
	 mertenaw(N/i, count, I);
	 if (delta==0)
	    sum10=sum10+((double)count[1]-(double)count[0])*((double)count[1]-(double)count[0]);
	 else
	    sum10=sum10+((double)count[0]-(double)count[1]+(double)(J-1)/(double)I)*((double)count[0]-(double)count[1]+(double)(J-1)/(double)I);
	 }
      fprintf(Outfp," %e\n",sum10);
      continue;
      }
   if (out==27) {
      sum11=0.0;
      for (i=1; i<=N; i++) {
	 mertenaw(N/i, count, I);
//	 printf(" %d %d %d\n",count[0],count[1],(int)count[0]-(int)count[1]);
	 if (delta==0)
	    sum11=sum11+((double)count[1]-(double)count[0])*log((double)i);
	 else
	    sum11=sum11+((double)count[0]-(double)count[1]+(double)(J-1)/(double)I)*log((double)i);
	 }
      fprintf(Outfp," %e\n",sum11);
      continue;
      }
   if (out==28) {
      sum12=0.0;
      for (i=1; i<=N; i++) {
	 mertenaw(N/i, count, I);
	 if (count[0]>count[1])       // sum of sgn(n(x/i)-m(x/i))
	    sum12=sum12-1.0;
	 if (count[0]<count[1])
	    sum12=sum12+1.0;
	 }
      if (norm==0)
	 fprintf(Outfp," %e\n",sum12);
      else
	 fprintf(Outfp," %e\n",sum12/(double)N);
      continue;
      }
   if (out==29) {
      sum18=0.0;
      for (i=1; i<=N; i++) {
	 mertenaw(N/i, count, I);
	 if (delta==0)
	    sum18=sum18+((double)count[1]-(double)count[0])*log((double)i)*numdiv(i,1);
	 else
	    sum18=sum18+((double)count[0]-(double)count[1]+(double)(J-1)/(double)I)*log((double)i)*numdiv(i,1);
	 }
      fprintf(Outfp," %e\n",sum18);
      continue;
      }
   if (out==30) {
      sum17=0.0;
      tsum=0;
      for (i=1; i<=N; i++) {
	 mertenaw(N/i, count, I);
	 t=liouvill(i, table);
	 tsum=tsum+t;
	 sum17=sum17+((double)count[1]-(double)count[0])*((double)count[1]-(double)count[0])*(double)tsum*(double)tsum;
	 }
      if (norm!=0)
	 sum17=sum17/(double)N;
      fprintf(Outfp," %e\n",sum17);
      continue;
      }
   if (out==31) {
      sum15=0.0;
      for (i=1; i<=N; i++) {
	 mertenaw(N/i, count, I);
	 if (count[0]>count[1])        // sum of sgn(n(x/i)-m(x/i))*i
	    sum15=sum15-(double)i;
	 if (count[0]<count[1])
	    sum15=sum15+(double)i;
	 }
      fprintf(Outfp," %e\n",sum15);
      continue;
      }
   if (out==32) {
      sum16=0.0;
      for (i=1; i<=N; i++) {
	 mertenaw(N/i, count, I);
	 if (count[0]!=count[1])      // sum of |sgn(n(x/i)-m(x/i))|
	    sum16=sum16+1.0;
	 }
      if (norm==0)
	 fprintf(Outfp," %e\n",sum16);
      else
	 fprintf(Outfp," %e\n",sum16/(double)N);
      }
   if (out==33) {
      sum17=0.0;
      tsum=0;
      for (i=1; i<=N; i++) {
	 mertenaw(N/i, count, I);
	 t=liouvill(i, table);
	 tsum=tsum+t;
	 sum17=sum17+((double)count[1]-(double)count[0])*(double)tsum;
	 }
      if (norm!=0)
	 sum17=sum17/(double)N;
      fprintf(Outfp," %e\n",sum17);
      continue;
      }
   if (out==34) {
      sum18=0.0;
      for (i=1; i<=N; i++) {
	 mertenaw(N/i, count, I);
	 if (delta==0)
	    sum18=sum18+((double)count[1]-(double)count[0])*(double)M[i-1];
	 else {
	    if (delta==1)
	       sum18=sum18+((double)count[0]-(double)count[1]+(double)(J-1)/(double)I)*(double)M[i-1];
	    else {
	       if (delta==2)
		  sum18=sum18+(double)count[0]*(double)M[i-1];
	       else {
		  if (delta==3)
		     sum18=sum18+(double)count[1]*(double)M[i-1];
		  else {
		     if (delta==4)
			sum18=sum18+((double)count[1]-(double)count[0])*(double)M[N/i-1];
		     else
			sum18=sum18+((double)count[0]-(double)count[1]+(double)(J-1)/(double)I)*(double)M[N/i-1];
		     }
		  }
	       }
	    }
	 }
      fprintf(Outfp," %e\n",sum18);
      continue;
      }
   if (out==36) {
      sum4=0.0;
      for (i=1; i<=N; i++) {
	 mertenaw(N/i, count, I);
	 if (delta==0)
	    sum4=sum4+((double)count[1]-(double)count[0])*numdiv(i,5);
	 else
	    sum4=sum4+((double)count[0]-(double)count[1]+(double)(J-1)/(double)I)*numdiv(i,5);
	 }
      fprintf(Outfp," %e\n",sum4);
      continue;
      }
   if (out==37) {
      sum4=0.0;
      for (i=1; i<=N; i++) {
	 mertenaw(N/i, count, I);
	 if (delta==0) {
	    if (scale==0)
	       sum4=sum4+((double)count[1]-(double)count[0])*mangoldt(i,table);
	    else
	       sum4=sum4+(((double)count[0]-(double)count[1])/(double)I)*mangoldt(i,table);
	    }
	 else
	    sum4=sum4+((double)count[0]-(double)count[1]+(double)(J-1)/(double)I)*mangoldt(i,table);
//	 printf(" %e %e %e\n",sum4,(double)count[1]-(double)count[0],mangoldt(i, table));
	 }
      if (mflag==0)
	 fprintf(Outfp," %e\n",sum4);
      else {
	 outman[N-1]=sum4;
	 mertenaw(N, count, I);
	 output[N-1]=((double)count[0]-(double)count[1])/(double)I;
	 }
      continue;
      }
   if (out==38) {
      sum4=0.0;
      for (i=1; i<=N; i++) {
	 mertenaw(N/i, count, I);
	 if (delta==0)
	    sum4=sum4+((double)count[1]-(double)count[0])*ramanuj(i,k,table);
	 else
	    sum4=sum4+((double)count[0]-(double)count[1]+(double)(J-1)/(double)I)*ramanuj(i,k,table);
	 }
      fprintf(Outfp," %e\n",sum4);
      continue;
      }
   if (out==39) {
      sum4=0.0;
      for (i=1; i<=N; i++)
	 sum4=sum4+(double)M[N/i-1]*ramanuj(i,k,table);
      fprintf(Outfp," %e\n",sum4);
      continue;
      }
   }
//
// additional processing
//
if ((out==37)&&(mflag!=0)) {
   outman[0]=0.0;
   output[0]=0.0;
   for (N=2; N<=MAXN; N++) {
      sum4=0.0;
      for (i=1; i<=N; i++) {
	 if (delta==0) {
	    if (mflag==1)
	       sum4=sum4+outman[N/i-1]*mangoldt(i,table);
	    if (mflag==2)
	       sum4=sum4+outman[i-1]*mangoldt(N/i,table);
	    if (mflag==3)
	       sum4=sum4+outman[N/i-1]*output[i-1];
	    if (mflag==4)
	       sum4=sum4+output[N/i-1]*outman[i-1];
	    }
	 else {
	    if (mflag==1)
	       sum11=sum11+(outman[N/i-1]+temp)*mangoldt(i,table);
	    else
	       sum11=sum11+(outman[i-1]+temp)*mangoldt(N/i,table);
	    }
//	 printf(" %d %e %e \n",i,outman[i-1],mangoldt(N/i,table));
	 }
      printf(" %d %e \n",N,sum4);
      if (again==0)
	 fprintf(Outfp," %e\n",sum4);
      else
	 manout[N-1]=sum4;
      }
   if (again==0)
      goto zskip;
   manout[0]=0.0;
   for (N=2; N<=MAXN; N++) {
      sum4=0.0;
      for (i=1; i<=N; i++) {
	 if (delta==0) {
	    if (mflag==1)
	       sum4=sum4+manout[N/i-1]*mangoldt(i,table);
	    if (mflag==2)
	       sum4=sum4+manout[i-1]*mangoldt(N/i,table);
	    if (mflag==3)
	       sum4=sum4+manout[N/i-1]*output[i-1];
	    if (mflag==4)
	       sum4=sum4+output[N/i-1]*manout[i-1];
	    }
	 else {
	    if (mflag==1)
	       sum11=sum11+(manout[N/i-1]+temp)*mangoldt(i,table);
	    else
	       sum11=sum11+(manout[i-1]+temp)*mangoldt(N/i,table);
	    }
//	 printf(" %d %e %e \n",i,manout[i-1],mangoldt(N/i,table));
	 }
      printf(" %d %e \n",N,sum4);
      fprintf(Outfp," %e\n",sum4);
      }
   }
if ((out==17)&&(hflag==1)) {
   mean=0.0;
   msum2=0.0;
   for (i=0; i<301; i++) {
      fprintf(Outfp," %d\n",histo[i]);
      mean=mean+((double)i-150.0)*histo[i];
//    printf(" %d %d %d %e\n",(int)(i-150),histo[i],(int)(i-150)*histo[i],mean);
      msum2=msum2+((double)i-150.0)*histo[i]*((double)i-150.0);
      }
   msum=mean;
   mean=mean/(double)(MAXN-1);
   std=(double)(MAXN-1)*msum2-msum*msum;
   std=sqrt(std/((double)(MAXN-1)*(double)(MAXN-2)));
   printf("mean=%e, std=%e \n",mean,std);
   }
zskip:
if (out==37) {
   free(output);
   free(outman);
   free(manout);
   }
fclose(Outfp);
return;
}
