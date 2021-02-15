/******************************************************************************/
/*									      */
/* COMPUTE CEILING AND FLOOR FUNCTIONS					      */
/* 12/12/20 (dkc)							      */
/*									      */
/* This C program finds a rotation of the floor function parity vector that   */
/* matches the ceiling function parity vector.				      */
/*									      */
/******************************************************************************/
#include <stdio.h> 
#include <math.h> 

int main () {
unsigned int l=15;  // number of odds and evens
unsigned int n=7;  // number of odds
unsigned int j,a,b,c,d,ws,wp,mask;
unsigned int sv[1000],pv[1000],r[1000];
FILE *Outfp;
Outfp = fopen("out14q.dat","w");
//
// compute rotated parity vector using ceiling function
//
      ws=0;
      wp=0;
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
//
         c=j*n;
	 c=(c/l);
         d=(j-1)*n;
	 d=(d/l); 
	 sv[j-1]=c-d;
	 pv[j-1]=a-b;
	 ws=ws*2+c-d;
	 wp=wp*2+a-b;
	 printf(" %d %d \n ",sv[j-1],pv[j-1]);
	 }
	 printf("floor=%#10x, ceiling=%#10x \n",ws,wp);
	 mask=(2<<(l-1))-1;
	 a=ws;
	 r[0]=a;
	 for (j=1; j<l; j++) {
	    b=a>>(l-1);
//	    printf("b=%d, ",b);
	    if (b==0)
	      a=(a*2)&mask;
	    else
	      a=((a*2)&mask)+1;
	    r[j]=a;
	    printf(" %#10x \n",a);
	    }
	 for (j=0; j<l; j++) {
	    if (r[j]==wp) {
	       printf("match=%d \n",j);
	       break;
	       }
	    }
fclose(Outfp);
return(0);
} 
