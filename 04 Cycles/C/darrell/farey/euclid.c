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
