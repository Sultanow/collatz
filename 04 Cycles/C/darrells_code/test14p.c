/******************************************************************************/
/*									      */
/* COMPUTE THE MINIMUM AND MAXIMUM ODD ELEMENTS IN A CYCLE (M(l,n) and N(l,n))*/
/* 07/21/10 (dkc)							      */
/*									      */
/* This C program computes the minimum and maximum odd elements in a cycle.   */
/* "l" is the number of elements in a cycle (where the element following the  */
/* odd element i is (3i+c)/2) and "n" is the number of odd elements in the    */
/* cycle.								      */
/*									      */
/******************************************************************************/
void lshiftn(unsigned int *a, unsigned int *b, unsigned int shift, unsigned int n);
void addn(unsigned int *a, unsigned int *b, unsigned int n);
void subn(unsigned int *a, unsigned int *b, unsigned int n);
void copyn(unsigned int *a, unsigned int *b, unsigned int n);
void setn(unsigned int *a, unsigned int b, unsigned int n);
unsigned int divn_32(unsigned int *an,unsigned int *quotient, unsigned int dn,
	       unsigned int n);
unsigned int orn(unsigned int *a, unsigned int n);
#include <stdio.h> 
#include <math.h> 

int main () {
unsigned int l=14;  // number of odds and evens
unsigned int n=8;  // number of odds
unsigned int m=6; // number of words (hard-coded to 6)
unsigned int divisor=1; // check if value divides 2^l-3^n
unsigned int h,j,offset,count;
unsigned int sv[158],temp;
unsigned int A[6],B[6],C[6],M[6],Q[6],S[6],Z[6],O[6*100];
FILE *Outfp;
Outfp = fopen("out14n.dat","w");
   setn(Z, 0, 6);
   setn(O, 0, 6*100);
   printf("l=%d, n=%d \n",l,n);
   count=0;
   setn(M, 0, m);
sv[0]=1;
sv[1]=1;
sv[2]=1;
sv[3]=0;
sv[4]=1;
sv[5]=0;
sv[6]=1;
sv[7]=1;
sv[8]=1;
sv[9]=0;
sv[10]=0;
sv[11]=1;
sv[12]=0;
sv[13]=0;
//
// compute rotated parity vector using ceiling function
//
   for (offset=0; offset<n; offset++) {
//
// compute odd element
//
      temp=0;
      setn(S, 0, m);
      for (j=1; j<=l; j++) {
	 if (sv[j-1]!=0) {
	    setn(A, sv[j-1], m);
	    for (h=0; h<(n-temp-1); h++) {
	       copyn(A, B, m);
	       addn(A, A, m);
	       addn(B, A, m);
	       }
	    lshiftn(A, B, j-1, m);
	    addn(B, S, m);
	    if ((S[0]&0xc0000000)!=0) {
	       printf("error A: sum too large \n");
	       goto zskip;
	       }
	    }
	 temp=temp+sv[j];
	 }
      printf(" %d \n",S[m-1]);
      temp=sv[0];
      for (j=0; j<(l-1); j++)
	 sv[j]=sv[j+1];
      sv[l-1]=temp;
      while (sv[0]==0) {
	 temp=sv[0];
	 for (j=0; j<(l-1); j++)
	    sv[j]=sv[j+1];
	sv[l-1]=temp;
	}
     }
//
// compute 2*l-3^n
//
   printf("\n");
   setn(A, 1, m);
   for (h=0; h<n; h++) {	 // 3^n
      copyn(A, B, m);
      addn(A, A, m);
      addn(B, A, m);
      }
   setn(B, 1, m);
   lshiftn(B, C, l, m);        // 2^l
   subn(C, A, m);		 // 2^l-3^n
   if ((A[0]&0x80000000)!=0)	 // check if negative
      subn(Z, A, m);
   printf("|2^l-3^n|=%#05x %#010x %#010x %#010x %#010x %#010x \n",A[0],A[1],A[2],A[3],A[4],A[5]);
   h=divn_32(A, Q, divisor,m);
   printf(" quotient=%#05x %#010x %#010x %#010x %#010x %#010x \n",Q[0],Q[1],Q[2],Q[3],Q[4],Q[5]);
   copyn(Q, B, m);
   for (h=1; h<divisor; h++)
      addn(Q, B, m);
   printf("      sum=%#05x %#010x %#010x %#010x %#010x %#010x \n",B[0],B[1],B[2],B[3],B[4],B[5]);
zskip:
fclose(Outfp);
return(0);
} 
/*****************************************************************************/
/*									     */
/*  N-WORD BIT DIVIDE (UNSIGNED, 32-BIT DIVISOR)			     */
/*  02/11/12 (dkc)							     */
/*									     */
/*****************************************************************************/
unsigned int carry(unsigned int a, unsigned int b, unsigned int sum);
unsigned int lmbd(unsigned int mode, unsigned int a);
unsigned int divn_32(unsigned int *an,unsigned int *quotient, unsigned int dn,
	       unsigned int n) {
unsigned int i,j,dshift,ashift,count,flag,shift,c0,c1,temp0;
unsigned int a[1024],d[1024],temp[1024];

if (n>1024)
   return(0);
temp0=0;
for (i=0; i<(n-1); i++) {
   a[i]=an[i];
   d[i]=0;
   temp0=temp0|a[i];
   }
a[n-1]=an[n-1];
if ((temp0==0)&&(a[n-1]<dn)) {
   a[n-1]=0;
   goto zskip;
   }
d[n-1]=dn;
//
// divisor shift count
//
dshift=lmbd(1,dn);
dshift+=32*(n-1);
//
// dividend shift count
//
for (i=0; i<n; i++) {
   if (a[i]!=0) {
      ashift=32*i+lmbd(1,a[i]);
      break;
      }
   }
//
// count
//
shift=dshift-ashift;
count=shift+1;
//
// align dividend and divisor
//
flag=shift/32;
shift=shift-flag*32;
flag=n-1-flag;
if (flag!=(n-1)) {
   d[flag]=d[n-1];
   d[n-1]=0;
   }
if (shift!=0) {
   d[flag-1]=d[flag]>>(32-shift);
   d[flag]=d[flag]<<shift;
   }
//
// negate divisor
//
for (i=0; i<n; i++)
   d[i]=~d[i];
c0=1;
for (i=0; i<n; i++) {
   temp0=d[n-1-i]+c0;
   c0=carry(d[n-1-i],c0,temp0);
   d[n-1-i]=temp0;
   }
//
//  do additions
//
for (i=0; i<count; i++) {
   temp0=a[n-1];
   c0=0;
   for (j=0; j<(n-1); j++) {
      temp[n-1-j]=temp0+d[n-1-j];
      c0+=carry(temp0,d[n-1-j],temp[n-1-j]);
      temp0=a[n-2-j]+c0;
      c1=c0;
      c0=carry(a[n-2-j],c0,temp0);
      }
   temp[0]=a[0]+d[0]+c1;

   if ((temp[0]>>31)==0) {
      for (j=0; j<(n-1); j++) {
	 a[j]=temp[j]<<1;
	 if ((temp[j+1]>>31)!=0)
	    c0=1;
	 else
	    c0=0;
	 a[j]=a[j]+c0;
	 }
      a[n-1]=temp[n-1]<<1;
      a[n-1]=a[n-1]+1;
      }
   else {
      for (j=0; j<(n-1); j++) {
	 a[j]=a[j]<<1;
	 if ((a[j+1]>>31)!=0)
	    c0=1;
	 else
	    c0=0;
	 a[j]=a[j]+c0;
	 }
      a[n-1]=a[n-1]<<1;
      }
   }
//
// shift off extra bits
//
for (i=0; i<(flag-2); i++)
   a[i]=0;
if (shift!=0) {
   a[flag-1]=a[flag-1]<<shift;
   a[flag-1]=a[flag-1]>>shift;
   }
//
// store quotient
//
zskip:
for (i=0; i<n; i++)
   *(quotient+i)=a[i];
return(1);
}
/*****************************************************************************/
/*									     */
/*  LEFT-SHIFT 32*N BITS						     */
/*  10/16/09 (dkc)							     */
/*									     */
/*****************************************************************************/
void lshiftn(unsigned int *a, unsigned int *b, unsigned int shift, unsigned
	     int n) {
unsigned int i;
for (i=0; i<n; i++)
   *(b+i)=*(a+i);
while (shift>31) {
   for (i=0; i<n-1; i++)
      *(b+i)=*(b+i+1);
   *(b+n-1)=0;
   shift=shift-32;
   }
if (shift==0)
   return;
for (i=0; i<n-1; i++)
   *(b+i)=(*(b+i)<<shift)|(*(b+i+1)>>(32-shift));
*(b+n-1)=*(b+n-1)<<shift;
return;
}
/******************************************************************************
*									      *
*  N-WORD ADD								      *
*  04/19/10 (dkc)							      *
*									      *
******************************************************************************/
unsigned int carry(unsigned int a, unsigned int b, unsigned int sum);
void addn(unsigned int *a, unsigned int *b, unsigned int n) {
unsigned int i,s;
unsigned int c[8192];
if (n>8192)
   return;
for (i=n-1; i>0; i--) {
   s=*(a+i)+*(b+i);
   c[i]=carry(*(a+i),*(b+i),s);
   *(b+i)=s;
   }
*b=*a+*b;

for (i=n-2; i>0; i--) {
   s=*(b+i)+c[i+1];
   c[i]+=carry(*(b+i),c[i+1],s);
   *(b+i)=s;
   }
*b=*b+c[1];
return;
}
/******************************************************************************
*									      *
*  N-WORD SUBTRACT							      *
*  04/19/10 (dkc)							      *
*									      *
******************************************************************************/
unsigned int carry(unsigned int a, unsigned int b, unsigned int sum);
void subn(unsigned int *a, unsigned int *b, unsigned int n) {
unsigned int i,s,d;
unsigned int c[8192];
if (n>8192)
   return;
for (i=0; i<n; i++) {
   s=*(b+i);
   *(b+i)=~s;
   }

d=1;
for (i=n-1; i>0; i--) {
   s=*(b+i)+d;
   d=carry(*(b+i),d,s);
   *(b+i)=s;
   }
*b=*b+d;

for (i=n-1; i>0; i--) {
   s=*(a+i)+*(b+i);
   c[i]=carry(*(a+i),*(b+i),s);
   *(b+i)=s;
   }
*b=*a+*b;

for (i=n-2; i>0; i--) {
   s=*(b+i)+c[i+1];
   c[i]+=carry(*(b+i),c[i+1],s);
   *(b+i)=s;
   }
*b=*b+c[1];
return;
}
/******************************************************************************
*									      *
*  32*N-BIT COPY							      *
*  04/02/10 (dkc)							      *
*									      *
******************************************************************************/
void copyn(unsigned int *a, unsigned int *b, unsigned int n) {
unsigned int i;
for (i=0; i<n; i++)
   *(b+i)=*(a+i);
return;
}
/******************************************************************************
*									      *
*  SET THE LAST OF N WORDS						      *
*  04/02/10 (dkc)							      *
*									      *
******************************************************************************/
void setn(unsigned int *a, unsigned int b, unsigned int n) {
unsigned int i;
for (i=0; i<n-1; i++)
   *(a+i)=0;
*(a+n-1)=b;
return;
}
/******************************************************************************
*									      *
*  "OR" N WORDS TOGETHER                                                   *
*  04/02/10 (dkc)							      *
*									      *
******************************************************************************/
unsigned int orn(unsigned int *a, unsigned int n) {
unsigned int i,b;
b=*a;
for (i=1; i<n; i++)
   b=b|*(a+i);
return b;
}
/*****************************************************************************/
/*									     */
/*  LEFT-MOST BIT DETECTION						     */
/*  01/12/07 (dkc)							     */
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
/*****************************************************************************/
/*									     */
/*  CARRY								     */
/*  01/12/07 (dkc)							     */
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
