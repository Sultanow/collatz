/*****************************************************************************/
/*									     */
/*  COMPUTE (n-x,x)							     */
/*  01/15/2021 Darrell Cox						     */
/*									     */
/*****************************************************************************/
#include <stdio.h>
#include <math.h>
int main () {
unsigned int l=97;  // prime only
unsigned int r,n,x,temp;
FILE *Outfp;
Outfp = fopen("out2.dat","w");
for (r=1; r<l; r++) {
   n=1;
   temp=r*n+1;
   while (temp!=(temp/l)*l) {
      n=n+1;
      if (n>=l)
	 goto askip;
      temp=r*n+1;
      }
   x=l-1;
   temp=r*(n-x)-(l-r)*x+1;
   while (temp!=(temp/l)*l) {
      x=x-1;
      temp=r*(n-x)-(l-r)*x+1;
      if (x<=0)
	 break;
      }
   x=x-1;
   temp=r*(n-x)-(l-r)*x+1;
   if (temp!=(temp/l)*l) {
      printf("error \n");
      break;
      }
   printf("n=%d, n-x=%d, x=%d, r=%d \n",n,n-x,x,r);
   fprintf(Outfp," %d %d %d %d \n",n,n-x,x,r);
askip:
   continue;
   }
fclose(Outfp);
return(0);
}
