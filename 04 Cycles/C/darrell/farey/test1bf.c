/*CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                             C
C  COMPUTE DIFFERENCES IN NUMBER OF FRACTIONS				      C
C  01/02/15 (DKC)							      C
C                                                                             C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
int liouvile(unsigned int a);
double numdiv(unsigned int a, unsigned int flag);
mertens9(unsigned int, unsigned int *count, unsigned int *S, unsigned int flag);
//
unsigned int limit=0;	// partial sum flag, normally set to 0
unsigned int norm=0;	// normalize, normally set to 0
unsigned int hflag=0;	// histogram flag
unsigned int delta=1;	// increment by 1/6 if flag=0 or 1/4 if flag=1
unsigned int flag=0;   // x=3 flag
unsigned int MAXN=100; // up to 60000
unsigned int BEGINN=2;
unsigned int out=18;
		     // 10, sgn(n(x)-m(x))+1.781072418*sum(sgn(n(x/i)-m(x/i))*i*log(log(i)))
		     // 11, sum of m(x/i)
		     // 12, sum of n(x/i)
		     // 13, sum of n(x/i)-m(x/i)
		     // 14, sum of (n(x/i)-m(x/i))*i
		     // 15, |n(x)-m(x)|+1.781072418*sum(|n(x/i)-m(x/i)|*i*log(log(i)))
		     // 16, sum of (n(x/i)-m(x/i))*(h+exp(h)*log(h))
		     // 17, differences in number of fractions before, after 1/4
		     // 18, number of fractions before 1/4
		     // 19, number of fractions after 1/4
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
		     // 34, sum of (n(x/i)-m(x/i)*M(i) (or n(x/i)M(i), m(x/i)M(i))
		     // 35, sum of (n(x/i)-m(x/i))*L(i)*lambda(i)
		     // 36, sum of (n(x/i)-m(x/i))*sigma2(i)
//
void main() {
int t,tsum,sum;
unsigned int N,i,temp,count[2],iters;
double sum6,sum7,sum8,sum9,sum10,sum11,sum12,sum13,sum14,sum15,sum16,sum17,sum18;
double sum1,sum2,sum3,sum4,sum5,h,temp1;
unsigned int *S;
short M[60001];
unsigned int histo[201];
double mean,std,msum,msum2,dsum,d2sum;
FILE *Outfp;
Outfp = fopen("out1bf.dat","w");
S=(unsigned int*) malloc(40000000);
if (S==NULL)
   return;
if ((out==17)&&(hflag!=0)&&(BEGINN!=2)) {
   printf("parameter error \n");
   goto zskip;
   }
//
// compute Mertens function
//
if (out==34) {
   M[0]=1;
   for (N=2; N<=(MAXN+1); N++) {
      sum=0;
      for (i=2; i<=(N/3); i++)
	 sum=sum+M[N/i-1];
      sum=sum+(N+1)/2;
      t=1-sum;
      M[N-1]=(short)t;
      }
   }
for (i=0; i<201; i++)
   histo[i]=0;
iters=0;
dsum=0.0;
d2sum=0.0;
for (N=BEGINN; N<=MAXN; N++) {
   printf(" %d \n",N);
   if ((out==17)||(out==18)||(out==19))
      mertens9(N, count, S, flag);
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
	 if ((t<-100)||(t>100)) {
	    printf("difference out of range \n");
	    goto zskip;
	    }
	 histo[t+100]=histo[t+100]+1;
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
	 mertens9(N/i, count, S, flag);
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
	 mertens9(N/i, count, S, flag);
//	 printf(" %d %d \n",count[0],count[1]);
	 if (delta==0)
	    sum8=sum8+(double)count[1]-(double)count[0];	   // h(x)
	 else {
	    if (flag==0)
	       sum8=sum8+(double)count[0]-(double)count[1]+1.0/6.0;   // h(x)
	    else
	       sum8=sum8+(double)count[0]-(double)count[1]+1.0/4.0;   // h(x)
	    }
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
	 mertens9(N/i, count, S, flag);
	 if (delta==0)
	    temp1=(double)count[1]-(double)count[0];
	 else
	    temp1=(double)count[0]-(double)count[1]+1.0/6.0;
	 if (temp1<0.0)
	    sum2=sum2-(double)i*log(log((double)i));
	 if (temp1>0.0)
	    sum2=sum2+(double)i*log(log((double)i));
	 }
      sum2=1.781072418*sum2;
      mertens9(N, count, S, flag);
      if (delta==0)
	 temp1=(double)count[1]-(double)count[0];
      else
	 temp1=(double)count[0]-(double)count[1]+1.0/6.0;
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
	 mertens9(N/i, count, S, flag);
	 if (delta==0)
	    temp1=(double)count[1]-(double)count[0];
	 else
	    temp1=(double)count[0]-(double)count[1]+1.0/6.0;
	 if (temp1<0.0)
	    temp1=-temp1;
//	 printf(" %e %e \n",temp1,(double)i*log(log((double)i)));
	 sum2=sum2+temp1*(double)i*log(log((double)i));
	 }
      mertens9(N, count, S, flag);
      if (delta==0)
	 temp1=(double)count[1]-(double)count[0];
      else
	 temp1=(double)count[0]-(double)count[1]+1.0/6.0;
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
	 mertens9(N/i, count, S, flag);
	 h=h+1.0/(double)i;
	 if (delta==0)
	    sum1=sum1+((double)count[1]-(double)count[0])*(h+exp(h)*log(h));
	 else {
	    if (flag==0)
	       sum1=sum1+((double)count[0]-(double)count[1]+1.0/6.0)*(h+exp(h)*log(h));
	    else
	       sum1=sum1+((double)count[0]-(double)count[1]+1.0/4.0)*(h+exp(h)*log(h));
	    }
	 }
      fprintf(Outfp," %e\n",sum1);
      continue;
      }
   if (out==20) {
      sum2=0.0;
      for (i=2; i<=N; i++) {
	 mertens9(N/i, count, S, flag);
//	 printf(" %e %e \n",(double)count[1]-(double)count[0],(double)i*log(log((double)i)));
	 if (delta==0)
	    sum2=sum2+((double)count[1]-(double)count[0])*(double)i*log(log((double)i));
	 else
	    sum2=sum2+((double)count[0]-(double)count[1]+1.0/6.0)*(double)i*log(log((double)i));
	 }
      mertens9(N, count, S, flag);
      if (delta==0)
	 sum2=1.781072418*sum2+(double)count[1]-(double)count[0];
      else
	 sum2=1.781072418*sum2+(double)count[0]-(double)count[1]+1.0/6.0;
      fprintf(Outfp," %e\n",sum2);
      continue;
      }
   if (out==21) {
      sum3=0.0;
      for (i=1; i<=N; i++) {
	 mertens9(N/i, count, S, flag);
	 if (delta==0)
	    sum3=sum3+((double)count[1]-(double)count[0])*2.0*numdiv(i,1);
	 else {
	    if (flag==0)
	       sum3=sum3+((double)count[0]-(double)count[1]+1.0/6.0)*2.0*numdiv(i,1);
	    else
	       sum3=sum3+((double)count[0]-(double)count[1]+1.0/4.0)*2.0*numdiv(i,1);
	    }
	 }
      fprintf(Outfp," %e\n",sum3);
      continue;
      }
   if (out==22) {
      sum4=0.0;
      for (i=1; i<=N; i++) {
	 mertens9(N/i, count, S, flag);
	 if (delta==0)
	    sum4=sum4+((double)count[1]-(double)count[0])*numdiv(i,4);
	 else
	    sum4=sum4+((double)count[0]-(double)count[1]+1.0/6.0)*numdiv(i,4);
	 }
      fprintf(Outfp," %e\n",sum4);
      continue;
      }
   if (out==23) {
      sum5=0.0;
      tsum=0;
      for (i=1; i<=N; i++) {
	 mertens9(N/i, count, S, flag);
	 t=liouvile(i);
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
	 mertens9(N/i, count, S, flag);
	 if (delta==0)
	    sum14=sum14+((double)count[1]-(double)count[0])*numdiv(i,1);
	 else {
	    if (flag==0)
	       sum14=sum14+((double)count[0]-(double)count[1]+1.0/6.0)*numdiv(i,1);
	    else
	       sum14=sum14+((double)count[0]-(double)count[1]+1.0/4.0)*numdiv(i,1);
	    }
	 }
      fprintf(Outfp," %e\n",sum14);
      continue;
      }
   if (out==25) {
      sum13=0.0;
      for (i=1; i<=N; i++) {
	 temp=(unsigned int)(sqrt((double)i)+0.001);
	 if ((temp*temp)==i) {
	    mertens9(N/i, count, S, flag);
	    if (delta==0)
	       sum13=sum13+(double)count[1]-(double)count[0];
	    else {
	       if (flag==0)
		  sum13=sum13+(double)count[0]-(double)count[1]+1.0/6.0;
	       else
		  sum13=sum13+(double)count[0]-(double)count[1]+1.0/4.0;
	       }
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
	 mertens9(N/i, count, S, flag);
	 if (delta==0)
	    sum10=sum10+((double)count[1]-(double)count[0])*((double)count[1]-(double)count[0]);
	 else
	    sum10=sum10+((double)count[0]-(double)count[1]+1.0/6.0)*((double)count[0]-(double)count[1]+1.0/6.0);
	 }
      fprintf(Outfp," %e\n",sum10);
      continue;
      }
   if (out==27) {
      sum11=0.0;
      for (i=1; i<=N; i++) {
	 mertens9(N/i, count, S, flag);
	 if (delta==0)
	    sum11=sum11+((double)count[1]-(double)count[0])*log((double)i);
	 else {
	    if (flag==0)
	       sum11=sum11+((double)count[0]-(double)count[1]+1.0/6.0)*log((double)i);
	    else
	       sum11=sum11+((double)count[0]-(double)count[1]+1.0/4.0)*log((double)i);
	    }
	 }
      fprintf(Outfp," %e\n",sum11);
      continue;
      }
   if (out==28) {
      sum12=0.0;
      for (i=1; i<=N; i++) {
	 mertens9(N/i, count, S, flag);
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
	 mertens9(N/i, count, S, flag);
	 if (delta==0)
	    sum18=sum18+((double)count[1]-(double)count[0])*log((double)i)*numdiv(i,1);
	 else {
	    if (flag==0)
	       sum18=sum18+((double)count[0]-(double)count[1]+1.0/6.0)*log((double)i)*numdiv(i,1);
	    else
	       sum18=sum18+((double)count[0]-(double)count[1]+1.0/4.0)*log((double)i)*numdiv(i,1);
	    }
	 }
      fprintf(Outfp," %e\n",sum18);
      continue;
      }
   if (out==30) {
      sum17=0.0;
      tsum=0;
      for (i=1; i<=N; i++) {
	 mertens9(N/i, count, S, flag);
	 t=liouvile(i);
	 tsum=tsum+t;
	 if (delta==0)
	    sum17=sum17+((double)count[1]-(double)count[0])*((double)count[1]-(double)count[0])*(double)tsum*(double)tsum;
	 else {
	    if (flag==0)
	       sum17=sum17+((double)count[0]-(double)count[1]+1.0/6.0)*((double)count[1]-(double)count[0])*(double)tsum*(double)tsum;
	    else
	       sum17=sum17+((double)count[0]-(double)count[1]+1.0/4.0)*((double)count[1]-(double)count[0])*(double)tsum*(double)tsum;
	    }
	 }
      if (norm!=0)
	 sum17=sum17/(double)N;
      fprintf(Outfp," %e\n",sum17);
      continue;
      }
   if (out==31) {
      sum15=0.0;
      for (i=1; i<=N; i++) {
	 mertens9(N/i, count, S, flag);
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
	 mertens9(N/i, count, S, flag);
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
	 mertens9(N/i, count, S, flag);
	 t=liouvile(i);
	 tsum=tsum+t;
	 if (delta==0)
	    sum17=sum17+((double)count[1]-(double)count[0])*(double)tsum;
	 else {
	    if (flag==0)
	       sum17=sum17+((double)count[0]-(double)count[1]+1.0/6.0)*(double)tsum;
	    else
	       sum17=sum17+((double)count[0]-(double)count[1]+1.0/4.0)*(double)tsum;
	    }
	 }
      if (norm!=0)
	 sum17=sum17/(double)N;
      fprintf(Outfp," %e\n",sum17);
      continue;
      }
   if (out==34) {
      sum18=0.0;
      for (i=1; i<=N; i++) {
	 mertens9(N/i, count, S, flag);
	 if (delta==0)
	    sum18=sum18+((double)count[1]-(double)count[0])*(double)M[i-1];
	 else {
	    if (delta==1)
	       sum18=sum18+((double)count[0]-(double)count[1]+1.0/6.0)*(double)M[i-1];
	    else {
	       if (delta==2)
		  sum18=sum18+(double)count[0]*(double)M[i-1];
	       else {
		  if (delta==3)
		     sum18=sum18+(double)count[1]*(double)M[i-1];
		  else {
		     if (delta==4) {
			sum18=sum18+((double)count[1]-(double)count[0])*(double)M[N/i-1];
//			printf(" %d %d \n",(int)count[1]-(int)count[0],M[N/i-1]);
			}
		     else
			sum18=sum18+((double)count[0]-(double)count[1]+1.0/6.0)*(double)M[N/i-1];
		     }
		  }
	       }
	    }
	 }
      fprintf(Outfp," %e\n",sum18);
      continue;
      }
   if (out==35) {
      sum17=0.0;
      tsum=0;
      for (i=1; i<=N; i++) {
	 mertens9(N/i, count, S, flag);
	 t=liouvile(i);
	 tsum=tsum+t;
	 if (delta==0)
	    sum17=sum17+((double)count[1]-(double)count[0])*((double)count[1]-(double)count[0])*(double)tsum*(double)t;
	 else {
	    if (flag==0)
	       sum17=sum17+((double)count[0]-(double)count[1]+1.0/6.0)*((double)count[1]-(double)count[0])*(double)tsum*(double)t;
	    else
	       sum17=sum17+((double)count[0]-(double)count[1]+1.0/4.0)*((double)count[1]-(double)count[0])*(double)tsum*(double)t;
	    }
	 }
      fprintf(Outfp," %e\n",sum17);
      continue;
      }
   if (out==36) {
      sum4=0.0;
      for (i=1; i<=N; i++) {
	 mertens9(N/i, count, S, flag);
	 if (delta==0)
	    sum4=sum4+((double)count[1]-(double)count[0])*numdiv(i,5);
	 else
	    sum4=sum4+((double)count[0]-(double)count[1]+1.0/6.0)*numdiv(i,5);
	 }
      fprintf(Outfp," %e\n",sum4);
      continue;
      }
   }
if ((out==17)&&(hflag==1)) {
   mean=0.0;
   msum2=0.0;
   for (i=0; i<201; i++) {
      fprintf(Outfp," %d\n",histo[i]);
      mean=mean+((double)i-100.0)*histo[i];
//    printf(" %d %d %d %e\n",(int)(i-100),histo[i],(int)(i-100)*histo[i],mean);
      msum2=msum2+((double)i-100.0)*histo[i]*((double)i-100.0);
      }
   msum=mean;
   mean=mean/(double)(MAXN-1);
   std=(double)(MAXN-1)*msum2-msum*msum;
   std=sqrt(std/((double)(MAXN-1)*(double)(MAXN-2)));
   printf("mean=%e, std=%e \n",mean,std);
   }
zskip:
free(S);
fclose(Outfp);
return;
}
