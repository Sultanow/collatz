/*  FIND CYCLES IN THE 3N+C SEQUENCE (cycles with attachment points only)    */
/*  08/03/19 (dkc)							     */
/*									     */
/*  This C program finds cycles in the 3n+c sequence.  Attachment points are */
/*  output.  Double-precision (64-bit) words are used.	Robert Floyd's       */
/*  cycle finding algorithm (Knuth 1969, "The Art of Computer Programming,   */
/*  Vol. 1: Fundamental Algorithms", pp. 4-7, exercise 7) is used.           */
/*									     */
/*  The output array "sinp" contains the attachment points.  The values in   */
/*  the output array "cflag" indicate which attachment points are associated */
/*  with a cycle.  The output array "size" gives the total number of         */
/*  attachment points for each c value.  The output array "cval" gives the   */
/*  corresponding c values.  The output variable "numbc" gives the number    */
/*  of c values.  After some minor editing to fill in array sizes, the	     */
/*  output can be made into an "include" file for other programs.            */
/*									     */
/*  Output "big" cycles indicate whether parameters (in particular "limit")  */
/*  need to be modified to find cycles that may have been missed.  "L" is    */
/*  is the number of even elements in the cycles, "K" is the number of odd   */
/*  elements, and "a" is the number of primary attachment points.  "count"   */
/*  gives the number of cycles found for a given c value.		     */
/*									     */
/*****************************************************************************/
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include "table5.h"
int main () {
//
unsigned int cstart=1;	     // beginning c value
unsigned int cend=209999;       // ending c value
unsigned int iters=1;	    // number of "jumps" from the initial odd integer
			    // divisible by 3.	The parameters "iters", "limit",
			    // and "max" have to be tuned to find all cycles in
			    // a reasonable amount of time.  Increasing "iters"
			    // usually decreases execution time.  When "iters"
			    // is increased, "max" must usually be increased
			    // (up to at most about 0x4000000).  A good c value
			    // to tune the parameters with is 17021 (where
			    // there should be 256 cycles).  For some larger
			    // c values, "iters" must be set to 2 to find all
			    // cycles.	When "iters" is set to 0, every "twig"
			    // of the tree in the range determined by "limit"
			    // is checked.
unsigned long long limit=30000000; // must be a multiple of 6, should be as large as
			    // possible relative to "max" to allow for cycles
			    // having a constrained dynamic range.  Increasing
			    // "limit" increases the execution time.  Cycles with
			    // (K+L,K) values equal to continued-fraction convergents
			    // of log(3)/log(2) are most likely to be missed if
			    // the parameters haven't been set properly.
unsigned long long max=0x40000000000; // maximum allowable value 
unsigned int outflg=0;	 // output flag (set to 0 if mode!=0)
unsigned int mode=1;  // if set, output only one attachment point
int i;
unsigned int first,acount,kount,attind,o,gomind,gop;
unsigned int c,t,e,g,h,j,n,count,total,flag,wrap,oldlop,lodds,bigind;
unsigned char cyc[300000];
unsigned int lopcnt[20000];
unsigned int cyccnt[20000];
unsigned int factor[20000];
unsigned long long gom[10000*3];
unsigned int index,z,lopind,lop;
long long K,T,U,L,C,V,Y,P,KP;
unsigned long long MIN,MAX;
errno_t err;
FILE *Outfp;
err = fopen_s(&Outfp,"outp.dat","w");
if (cstart==(cstart/3)*3) {
   printf("c divisible by 3 \n");
   goto zskip;
   }
if (cstart==(cstart/2)*2) {
   printf("c divisible by 2 \n");
   goto zskip;
   }
if (cend==(cend/3)*3) {
   printf("c divisible by 3 \n");
   goto zskip;
   }
if (cend==(cend/2)*2) {
   printf("c divisible by 2 \n");
   goto zskip;
   }
if (cend>3*199999) {
   printf("Warning: Cycles may not be primitive and may have elements that won't fit in double-words. \n");
   goto zskip;
   }
bigind=0;
fprintf(Outfp,"// c=%d \n",cstart);
if (outflg!=0)
   fprintf(Outfp,"long long sinp[     ]={ \n");
else {
	if (mode==0)
       fprintf(Outfp,"unsigned int count[     ]={ \n");
	else
	   fprintf(Outfp,"long long sinp[    ]={ \n"); 
   }
wrap=15;
index=0;
lopind=0;
total=0;
attind=0;
for (c=cstart; c<=cend; c+=2) {
   if (c==(c/3)*3)
      continue;
   printf("c=%d \n",c);
   kount=0;
   gomind=0;
   C=(long long)c;
   count=0;
   z=0;
   if (c!=1) {
      factor[0]=c;
      count=1;
      for (i=1; i<25996; i++) {
	 t=(unsigned int)table[i];
	 if (t>c)
	    break;
	 if (c==(c/t)*t) {
	    factor[count]=t;
	    count=count+1;
	    }
	 }
      }
   lop=0;
   for (i=3-(int)limit; i<(int)limit; i+=6) {
      K=(long long)i;
      for (h=0; h<iters; h++) {
	 K=K+C;
	 n=0;			    // clear count
	 while ((K&3)==0) {
	    K=K>>2;
	    n=n+2;
	    }
	 if ((K&1)==0) {
	    K=K>>1;
	    n=n+1;
	    }
	 flag=0;
	 if (K<0) {
	    K=-K;
	    flag=1;
	    }
	 for (j=0; j<n; j++) {
	    if ((unsigned long long)K>max)
	       goto askip;
	    L=K<<1;
	    K=K+L;
	    }
	 if (flag!=0)
	    K=-K;
	 K=K-C;
	 K=K>>1;
	 if ((K&1)==0)
	    goto askip;
	 }
//
// check if primitive
//
      T=K;
      if (T<0)
	 T=-T;
      for (e=0; e<count; e++) {
	 t=factor[e];
	 U=T/(unsigned long long)t;
	 P=U*(unsigned long long)t;
	 if (T==P)
	    goto askip;
	 }
//
      L=K<<1;
      K=K+L;
      K=K+C;
      T=K;
      if (T<0)
	 T=-T;
      if ((unsigned long long)T>max)
	 goto askip;
      while ((K&3)==0)
	 K=K>>2;
      if ((K&1)==0)
	 K=K>>1;
//
      KP=K;
      L=KP<<1;
      KP=KP+L;
      KP=KP+C;
      T=KP;
      if (T<0)
	 T=-T;
      if ((unsigned long long)T>max)
	 goto askip;
      while ((KP&3)==0)
	 KP=KP>>2;
      if ((KP&1)==0)
	 KP=KP>>1;
//
// begin loop
//
bloop:L=K<<1;
      K=K+L;
      K=K+C;
      T=K;
      if (T<0)
	 T=-T;
      if ((unsigned long long)T>max)
	 goto askip;
      while ((K&3)==0)
	 K=K>>2;
      if ((K&1)==0)
	 K=K>>1;
//
      L=KP<<1;
      KP=KP+L;
      KP=KP+C;
      while ((KP&3)==0)
	 KP=KP>>2;
      if ((KP&1)==0)
	 KP=KP>>1;
      L=KP<<1;
      KP=KP+L;
      KP=KP+C;
      T=KP;
      if (T<0)
	 T=-T;
      if ((unsigned long long)T>max)
	 goto askip;
      while ((KP&3)==0)
	 KP=KP>>2;
      if ((KP&1)==0)
	 KP=KP>>1;
      if (K!=KP)
	 goto bloop;
//
// find attachment point
//
      MIN=KP;
      MAX=KP;
      flag=0;
      L=K<<1;
      K=K+L;
      K=K+C;
      g=1;
      if ((K&7)==0) {
	 Y=K;
	 flag=1;
	 }
      while ((K&3)==0) {
	 K=K>>2;
	 g=g+2;
	 }
      if ((K&1)==0) {
	 K=K>>1;
	 g=g+1;
	 }
      if ((unsigned long long)K<MIN)
	 MIN=K;
      if ((unsigned long long)K>MAX)
	 MAX=K;
      o=1;
      while (K!=KP) {
	 L=K<<1;
	 K=K+L;
	 K=K+C;
	 g=g+1;
	 if (((K&7)==0)&&(flag==0)) {
	    Y=K;
	    flag=1;
	    }
	 while ((K&3)==0) {
	    K=K>>2;
	    g=g+2;
	    }
	 if ((K&1)==0) {
	    K=K>>1;
	    g=g+1;
	    }
	 if ((unsigned long long)K<MIN)
	    MIN=K;
	 if ((unsigned long long)K>MAX)
	    MAX=K;
	 o=o+1;
	 }
      if (flag==0)
	 goto askip;
      gop=(g<<16)|o;
      for (h=0; h<gomind; h++) {
	 if ((unsigned long long)gop==gom[3*h]) {
	    if ((MIN==gom[3*h+1])&&(MAX==gom[3*h+2]))
	       goto askip;
	    }
	 }
      gom[3*gomind]=(unsigned long long)gop;
      gom[3*gomind+1]=MIN;
      gom[3*gomind+2]=MAX;
      gomind=gomind+1;
      if (gomind>9999) {
	 printf("array not big enough (gom) \n");
	 goto zskip;
	 }
//
// new cycle
//
      kount=kount+1;
      oldlop=lop;
      lodds=0;
      acount=0;
      flag=0;
      T=Y;
      V=T;
//
// begin loop
//
aloop:first=0;
      while ((T&3)==0) {
	 T=T>>2;
	 if ((T&1)==1)
	    goto qskip;
	 if (first==0) {
	    acount=acount+1;
	    first=1;
	    }
	 printf("i=%lld, n=%d, k=%lld \n",i,g,T);
	 if ((outflg==0)&&(mode!=0)&&(flag==0)) {
		 fprintf(Outfp," %lld,",T);
		total=total+1;
		if (total>wrap) {
		   fprintf(Outfp,"\n");
		   total=0;
		   }
		flag=1;
	    }
	 if (outflg!=0) {
	    fprintf(Outfp," %lld,",T);
	    total=total+1;
	    if (total>wrap) {
	       fprintf(Outfp,"\n");
	       total=0;
	       }
	    cyc[index]=z;
	    index=index+1;
	    if (index>299999) {
	       printf("error: array not big enough (cyc) \n");
	       goto zskip;
	       }
	    }
	 lop=lop+1;
	 if (T==V) {
	    acount=acount-1;
	    goto rskip;
	    }
	 }
      if ((T&1)==0)
	 T=T>>1;
qskip:lodds=lodds+1;
      L=T<<1;
      T=T+L;
      T=T+C;
      if (T!=V)
	 goto aloop;
rskip:printf("L=%d, K=%d, a=%d, count=%d \n",g-2*lodds,lodds,acount,kount);
      printf("\n");
      z=z+1;
askip:n=0;
      }
   cyccnt[lopind]=kount;
   lopcnt[lopind]=lop;
   lopind=lopind+1;
   if (lopind>19999) {
      printf("array not big enough (lopcnt) \n");
      goto zskip;
      }
   printf("\n");
   }
//
// output results
//
if (outflg==0) {
	if (mode!=0) {
	   fprintf(Outfp,"\n");
	   fprintf(Outfp,"unsigned int count=[    ]={ \n");
	   }
   kount=0;
   total=0;
   for (h=0; h<lopind; h++) {
      kount=kount+cyccnt[h];
      fprintf(Outfp,"%d,",cyccnt[h]);
      total=total+1;
      if (total>20) {
	 fprintf(Outfp,"\n");
	 total=0;
	 }
      }
   fprintf(Outfp,"\n");
   fprintf(Outfp,"c values=%d, cycles=%d \n",lopind,kount);
   }
else {
   fprintf(Outfp,"\n");
   fprintf(Outfp,"unsigned char cflag[      +1]={ \n");
   total=0;
   for (h=0; h<index; h++) {
      fprintf(Outfp,"%d,",cyc[h]);
      total=total+1;
      if (total>20) {
	 fprintf(Outfp,"\n");
	 total=0;
	 }
      }
   fprintf(Outfp,"255};");
   fprintf(Outfp," //  index=%d \n",index);
   fprintf(Outfp,"unsigned int size[   ]={ \n");
   total=0;
   for (h=0; h<lopind; h++) {
      fprintf(Outfp,"%d,",lopcnt[h]);
      total=total+1;
      if (total>20) {
	 fprintf(Outfp,"\n");
	 total=0;
	 }
      }
   fprintf(Outfp,"\n");
   fprintf(Outfp,"int cval[   ]={ \n");
   index=0;
   total=0;
   for (h=(unsigned int)cstart; h<=(unsigned int)cend; h+=2) {
      if (h==(h/3)*3)
	 continue;
      index=index+1;
      fprintf(Outfp,"%d,",h);
      total=total+1;
      if (total>10) {
	 fprintf(Outfp,"\n");
	 total=0;
	 }
      }
   fprintf(Outfp,"\n");
   fprintf(Outfp,"unsigned int numbc=%d;   \n",index);
   }
zskip:
fclose(Outfp);
return(0);
}
