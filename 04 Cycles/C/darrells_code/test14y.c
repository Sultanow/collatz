/******************************************************************************/
/*									      */
/* COMPUTE THE MINIMUM AND MAXIMUM ODD ELEMENTS IN A CYCLE		      */
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
void subn(unsigned int *c, unsigned int *d, unsigned int n);
void copyn(unsigned int *a, unsigned int *b, unsigned int n);
void setn(unsigned int *a, unsigned int b, unsigned int n);
unsigned int orn(unsigned int *a, unsigned int n);
#include <stdio.h> 
#include <math.h> 
unsigned int ab[100*2]={ // a and b values
0, 1, 0, 2, 1, 2, 1, 3, 2, 3,
2, 4, 3, 4, 4, 4, 4, 5, 5, 5,
5, 6, 6, 6, 7, 6, 7, 7, 8, 7,
8, 8, 9, 8, 9, 9, 10, 9, 11, 9,
11, 10, 12, 10, 12, 11, 13, 11, 14, 11,
14, 12, 15, 12, 15, 13, 16, 13, 16, 14,
17, 14, 18, 14, 18, 15, 19, 15, 19, 16,
20, 16, 21, 16, 21, 17, 22, 17, 22, 18,
23, 18, 23, 19, 24, 19, 25, 19, 25, 20,
26, 20, 26, 21, 27, 21, 28, 21, 28, 22,
29, 22, 29, 23, 30, 23, 31, 23, 31, 24,
32, 24, 32, 25, 33, 25, 33, 26, 34, 26,
35, 26, 35, 27, 36, 27, 36, 28, 37, 28,
38, 28, 38, 29, 39, 29, 39, 30, 40, 30,
40, 31, 41, 31, 42, 31, 42, 32, 43, 32,
43, 33, 44, 33, 45, 33, 45, 34, 46, 34,
46, 35, 47, 35, 47, 36, 48, 36, 49, 36,
49, 37, 50, 37, 50, 38, 51, 38, 52, 38,
52, 39, 53, 39, 53, 40, 54, 40, 54, 41,
55, 41, 56, 41, 56, 42, 57, 42, 57, 43};
unsigned int m=6;  // number of 32-bit words
int main () {
unsigned int h,i,j,l,n,a,b,offset,length,count, period;
unsigned int sv[158],temp;
unsigned int A[6],B[6],L[6],M[6],S[6],T[6],O[6*100];
FILE *Outfp;
Outfp = fopen("out14y.dat","w");
setn(O, 0, 6*100);
for (i=0; i<100; i++) {
   length=3*ab[2*i]+2*ab[2*i+1]; // length of limb
   l=2*ab[2*i]+ab[2*i+1]+1; // length of loop
   n=ab[2*i]+ab[2*i+1];  // number of odd elements in loop
   printf("l=%d, n=%d, length=%d \n",l,n,length);
   count=0;
   setn(M, 0, m);
//
// compute rotated parity vector using ceiling function
//
   for (offset=0; offset<l; offset++) {
      for (j=1; j<=l; j++) {
         a=j*n;
         if (a==((a/l)*l))
            a=a/l;
         else
            a=(a/l)+1;
         b=(j-1)*n;
         if (b==((b/l)*l))
            b=b/l;
         else
            b=(b/l)+1;
         temp=j+offset;
         if (temp>l)
            temp=temp-l;
         sv[temp-1]=a-b;
         if (offset==0) 
            printf(" %d",sv[temp-1]);
	 }
      if (offset==0)
	 printf("\n");
      if (sv[0]==0)
	 continue;
//
// compute odd element
//
      setn(S, 0, m);
      temp=0;
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
//
// find maximum odd element
//
      copyn(S, T, m);
      subn(M, T, m);
      if ((T[0]&0x80000000)!=0)
	 copyn(S, M, m);
      if (offset==0)
	 copyn(S, L, m);
      copyn(S, O+m*count, m);
      count=count+1;
//    printf("offset=%d, sum=%#05x %#010x %#010x %#010x %#010x %#010x \n",offset,S[0], S[1], S[2], S[3],S[4],S[5]);
      }
   if (count!=n) {
      printf("error: incorrect number of odd elements \n");
      goto zskip;
      }
//
// find period
//
   period=n;
   for (h=1; h<count; h++) {
      copyn(O+m*h, T, m);
      subn(O, T, m);
      temp=orn(T, m);
      if (temp==0) {
//	 printf("period=%d \n",h);
	 period=h;
	 break;
	 }
      }
// printf("maximum=%#05x %#010x %#010x %#010x %#010x %#010x \n",M[0],M[1],M[2],M[3],M[4],M[5]);
//
// compare twice minimum odd element to maximum odd element
//
   addn(L, L, m);
   copyn(M, T, m);
   subn(L, T, m);
   if ((T[0]&0x80000000)!=0) {
      printf("error: twice minimum not greater than maximum \n");
      goto zskip;
      }
//
// compute maximum odd element from minimum odd element
//
   setn(A, 1, m);
   for (h=0; h<(n-1); h++) {
      copyn(A, B, m);
      addn(A, A, m);
      addn(B, A, m);
      }
   subn(L, A, m);
   if (period==n)
      goto yskip;
   if (period!=(n/2)) {
      printf("warning: maximum odd element not compared to minimum odd element \n");
      goto wskip;
      }
//
// adjustment for period equal to half number of odd elements
//
   copyn(A, T, m);
// printf(" %#05x %#010x %#010x %#010x %#010x %#010x \n",T[0],T[1],T[2],T[3],T[4],T[5]);
   setn(A, 1, m);
   for (h=0; h<(n-1-period); h++) {
      copyn(A, B, m);
      addn(A, A, m);
      addn(B, A, m);
      }
   lshiftn(A, A, l/2, m);
// printf(" %#05x %#010x %#010x %#010x %#010x %#010x \n",A[0],A[1],A[2],A[3],A[4],A[5]);
   subn(T, A, m);
//
// compare adjusted amount to maximum odd element
//
yskip:
   copyn(M, T, m);
   subn(A, T, m);
   temp=orn(T, m);
   if (temp!=0) {
      printf("error: incorrect maximum/minimum relationship \n");
      printf(" %#05x %#010x %#010x %#010x %#010x %#010x \n",T[0],T[1],T[2],T[3],T[4],T[5]);
      goto zskip;
      }
//
// compute largest odd element using floor function
//
wskip:
   temp=0;
   for (offset=0; offset<2; offset++) {
      for (j=1; j<=l; j++) {
	 a=j*n;
	 a=a/l;
	 b=(j-1)*n;
	 b=b/l;
	 temp=j+offset;
	 if (temp>l)
	    temp=temp-l;
	 sv[temp-1]=a-b;
	 }
      if (offset==1) {
	 for (h=0; h<l; h++)
	    printf(" %d",sv[h]);
	 printf("\n");
	 }
      if (sv[0]==0)
	 continue;
      setn(S, 0, m);
      temp=0;
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
      }
   subn(M, S, m);
   temp=orn(S, m);
   if (temp!=0) {
      printf("error: incorrect maximum odd element computed using floor function \n");
      printf("maximum=%#05x %#010x %#010x %#010x %#010x %#010x \n",S[0],S[1],S[2], S[3],S[4],S[5]);
      goto zskip;
      }
   printf("\n");
   }
zskip:
fclose(Outfp);
return(0);
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