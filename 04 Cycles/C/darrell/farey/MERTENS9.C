/*CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                             C
C  COMPUTE NUMBER OF FRACTIONS						      C
C  12/19/14 (DKC)							      C
C                                                                             C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC*/
#include <math.h>
#include <stdlib.h>
unsigned int haros5(unsigned int N, unsigned int M, unsigned int *R,
		    unsigned int H, unsigned int K, unsigned int HP,
		    unsigned int KP, unsigned int D);
unsigned int lagrange1(unsigned int N, unsigned int D, unsigned int O);
void mertens9(unsigned int N, unsigned int *count, unsigned int *S, unsigned int flag) {
unsigned int L,count1,count2,ctemp1,ctemp2;
if (N==1) {
   count[0]=0;
   count[1]=0;
   return;
   }
if (N==2) {
   count[0]=0;
   count[1]=0;
   return;
   }
if (N==3) {
   count[0]=0;
   if (flag==0)
      count[1]=0;
   else
      count[1]=1;
   return;
   }
if (N==4) {
   count[0]=0;
   count[1]=1;
   return;
   }
if ((N>4)&&(N<129)) {
   count1=haros5(N,0,S,0,1,1,N,4);
   count1=count1-2;
   count[0]=count1;
   L=lagrange1(1,4,N);
   count2=haros5(N,0,S,1,4,L>>16,L&0xffff,2);
   count2=count2-2;
   count[1]=count2;
   return;
   }
if (N>50000) {
   count[0]=0;
   count[1]=0;
   return;
   }
ctemp1=haros5(N,0,S,0,1,1,N,128);	      // towards 1/128
L=lagrange1(1,128,N);
count1=haros5(N,0,S,1,128,L>>16,L&0xffff,64); // towards 1/64
count1=count1+ctemp1-2;

L=lagrange1(1,64,N);
ctemp1=haros5(N,0,S,1,64,L>>16,L&0xffff,128);  // towards 3/128
count1=count1+ctemp1-1;
L=lagrange1(3,128,N);
ctemp1=haros5(N,0,S,3,128,L>>16,L&0xffff,32);  // towards 1/32
count1=count1+ctemp1-1;

L=lagrange1(1,32,N);
ctemp1=haros5(N,0,S,1,32,L>>16,L&0xffff,128);  // towards 5/128
count1=count1+ctemp1-1;
L=lagrange1(5,128,N);
ctemp1=haros5(N,0,S,5,128,L>>16,L&0xffff,64);  // towards 3/64
count1=count1+ctemp1-1;

L=lagrange1(3,64,N);
ctemp1=haros5(N,0,S,3,64,L>>16,L&0xffff,128);  // towards 7/128
count1=count1+ctemp1-1;
L=lagrange1(7,128,N);
ctemp1=haros5(N,0,S,7,128,L>>16,L&0xffff,16);  // towards 1/16
count1=count1+ctemp1-1;

L=lagrange1(1,16,N);
ctemp1=haros5(N,0,S,1,16,L>>16,L&0xffff,128);  // towards 9/128
count1=count1+ctemp1-1;
L=lagrange1(9,128,N);
ctemp1=haros5(N,0,S,9,128,L>>16,L&0xffff,64);  // towards 5/64
count1=count1+ctemp1-1;

L=lagrange1(5,64,N);
ctemp1=haros5(N,0,S,5,64,L>>16,L&0xffff,128);  // towards 11/128
count1=count1+ctemp1-1;
L=lagrange1(11,128,N);
ctemp1=haros5(N,0,S,11,128,L>>16,L&0xffff,32);	// towards 3/32
count1=count1+ctemp1-1;

L=lagrange1(3,32,N);
ctemp1=haros5(N,0,S,3,32,L>>16,L&0xffff,128);  // towards 13/128
count1=count1+ctemp1-1;
L=lagrange1(13,128,N);
ctemp1=haros5(N,0,S,13,128,L>>16,L&0xffff,64);	 // towards 7/64
count1=count1+ctemp1-1;

L=lagrange1(7,64,N);
ctemp1=haros5(N,0,S,7,64,L>>16,L&0xffff,128);	// towards 15/128
count1=count1+ctemp1-1;
L=lagrange1(15,128,N);
ctemp1=haros5(N,0,S,15,128,L>>16,L&0xffff,8);	// towards 1/8
count1=count1+ctemp1-1;

L=lagrange1(1,8,N);
ctemp1=haros5(N,0,S,1,8,L>>16,L&0xffff,128);   // towards 17/128
count1=count1+ctemp1-1;
L=lagrange1(17,128,N);
ctemp1=haros5(N,0,S,17,128,L>>16,L&0xffff,64);	// towards 9/64
count1=count1+ctemp1-1;

L=lagrange1(9,64,N);
ctemp1=haros5(N,0,S,9,64,L>>16,L&0xffff,128);  // towards 19/128
count1=count1+ctemp1-1;
L=lagrange1(19,128,N);
ctemp1=haros5(N,0,S,19,128,L>>16,L&0xffff,32);	// towards 5/32
count1=count1+ctemp1-1;

L=lagrange1(5,32,N);
ctemp1=haros5(N,0,S,5,32,L>>16,L&0xffff,128);  // towards 21/128
count1=count1+ctemp1-1;
L=lagrange1(21,128,N);
ctemp1=haros5(N,0,S,21,128,L>>16,L&0xffff,64);	// towards 11/64
count1=count1+ctemp1-1;

L=lagrange1(11,64,N);
ctemp1=haros5(N,0,S,11,64,L>>16,L&0xffff,128);	// towards 23/128
count1=count1+ctemp1-1;
L=lagrange1(23,128,N);
ctemp1=haros5(N,0,S,23,128,L>>16,L&0xffff,16);	 // towards 3/16
count1=count1+ctemp1-1;

L=lagrange1(3,16,N);
ctemp1=haros5(N,0,S,3,16,L>>16,L&0xffff,128);	// towards 25/128
count1=count1+ctemp1-1;
L=lagrange1(25,128,N);
ctemp1=haros5(N,0,S,25,128,L>>16,L&0xffff,64);	// towards 13/64
count1=count1+ctemp1-1;

L=lagrange1(13,64,N);
ctemp1=haros5(N,0,S,13,64,L>>16,L&0xffff,128);	// towards 27/128
count1=count1+ctemp1-1;
L=lagrange1(27,128,N);
ctemp1=haros5(N,0,S,27,128,L>>16,L&0xffff,32);	 // towards 7/32
count1=count1+ctemp1-1;

L=lagrange1(7,32,N);
ctemp1=haros5(N,0,S,7,32,L>>16,L&0xffff,128);	// towards 29/128
count1=count1+ctemp1-1;
L=lagrange1(29,128,N);
ctemp1=haros5(N,0,S,29,128,L>>16,L&0xffff,64);	 // towards 15/64
count1=count1+ctemp1-1;

L=lagrange1(15,64,N);
ctemp1=haros5(N,0,S,15,64,L>>16,L&0xffff,128);	 // towards 31/128
count1=count1+ctemp1-1;
L=lagrange1(31,128,N);
ctemp1=haros5(N,0,S,31,128,L>>16,L&0xffff,4);	// towards 1/4
count1=count1+ctemp1-2;
count[0]=count1;
//
L=lagrange1(1,4,N);
ctemp2=haros5(N,0,S,1,4,L>>16,L&0xffff,128);	// towards 33/128
L=lagrange1(33,128,N);
count2=haros5(N,0,S,33,128,L>>16,L&0xffff,64);	  // towards 17/64
count2=count2+ctemp2-1;

L=lagrange1(17,64,N);
ctemp2=haros5(N,0,S,17,64,L>>16,L&0xffff,128);	// towards 35/128
count2=count2+ctemp2-1;
L=lagrange1(35,128,N);
ctemp2=haros5(N,0,S,35,128,L>>16,L&0xffff,32);	// towards 9/32
count2=count2+ctemp2-1;

L=lagrange1(9,32,N);
ctemp2=haros5(N,0,S,9,32,L>>16,L&0xffff,128);	// towards 37/128
count2=count2+ctemp2-1;
L=lagrange1(37,128,N);
ctemp2=haros5(N,0,S,37,128,L>>16,L&0xffff,64);	 // towards 19/64
count2=count2+ctemp2-1;

L=lagrange1(19,64,N);
ctemp2=haros5(N,0,S,19,64,L>>16,L&0xffff,128);	// towards 39/128
count2=count2+ctemp2-1;
L=lagrange1(39,128,N);
ctemp2=haros5(N,0,S,39,128,L>>16,L&0xffff,16);	// towards 5/16
count2=count2+ctemp2-1;

L=lagrange1(5,16,N);
ctemp2=haros5(N,0,S,5,16,L>>16,L&0xffff,128);	// towards 41/128
count2=count2+ctemp2-1;
L=lagrange1(41,128,N);
ctemp2=haros5(N,0,S,41,128,L>>16,L&0xffff,64);	 // towards 21/64
count2=count2+ctemp2-1;

L=lagrange1(21,64,N);
ctemp2=haros5(N,0,S,21,64,L>>16,L&0xffff,128);	// towards 43/128
count2=count2+ctemp2-1;
L=lagrange1(43,128,N);
ctemp2=haros5(N,0,S,43,128,L>>16,L&0xffff,32);	// towards 11/32
count2=count2+ctemp2-1;

L=lagrange1(11,32,N);
ctemp2=haros5(N,0,S,11,32,L>>16,L&0xffff,128);	// towards 45/128
count2=count2+ctemp2-1;
L=lagrange1(45,128,N);
ctemp2=haros5(N,0,S,45,128,L>>16,L&0xffff,64);	// towards 23/64
count2=count2+ctemp2-1;

L=lagrange1(23,64,N);
ctemp2=haros5(N,0,S,23,64,L>>16,L&0xffff,128);	 // towards 47/128
count2=count2+ctemp2-1;
L=lagrange1(47,128,N);
ctemp2=haros5(N,0,S,47,128,L>>16,L&0xffff,8);	// towards 3/8
count2=count2+ctemp2-1;

L=lagrange1(3,8,N);
ctemp2=haros5(N,0,S,3,8,L>>16,L&0xffff,128);	// towards 49/128
count2=count2+ctemp2-1;
L=lagrange1(49,128,N);
ctemp2=haros5(N,0,S,49,128,L>>16,L&0xffff,64);	  // towards 25/64
count2=count2+ctemp2-1;

L=lagrange1(25,64,N);
ctemp2=haros5(N,0,S,25,64,L>>16,L&0xffff,128);	// towards 51/128
count2=count2+ctemp2-1;
L=lagrange1(51,128,N);
ctemp2=haros5(N,0,S,51,128,L>>16,L&0xffff,32);	// towards 13/32
count2=count2+ctemp2-1;

L=lagrange1(13,32,N);
ctemp2=haros5(N,0,S,13,32,L>>16,L&0xffff,128);	// towards 53/128
count2=count2+ctemp2-1;
L=lagrange1(53,128,N);
ctemp2=haros5(N,0,S,53,128,L>>16,L&0xffff,64);	// towards 27/64
count2=count2+ctemp2-1;

L=lagrange1(27,64,N);
ctemp2=haros5(N,0,S,27,64,L>>16,L&0xffff,128);	// towards 55/128
count2=count2+ctemp2-1;
L=lagrange1(55,128,N);
ctemp2=haros5(N,0,S,55,128,L>>16,L&0xffff,16);	// towards 7/16
count2=count2+ctemp2-1;

L=lagrange1(7,16,N);
ctemp2=haros5(N,0,S,7,16,L>>16,L&0xffff,128);	// towards 57/128
count2=count2+ctemp2-1;
L=lagrange1(57,128,N);
ctemp2=haros5(N,0,S,57,128,L>>16,L&0xffff,64);	 // towards 29/64
count2=count2+ctemp2-1;

L=lagrange1(29,64,N);
ctemp2=haros5(N,0,S,29,64,L>>16,L&0xffff,128);	// towards 59/128
count2=count2+ctemp2-1;
L=lagrange1(59,128,N);
ctemp2=haros5(N,0,S,59,128,L>>16,L&0xffff,32);	// towards 15/32
count2=count2+ctemp2-1;

L=lagrange1(15,32,N);
ctemp2=haros5(N,0,S,15,32,L>>16,L&0xffff,128);	// towards 61/128
count2=count2+ctemp2-1;
L=lagrange1(61,128,N);
ctemp2=haros5(N,0,S,61,128,L>>16,L&0xffff,64);	// towards 31/64
count2=count2+ctemp2-1;

L=lagrange1(31,64,N);
ctemp2=haros5(N,0,S,31,64,L>>16,L&0xffff,128);	 // towards 63/128
count2=count2+ctemp2-1;
L=lagrange1(63,128,N);
ctemp2=haros5(N,0,S,63,128,L>>16,L&0xffff,2);	// towards 1/2
count2=count2+ctemp2-3;
count[1]=count2;
return;
}
