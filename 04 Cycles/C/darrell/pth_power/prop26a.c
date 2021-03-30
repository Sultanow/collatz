/*****************************************************************************/
/*									     */
/*  FACTOR (a**p+b**p)/(a+b)						     */
/*  11/03/06 (dkc)							     */
/*									     */
/*  This C program determines if q is a pth power with respect to	     */
/*  (a**p+b**p)/(a+b).							     */
/*									     */
/*  The output is "a, b, (number of prime factors<<16)|code".  The code is   */
/*  set to 0, 1, 2, or 3 when p divides a, b, a-b, or a+b respectively.      */
/*  Setting "select" to 0, 1, 2, 3, 4, 5, or 6 selects a, b, a-b, a+b, p,    */
/*  2, or 2*p respectively as the base (q).  When "select" ranges from       */
/*  0 to 3, setting "pflag" to 1 multplies the base by p and setting "pflag" */
/*  to 2 multiplies the base by p*p.  2*p must divide a, b, a-b, or a+b      */
/*  when "split" is set to 0, and 2*p must not divide a, b, a-b, or a+b when */
/*  "split" is set to 1.  The prime factors of [(a**p-b**p)/(a-b)] must not  */
/*  be of the form p**2*k+1 when "psflag" is set to 0, and the prime factors */
/*  of [(a**p-b**p)/(a-b)] must be of the form p**2*k+1 when "psflag" is set */
/*  to 1.								     */
/*									     */
/*  Also, setting "select" to 7 selects the specified q value.               */
/*									     */
/*****************************************************************************/
#include <math.h>
#include <stdio.h>
#include "tablez.h"
unsigned int euclid(unsigned int d, unsigned int e);
void differ(unsigned int *a, unsigned int *b);
void bigbigs(unsigned int *a, unsigned int *b);
void bigbigd(unsigned int *a, unsigned int *b);
void bigbigq(unsigned int a0, unsigned int a1, unsigned int a2,
	     unsigned int a3, unsigned int *quotient, unsigned int d2,
	     unsigned int d3);
unsigned int lmbd(unsigned int mode, unsigned int a);
void dummy(unsigned int a, unsigned int b, unsigned int c);
void bigprod(unsigned int a, unsigned int b, unsigned int c, unsigned int *p);
void quotient(unsigned int *a, unsigned int *b, unsigned int);
void bigresx(unsigned int a, unsigned int b, unsigned int c, unsigned int d,
	     unsigned int *e, unsigned int f);
int main ()
{
unsigned int p=3; // input prime
unsigned int select=7; // select d, e, d-e, d+e, p, 2, or 2*p
unsigned int pflag=0;  // if set to 1, base = base*p for "select" = 0 to 3
                       // if set to 2, base = base*p*p for "select"=0 to 3
unsigned int split=2;  // if set to 0, 2*p must divide a, b, a-b, or a+b
		       // if set to 1, 2*p must not divide a, b, a-b, or a+b
		       // otherwise, no restrictions
unsigned int psflag=2; // if set to 1, factors must be of the form p**2*k+1
		       // if set to 0, factors must not be of the form p**2*k+1
		       // otherwise, factors can be of mixed types
unsigned int qflag=4;  // if set to 0, p must divide a+b
                       // if set to 1, p must divide a-b
                       // if set to 2, p must divide a
                       // if set to 3, p must divide b
		       // otherwise, no restrictions
unsigned int prmflg=0; // if set to 1, [(a**p-b**p)/(a-b)] must be prime
unsigned int sumdif=1; // if set to 1, do sum
unsigned int offset=0;  // offset into input array
unsigned int que=6;   // q value

unsigned int input[1000];
unsigned int terror[100];
unsigned int count;
unsigned int insize=1000;
unsigned int tmptab[15000];
unsigned int tsize=1228;
unsigned int tmpsiz;
unsigned int output[1000*3];
unsigned int outsiz=1000*3;
unsigned int save[20];  // solutions array
unsigned int savsiz=19;  // size of solutions array minus one
unsigned int d,e,icount;
unsigned int h,i,j,k,l,m,q;
unsigned int flag,base,temp,tflag,ps;
unsigned int S[2],T[2],U[2],V[2],W[2],X[3],Y[4],Z[4];
int zflag;
unsigned int n=0;
FILE *Outfp;
Outfp = fopen("out26a.dat","w");
/*********************************/
/*  extend prime look-up table	 */
/*********************************/
for (i=0; i<tsize; i++) tmptab[i] = (int)(table[i]);
tmpsiz=tsize;
for (d=10001; d<160000; d++) {
   if(d==(d/2)*2) continue;
   if(d==(d/3)*3) continue;
   if(d==(d/5)*5) continue;
   if(d==(d/7)*7) continue;
   if(d==(d/11)*11) continue;
   if(d==(d/13)*13) continue;
   if(d==(d/17)*17) continue;
   if(d==(d/19)*19) continue;
/************************************************/
/*  look for prime factors using look-up table	*/
/************************************************/
      l = (int)(100.0 + sqrt((double)d));
      k=0;
      if (l>table[tsize-1]) {
	 terror[0]=1;
aspin:	 goto aspin;
	 }
      else {
	 for (i=0; i<tsize; i++) {
	    if (table[i] < l) k=i;
	    else break;
	    }
	 }
      flag=1;
      l=k;
      for (i=0; i<=l; i++) {
	 k = table[i];
	 if ((d/k)*k == d) {
	    flag=0;
	    break;
	    }
	 }
      if (flag==1) {
	 tmptab[tmpsiz]=d;
	 tmpsiz = tmpsiz + 1;
	 }
   }
printf("tmpsiz=%d \n",tmpsiz);
/***********************************/
/*  factor (d**p + e**p)/(d + e)   */
/***********************************/
icount=0;
for (i=50; i>1; i--) {
   for (j=i-1; j>0; j--) {
      k=euclid(i,j);
      if (k==1) {
	 if (icount>999)
	   goto qskip;
	 input[icount]=(i<<16)+j;
	 icount=icount+1;
//	 if (icount<20)
//	    printf(" %d %d \n",i,j);
	 }
      }
   }
qskip:
printf("count=%d \n",icount);
//
terror[0]=0;	 // clear error array
terror[1]=0;
terror[2]=0;
ps=p*p;
zflag=0;
q=0;
count=0;
for (h=offset; h<icount; h++) {
   d=input[h]>>16;
   e=input[h]&0xffff;
//   if (h<20)
//      printf(" %d %d \n",d,e);
   if (qflag==0) {
      if (((d+e)/p)*p!=(d+e))
	 goto askip;
      }
   if (qflag==1) {
      if (((d-e)/p)*p!=(d-e))
	 goto askip;
      }
   if (qflag==2) {
      if ((d/p)*p!=d)
	 goto askip;
      }
   if (qflag==3) {
      if ((e/p)*p!=e)
	 goto askip;
      }
   if ((d/p)*p==d) {
      tflag=0;
      if (split==0) {
	 if ((d/2)*2!=d)
            goto askip;
         }
      if (split==1) {
         if ((d/2)*2==d)
            goto askip;
         }
      }
   if ((e/p)*p==e) {
      tflag=1;
      if (split==0) {
	 if ((e/2)*2!=e)
            goto askip;
         }
      if (split==1) {
         if ((e/2)*2==e)
            goto askip;
         }
      }
   if (((d+e)/p)*p==(d+e)) {
      if (sumdif==0)
	 tflag=3;
      else
	 tflag=2;
      if (split==0) {
	 if (((d+e)/2)*2!=(d+e))
            goto askip;
         }
      if (split==1) {
         if (((d+e)/2)*2==(d+e))
            goto askip;
         }
      }
   if (((d-e)/p)*p==(d-e)) {
      if (sumdif==0)
	 tflag=2;
      else
	 tflag=3;
      if (split==0) {
	 if (((d-e)/2)*2!=(d-e))
            goto askip;
         }
      if (split==1) {
         if (((d-e)/2)*2==(d-e))
            goto askip;
         }
      }
   if (select==0)
      base=d;
   if (select==1)
      base=e;
   if (select==2) {
      if (sumdif==0)
	 base=d-e;
      else
	 base=d+e;
      }
   if (select==3) {
      if (sumdif==0)
	 base=d+e;
      else
	 base=d-e;
      }
   if (pflag==1)
      base=base*p;
   if (pflag==2)
      base=base*p*p;
   if (select==4)
      base=p;
   if (select==5)
      base=2;
   if (select==6)
      base=2*p;
   if (select==7)
      base=que;
/************************************/
/*  compute (d**p + e**p)/(d + e)   */
/************************************/
      Y[0] = 0;
      Y[1] = 0;
      Y[2] = 0;
      Y[3] = d;
      for (i=0; i<p-1; i++) {
	 bigprod(Y[2], Y[3], d, X);
	 Y[1]=X[0];
	 Y[2]=X[1];
	 Y[3]=X[2];
	 }
      Z[0] = 0;
      Z[1] = 0;
      Z[2] = 0;
      Z[3] = e;
      for (i=0; i<p-1; i++) {
	 bigprod(Z[2], Z[3], e, X);
	 Z[1]=X[0];
	 Z[2]=X[1];
	 Z[3]=X[2];
	 }
      if (sumdif==1) {
	 bigbigs(Y, Z);
	 temp=d+e;
	 if (((d+e)/p)*p==(d+e))
	    temp=temp*p;
	 bigbigq(Z[0], Z[1], Z[2], Z[3], Y, 0, temp);
	 }
      else {
	 bigbigd(Y, Z);
	 temp=d-e;
	 if (((d-e)/p)*p==(d-e))
	    temp=temp*p;
	 bigbigq(Z[0], Z[1], Z[2], Z[3], Y, 0, temp);
	 }
      S[0]=Y[2];
      S[1]=Y[3];
      W[0]=S[0];
      W[1]=S[1];
/************************************************/
/*  look for prime factors using look-up table	*/
/************************************************/
      if (S[0]==0) {
	 l = (33 - lmbd(1, S[1]))/2;
	 l = 1 << l;
	 }
      else {
	 l = (65 - lmbd(1, S[0]))/2;
	 l = 1 << l;
	 }
      k=0;
      if (l>tmptab[tmpsiz-1]) {
	 flag=1;
	 k=tmpsiz-1;
	 }
      else {
	 flag=0;
	 for (i=0; i<tmpsiz; i++) {
	    if (tmptab[i] < l) k=i;
	    else break;
	    }
	 }
      l=k;
      m=0;
      for (i=0; i<=l; i++) {
	 k = tmptab[i];
	 quotient(S, T, k);
	 V[0]=T[0];
	 V[1]=T[1];
	 bigprod(T[0], T[1], k, X);
	 if ((S[0]!=X[1]) || (S[1]!=X[2])) continue;
	 if (psflag==1) {
            if (((k-1)/ps)*ps!=(k-1))
               goto askip;
            }
	 if (psflag==0) {
	    if (((k-1)/ps)*ps==(k-1))
               goto askip;
            }
	 bigresx(0, (k-1)/p, 0, k, U, base);
	 if ((U[0]!=0)||(U[1]!=1)) {
	    if (split==0) {
	       if (tflag==select)
		  terror[0]=8;
	       }
	    if (split==1) {
	       if ((tflag==0)&&(select==0))
		  terror[0]=8;
	       if ((tflag==1)&&(select==1))
		  terror[0]=8;
	       }
	    if (split==0) {
	       if ((select==5)&&((tflag==2)||(tflag==3)))
		  terror[0]=9;
	       }
	    goto askip;
	    }
aloop:	 S[0]=V[0];
	 S[1]=V[1];
	 save[m]=k;
	 if (m < savsiz) m=m+1;
	 else {
	    terror[0]=3;
	    goto bskip;
	    }
	 quotient(S, T, k);
	 V[0]=T[0];
	 V[1]=T[1];
	 bigprod(T[0], T[1], k, X);
	 if ((S[0]==X[1]) && (S[1]==X[2])) goto aloop;
	 }
/***********************************************/
/*  output prime factors satisfying criterion  */
/***********************************************/
      if ((S[0]!=0) || (S[1]!=1)) {
	 if (flag==1) {
	    if (S[0]==0) {
	       j = (33 - lmbd(1, S[1]))/2;
	       j = 1 << j;
	       }
	    else {
	       j = (65 - lmbd(1, S[0]))/2;
	       j = 1 << j;
	       }
	    for (i=tmptab[tmpsiz-1]; i<j; i+=2) {
	       quotient(S, T, i);
	       bigprod(T[0], T[1], i, X);
	       if ((X[1]==S[0]) && (X[2]==S[1])) {
		  if (psflag==1) {
                     if (((i-1)/ps)*ps!=(i-1))
                        goto askip;
                     }
		  if (psflag==0) {
		     if (((i-1)/ps)*ps==(i-1))
                        goto askip;
                     }
		  bigresx(0, (i-1)/p, 0, i, U, base);
		  if ((U[0]!=0)||(U[1]!=1)) {
		     if (split==0) {
			if (tflag==select)
			   terror[0]=8;
			}
		     if (split==1) {
			if ((tflag==0)&&(select==0))
			   terror[0]=8;
			if ((tflag==1)&&(select==1))
			   terror[0]=8;
			}
		     if (split==0) {
			if ((select==5)&&((tflag==2)||(tflag==3)))
			   terror[0]=9;
			}
		     goto askip;
		     }
		  if (T[0]<=4) {     // largest prime in table is 0x270eb
		     S[0]=T[0];
		     S[1]=T[1];
		     save[m]=i;
		     if (m < savsiz) m=m+1;
		     else {
			terror[0]=3;
			goto bskip;
			}
		     goto cskip;
		     }
		  else {
		     terror[0]=4;
		     goto bskip;
		     }
		  }
	       }
	    }
cskip:	 if (psflag==1) {
            T[0]=0;
            T[1]=1;
            differ(S,T);
            quotient(T,T,ps);
            bigprod(T[0],T[1],ps,X);
            T[0]=0;
            T[1]=1;
            differ(S,T);
            if ((X[1]!=T[0])||(X[2]!=T[1]))
               goto askip;
            }
	 if (psflag==0) {
            T[0]=0;
            T[1]=1;
            differ(S,T);
            quotient(T,T,ps);
            bigprod(T[0],T[1],ps,X);
            T[0]=0;
            T[1]=1;
            differ(S,T);
	    if ((X[1]==T[0])&&(X[2]==T[1]))
               goto askip;
            }
         T[0]=0;
	 T[1]=1;
	 differ(S, T);
	 quotient(T, T, p);
	 bigresx(T[0], T[1], S[0], S[1], T, base);
	 if ((T[0]==0)&&(T[1]==1)) {
            if (n+2>outsiz) {
	       terror[0]=6;
	       goto bskip;
	       }
	    output[n]=d;
	    output[n+1]=e;
	    output[n+2]=((m+1)<<16)|tflag;
            if (m>1) {
	       if (q<999) {
		  terror[2*q+2]=d;
		  terror[2*q+3]=e;
		  }
               q+=1;
	       terror[1]=q;
               }
            T[0]=S[0];
	    T[1]=S[1];
	    for (i=0; i<m; i++) {
	       bigprod(T[0], T[1], save[i], X);
	       T[0] = X[1];
	       T[1] = X[2];
	       }
	    if ((T[0]!=W[0]) || (T[1]!=W[1])) {
	       terror[0]=7;
	       goto bskip;
	       }
	    if ((prmflg==0)||(m==0)) {
	       n=n+3;
	       count=count+1;
	       }
	    }
	 else {
	    if (split==0) {
	       if (tflag==select)
		  terror[0]=8;
	       }
	    if (split==1) {
	       if ((tflag==0)&&(select==0))
		  terror[0]=8;
	       if ((tflag==1)&&(select==1))
		  terror[0]=8;
	       }
	    if (split==0) {
	       if ((select==5)&&((tflag==2)||(tflag==3)))
		  terror[0]=9;
	       }
	    goto askip;
	    }
	 }
      else {
	 if (n+2>outsiz) {
	    terror[0]=6;
	    goto bskip;
	    }
	 output[n]=d;
	 output[n+1]=e;
	 output[n+2]=(m<<16)|tflag;
         if (m>2) {
	    if (q<999) {
	       terror[2*q+2]=d;
	       terror[2*q+3]=e;
	       }
            q+=1;
	    terror[1]=q;
            }
         S[0]=0;
	 S[1]=1;
	 for (i=0; i<m; i++) {
	    bigprod(S[0], S[1], save[i], X);
	    S[0] = X[1];
	    S[1] = X[2];
	    }
	 if ((S[0]!=W[0]) || (S[1]!=W[1])) {
	    terror[0]=7;
	    goto bskip;
	    }
	 if ((prmflg==0)||(m==1)) {
	    n=n+3;
	    count=count+1;
	    }
	 }
askip:
   zflag=0;
   }
bskip:
output[n]=0xffffffff;
fprintf(Outfp," error0=%d \n",terror[0]);
fprintf(Outfp," count=%d \n",(n+1)/3);
for (i=0; i<(n+1)/3; i++)
   fprintf(Outfp," %d %d %#10x \n",output[3*i],output[3*i+1],
	   output[3*i+2]);
fclose(Outfp);
return(0);
}
/*****************************************************************************/
/*									     */
/*  EUCLIDEAN G.C.D.							     */
/*  11/14/06 (dkc)							     */
/*									     */
/*****************************************************************************/
unsigned int euclid(unsigned int d, unsigned int e) {
unsigned int a,b,temp;
a=d;
b=e;
if (b>a) {
   temp=a;
   a=b;
   b=temp;
   }
loop:
   temp=a-(a/b)*b;
   a=b;
   b=temp;
   if (b!=0) goto loop;
return(a);
}
/******************************************************************************
*									      *
*  COMPUTE 64-BIT DIFFERENCE						      *
*  11/13/06 (dkc)							      *
*									      *
******************************************************************************/
unsigned int carry(unsigned int a, unsigned int b, unsigned int sum);
void differ(unsigned int *a, unsigned int *b) {
unsigned int high0,high1,low0,low1,templo,temphi,temp,c;
high0=*a;
low0=*(a+1);
high1=*b;
low1=*(b+1);

low1=~low1;
high1=~high1;
temp=low1+1;
c=carry(low1,1,temp);
low1=temp;
high1+=c;

templo=low0+low1;
c=carry(low0,low1,templo);
temphi=high0+high1+c;
*b=temphi;
*(b+1)=templo;
return;
}
/******************************************************************************
*									      *
*  128-BIT SUM								      *
*  11/13/06 (dkc)							      *
*									      *
******************************************************************************/
unsigned int carry(unsigned int a, unsigned int b, unsigned int sum);
void bigbigs(unsigned int *a, unsigned int *b) {
unsigned int a0,a1,a2,a3,b0,b1,b2,b3;
unsigned int s0,s1,s2,s3,temp,c1,c2,c3;
a0=*a;
a1=*(a+1);
a2=*(a+2);
a3=*(a+3);
b0=*b;
b1=*(b+1);
b2=*(b+2);
b3=*(b+3);

s3=a3+b3;
c3=carry(a3,b3,s3);
s2=a2+b2;
c2=carry(a2,b2,s2);
s1=a1+b1;
c1=carry(a1,b1,s1);
s0=a0+b0;

temp=s2+c3;
c2+=carry(s2,c3,temp);
s2=temp;

temp=s1+c2;
c1+=carry(s1,c2,temp);
s1=temp;

s0=s0+c1;

*b=s0;
*(b+1)=s1;
*(b+2)=s2;
*(b+3)=s3;
return;
}
/*****************************************************************************/
/*									     */
/*  128/64 DIVISION (UNSIGNED)						     */
/*  11/14/06 (dkc)							     */
/*									     */
/*****************************************************************************/
unsigned int carry(unsigned int a, unsigned int b, unsigned int sum);
unsigned int lmbd(unsigned int mode, unsigned int a);
void bigbigq(unsigned int a0, unsigned int a1, unsigned int a2,
	     unsigned int a3, unsigned int *quotient, unsigned int d2,
	     unsigned int d3) {
unsigned int i,d0,d1,dshift,ashift,count,flag;
unsigned int shift,c,c0,c1,c2,temp,temp0,temp1,temp2,temp3;

if (d2==0) {
   if ((a0==0)&&(a1==0)&&(a2==0)&&(a3<d3)) {
      *quotient=0;
      *(quotient+1)=0;
      *(quotient+2)=0;
      *(quotient+3)=0;
      return;
      }
   }
else {
   if ((a0==0)&&(a1==0)&&(a2<d2)) {
      *quotient=0;
      *(quotient+1)=0;
      *(quotient+2)=0;
      *(quotient+3)=0;
      return;
      }
   }
dshift=lmbd(1,d2);
if (d2==0)
   dshift+=lmbd(1,d3);
dshift+=64;

ashift=lmbd(1,a0);
if (a0==0)
   ashift+=lmbd(1,a1);
if ((a0|a1)==0)
   ashift+=lmbd(1,a2);
if ((a0|a1|a2)==0)
   ashift+=lmbd(1,a3);

shift=dshift-ashift;
count=shift+1;
d0=0;
d1=0;
if (shift<32) {
   if (shift!=0) {
      d1=d2>>(32-shift);
      d2=(d2<<shift)|(d3>>(32-shift));
      }
   else
      d1=0;		       // added to get MSVC to work
   d3=d3<<shift;
   flag=3;
   shift=32-shift;
   }
else {
   shift=shift-32;
   d1=d2;
   d2=d3;
   d3=0;
   if (shift<32) {
      if (shift!=0) {
	 d0=d1>>(32-shift);
	 d1=(d1<<shift)|(d2>>(32-shift));
	 }
      else
	 d0=0;		       // added to get MSVC to work
      d2=d2<<shift;
      flag=2;
      shift=32-shift;
      }
   else {
      shift=shift-32;
      d0=d1;
      d1=d2;
      d2=0;
      if (shift<32) {
	 if (shift!=0)	       // added to get MSVC to work
	    d0=(d0<<shift)|(d1>>(32-shift));
	 d1=d1<<shift;
	 flag=1;
	 shift=32-shift;
	 }
      else {
	 shift=shift-32;
	 d0=d1;
	 d1=0;
	 d0=d0<<shift;
	 flag=0;
	 shift=32-shift;
	 }
      }
   }

d0=~d0;
d1=~d1;
d2=~d2;
d3=~d3;
temp=d3+1;
c=carry(d3,1,temp);
d3=temp;

temp=d2+c;
c=carry(d2,c,temp);
d2=temp;

temp=d1+c;
c=carry(d1,c,temp);
d1=temp;

d0=d0+c;
for (i=0; i<count; i++) {
   temp3=a3+d3;
   c2=carry(a3,d3,temp3);

   temp=a2+c2;
   c1=carry(a2,c2,temp);

   temp2=temp+d2;
   c1+=carry(temp,d2,temp2);

   temp=a1+c1;
   c0=carry(a1,c1,temp);

   temp1=temp+d1;
   c0+=carry(temp,d1,temp1);

   temp0=a0+d0+c0;
   if ((temp0>>31)==0) {
      a0=temp0<<1;
      if ((temp1>>31)!=0)
	 c=1;
      else
	 c=0;
      a0=a0+c;
      a1=temp1<<1;
      if ((temp2>>31)!=0)
	 c=1;
      else
	 c=0;
      a1=a1+c;
      a2=temp2<<1;
      if ((temp3>>31)!=0)
	 c=1;
      else
	 c=0;
      a2=a2+c;
      a3=temp3<<1;
      a3=a3+1;
      }
   else {
      a0=a0<<1;
      if ((a1>>31)!=0)
	 c=1;
      else
	 c=0;
      a0=a0+c;

      a1=a1<<1;
      if ((a2>>31)!=0)
	 c=1;
      else
	 c=0;
      a1=a1+c;

      a2=a2<<1;
      if ((a3>>31)!=0)
	 c=1;
      else
	 c=0;
      a2=a2+c;

      a3=a3<<1;
      }
   }
shift=shift-1;
if (flag==3) {
   a0=0;
   a1=0;
   a2=0;
   a3=a3<<shift;
   a3=a3>>shift;
   }
else {
   if (flag==2) {
      a0=0;
      a1=0;
      a2=a2<<shift;
      a2=a2>>shift;
      }
   else {
      if (flag==1) {
	 a0=0;
	 a1=a1<<shift;
	 a1=a1>>shift;
	 }
      else {
	 a0=a0<<shift;
	 a0=a0>>shift;
	 }
      }
   }
*quotient=a0;
*(quotient+1)=a1;
*(quotient+2)=a2;
*(quotient+3)=a3;
return;
}
/*****************************************************************************/
/*									     */
/*  LEFT-MOST BIT DETECTION						     */
/*  11/15/06 (dkc)							     */
/*									     */
/*****************************************************************************/
unsigned int lmbd(unsigned int mode, unsigned int a) {
unsigned int i,mask,count;
if (mode==0)
   a=~a;
if (a==0)
   return(32);
count=0;
mask=0x80000000;
for (i=0; i<32; i++) {
   if ((a&mask)!=0)
      break;
   count+=1;
   mask>>=1;
   }
return(count);
}
/******************************************************************************
*									      *
*  64x32 MULTIPLY (UNSIGNED)						      *
*  11/14/06 (dkc)							      *
*									      *
******************************************************************************/
unsigned int carry(unsigned int a, unsigned int b, unsigned int sum);
void bigprod(unsigned int a0, unsigned int a2, unsigned int m0,
	     unsigned int *product) {
unsigned int a1,a3,m1,temp;
unsigned int p0,p1,p2,p3,p4,p5,p6,p7;
unsigned int s1,s2,s3;
unsigned int c2,c3;
a1=a0&0xffff;
a0=a0>>16;
a3=a2&0xffff;
a2=a2>>16;
m1=m0&0xffff;
m0=m0>>16;
p0=a0*m0;
p1=a0*m1;
p2=a1*m0;
p3=a1*m1;
p4=a2*m0;
p5=a2*m1;
p6=a3*m0;
p7=a3*m1;

s3=p7+(p6<<16);
c3=carry(p7,(p6<<16),s3);
temp=s3+(p5<<16);
c3+=carry(s3,(p5<<16),temp);
s3=temp;

s2=p4+(p6>>16);
c2=carry(p4,(p6>>16),s2);
temp=s2+(p5>>16);
c2+=carry(s2,(p5>>16),temp);
s2=temp;
temp=s2+p3;
c2+=carry(s2,p3,temp);
s2=temp;
temp=s2+(p2<<16);
c2+=carry(s2,(p2<<16),temp);
s2=temp;
temp=s2+(p1<<16);
c2+=carry(s2,(p1<<16),temp);
s2=temp;

s1=p0+(p2>>16);
s1=s1+(p1>>16);

temp=s2+c3;
c2+=carry(s2,c3,temp);
s2=temp;

s1=s1+c2;

*product=s1;
*(product+1)=s2;
*(product+2)=s3;
return;
}
/*****************************************************************************/
/*									     */
/*  64/32 UNSIGNED DIVIDE						     */
/*  11/14/06 (dkc)							     */
/*									     */
/*****************************************************************************/
unsigned int lmbd(unsigned int mode, unsigned int a);
unsigned int carry(unsigned int a, unsigned int b, unsigned int sum);
void quotient(unsigned int *dividend, unsigned int *quotient,
	      unsigned int divisor) {
unsigned int i;
unsigned int shift,dshift,ashift,d0,d1,div0,div1,c,temp0,temp1,t0,t1;
d0=*dividend;
d1=*(dividend+1);
if ((d0==0)&&(d1<divisor)) {
   *quotient=0;
   *(quotient+1)=0;
   return;
   }

dshift=lmbd(1, divisor);
dshift=dshift+32;
ashift=lmbd(1, d0);
if (d0==0)
   ashift+=lmbd(1, d1);
shift=dshift-ashift;
if (shift<32) {
   div1=divisor<<shift;
   if (shift!=0)
      div0=divisor>>(32-shift);
   else 			      // added to get MSVC to work
      div0=0;			      //
   }
else {
   if (shift!=32)
      div0=divisor<<(shift-32);
   else 			      // added to get MSVC to work
      div0=divisor;
   div1=0;
   }
t0=~div0;
t1=~div1;
temp1=t1+1;
c=carry(t1,1,temp1);
t1=temp1;
t0=t0+c;
shift+=1;
for (i=0; i<shift; i++) {
   temp1=d1+t1;
   c=carry(d1,t1,temp1);
   temp0=d0+t0+c;
   if ((temp0>>31)==0) {
      d0=temp0<<1;
      if ((temp1>>31)!=0)
	 c=1;
      else
	 c=0;
      d0=d0+c;
      d1=temp1<<1;
      d1=d1+1;
      }
   else {
      d0=d0<<1;
      if ((d1>>31)!=0)
	 c=1;
      else
	 c=0;
      d0=d0+c;
      d1=d1<<1;
      }
   }
if (shift>32) {
   d0=d0<<(64-shift);
   d0=d0>>(64-shift);
   }
else {
   d0=0;
   if (shift!=32) {		// added to get MSVC to work
      d1=d1<<(32-shift);
      d1=d1>>(32-shift);
      }
   }
*quotient=d0;
*(quotient+1)=d1;
return;
}
/******************************************************************************/
/*									      */
/*  FIND LEAST RESIDUE (UNSIGNED)					      */
/*  11/13/06 (dkc)							      */
/*									      */
/*  This C subroutine finds the least residue of "base**exponent" modulus q.  */
/*  The binary representation of the exponent is used to efficiently compute  */
/*  the least residue.							      */
/*									      */
/******************************************************************************/
void differ(unsigned int *a, unsigned int *b);
void bigbigp(unsigned int d0, unsigned int d1, unsigned int m0, unsigned int m1,
	     unsigned int *output);
void bigbigq(unsigned int a0, unsigned int a1, unsigned int a2, unsigned int a3,
	     unsigned int *quotient, unsigned int d0, unsigned int d1);
void bigbigd(unsigned int *subtrahend, unsigned int *minuend);
void quotient(unsigned int *dividend, unsigned int *quotient,
	      unsigned int divisor);
void bigprod(unsigned int a0, unsigned int a1, unsigned int m0,
	     unsigned int *output);
void bigresx(unsigned int exp0, unsigned int exp1, unsigned int q0,
	     unsigned int q1, unsigned int *output, unsigned int base) {
int i,index;
unsigned int save[128];
unsigned int B[2],E[2],F[2],T[2],P[4],R[4],X[3];

B[0]=0; 		     // load base
B[1]=base;		     // load base
if ((q0==0)&&(base>q1)) {    // check for base greater than q
   quotient(B, T, q1);	     // base/q1
   bigprod(T[0], T[1], q1, X);	// (base/q1)*q1
   T[0]=X[1];
   T[1]=X[2];
   differ(B, T);	     // base=base-(base/q1)*q1
   B[0]=T[0];
   B[1]=T[1];
   }
if (base==1)		  // check for base equal to 1
   goto askip;
E[0]=0; 		  // load exponent
E[1]=1; 		  // load exponent
F[0]=exp0;		  // load input exponent
F[1]=exp1;		  // load input exponent
for (i=0; i<64; i++) {
   save[2*i]=B[0];		      // save base
   save[2*i+1]=B[1];		      // save base
   bigprod(E[0], E[1], 2, X);	      // exponent=exponent*2
   if (X[1]>F[0]) {		      // compare exponent to input exponent
      index=i;
      break;
      }
   if ((X[1]==F[0])&&(X[2]>F[1])) {   // compare exponent to input exponent
      index=i;
      break;
      }
   E[0]=X[1];
   E[1]=X[2];
   bigbigp(B[0], B[1], B[0], B[1], P);		      // base=base**2
   bigbigq(P[0], P[1], P[2], P[3], R, q0, q1);	      // base/q
   bigbigp(R[2], R[3], q0, q1, R);		      // (base/q)*q
   bigbigd(P, R);				      // base=base-(base/q)*q
   B[0]=R[2];
   B[1]=R[3];
   if ((E[0]==F[0])&&(E[1]==F[1]))    // compare exponent to input exponent
      goto askip;
   }
B[0]=save[2*index];		      // load base
B[1]=save[2*index+1];		      // load base
T[0]=E[0];
T[1]=E[1];
differ(F, T);			      // input exponent-=exponent
F[0]=T[0];
F[1]=T[1];
for (i=index-1; i>=0; i--) {
   quotient(E, E, 2);		      // exponent=exponent/2
   T[0]=E[0];
   T[1]=E[1];
   differ(F, T);		      // input exponent-=exponent
   if ((T[0]&0x80000000)==0) {	      // check if non-negative
      F[0]=T[0];
      F[1]=T[1];
      bigbigp(B[0], B[1], save[2*i], save[2*i+1], P); // base*=save[i]
      bigbigq(P[0], P[1], P[2], P[3], R, q0, q1);     // base/q
      bigbigp(R[2], R[3], q0, q1, R);		      // (base/q)*q
      bigbigd(P, R);				  // base=base-(base/p)*q
      B[0]=R[2];
      B[1]=R[3];
      }
   if ((F[0]==0)&&(F[1]==0))	      // check if input exponent is zero
      break;
   }
askip:
*output=B[0];			      // store least residue
*(output+1)=B[1];		      // store least residue
}
/*****************************************************************************/
/*									     */
/*  CARRY								     */
/*  11/15/06 (dkc)							     */
/*									     */
/*****************************************************************************/
unsigned int carry(unsigned int a, unsigned int b, unsigned int sum) {
unsigned int c;
if ((a&b)>>31!=0)
   c=1;
else {
   if ((a|b)>>31==0)
      c=0;
   else {
      if (sum>=0x80000000)
	 c=0;
      else
	 c=1;
      }
   }
return c;
}
/******************************************************************************
*									      *
*  128-BIT DIFFERENCE							      *
*  11/13/06 (dkc)							      *
*									      *
******************************************************************************/
unsigned int carry(unsigned int a, unsigned int b, unsigned int sum);
void bigbigd(unsigned int *a, unsigned int *b) {
unsigned int a0,a1,a2,a3,b0,b1,b2,b3;
unsigned int s0,s1,s2,s3,temp,c,c1,c2,c3;
a0=*a;
a1=*(a+1);
a2=*(a+2);
a3=*(a+3);
b0=*b;
b1=*(b+1);
b2=*(b+2);
b3=*(b+3);

b0=~b0;
b1=~b1;
b2=~b2;
b3=~b3;
temp=b3+1;
c=carry(b3,1,temp);
b3=temp;
temp=b2+c;
c=carry(b2,c,temp);
b2=temp;
temp=b1+c;
c=carry(b1,c,temp);
b1=temp;
b0=b0+c;

s3=a3+b3;
c3=carry(a3,b3,s3);
s2=a2+b2;
c2=carry(a2,b2,s2);
s1=a1+b1;
c1=carry(a1,b1,s1);
s0=a0+b0;

temp=s2+c3;
c2+=carry(s2,c3,temp);
s2=temp;

temp=s1+c2;
c1+=carry(s1,c2,temp);
s1=temp;

s0=s0+c1;

*b=s0;
*(b+1)=s1;
*(b+2)=s2;
*(b+3)=s3;
return;
}
/******************************************************************************
*									      *
*  64x64 MULTIPLY (UNSIGNED)						      *
*  11/14/06 (dkc)							      *
*									      *
******************************************************************************/
unsigned int carry(unsigned int a, unsigned int b, unsigned int sum);
void bigbigp(unsigned int a0, unsigned int a2, unsigned int m0,
	     unsigned int m2, unsigned int *product) {
unsigned int a1,a3,m1,m3,temp;
unsigned int p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15;
unsigned int s0,s1,s2,s3;
unsigned int c1,c2,c3;
a1=a0&0xffff;
a0=a0>>16;
a3=a2&0xffff;
a2=a2>>16;
m1=m0&0xffff;
m0=m0>>16;
m3=m2&0xffff;
m2=m2>>16;

p0=a0*m0;

p1=a0*m1;
p2=a1*m0;

p3=a0*m2;
p4=a1*m1;
p5=a2*m0;

p6=a0*m3;
p7=a1*m2;
p8=a2*m1;
p9=a3*m0;

p10=a1*m3;
p11=a2*m2;
p12=a3*m1;

p13=a2*m3;
p14=a3*m2;

p15=a3*m3;

s3=p15+(p14<<16);
c3=carry(p15,(p14<<16),s3);
temp=s3+(p13<<16);
c3+=carry(s3,(p13<<16),temp);
s3=temp;

s2=p12+(p14>>16);
c2=carry(p12,(p14>>16),s2);
temp=s2+(p13>>16);
c2+=carry(s2,(p13>>16),temp);
s2=temp;
temp=s2+p11;
c2+=carry(s2,p11,temp);
s2=temp;
temp=s2+p10;
c2+=carry(s2,p10,temp);
s2=temp;
temp=s2+(p9<<16);
c2+=carry(s2,(p9<<16),temp);
s2=temp;
temp=s2+(p8<<16);
c2+=carry(s2,(p8<<16),temp);
s2=temp;
temp=s2+(p7<<16);
c2+=carry(s2,(p7<<16),temp);
s2=temp;
temp=s2+(p6<<16);
c2+=carry(s2,(p6<<16),temp);
s2=temp;

s1=p5+(p9>>16);
c1=carry(p5,(p9>16),s1);
temp=s1+(p8>>16);
c1+=carry(s1,(p8>>16),temp);
s1=temp;
temp=s1+(p7>>16);
c1+=carry(s1,(p7>>16),temp);
s1=temp;
temp=s1+(p6>>16);
c1+=carry(s1,(p6>>16),temp);
s1=temp;
temp=s1+p4;
c1+=carry(s1,p4,temp);
s1=temp;
temp=s1+p3;
c1+=carry(s1,p3,temp);
s1=temp;
temp=s1+(p2<<16);
c1+=carry(s1,(p2<<16),temp);
s1=temp;
temp=s1+(p1<<16);
c1+=carry(s1,(p1<<16),temp);
s1=temp;

s0=p0+(p2>>16);
s0=s0+(p1>>16);

temp=s2+c3;
c2+=carry(s2,c3,temp);
s2=temp;

temp=s1+c2;
c1+=carry(s1,c2,temp);
s1=temp;

s0=s0+c1;

*product=s0;
*(product+1)=s1;
*(product+2)=s2;
*(product+3)=s3;
return;
}
